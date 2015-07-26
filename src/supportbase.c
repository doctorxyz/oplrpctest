#include "include/opl.h"
#include "include/lang.h"
#include "include/util.h"
#include "include/iosupport.h"
#include "include/system.h"
#include "include/supportbase.h"
#include "include/ioman.h"
#include "modules/iopcore/common/cdvd_config.h"
#ifdef GSM
#include "include/pggsm.h"
#endif

#include <sys/fcntl.h>

/// internal linked list used to populate the list from directory listing
struct game_list_t {
	base_game_info_t gameinfo;
	struct game_list_t *next;
};

int sbIsSameSize(const char* prefix, int prevSize) {
	int size = -1;
	char path[256];
	snprintf(path, sizeof(path), "%sul.cfg", prefix);

	int fd = openFile(path, O_RDONLY);
	if (fd >= 0) {
		size = getFileSize(fd);
		fileXioClose(fd);
	}

	return size == prevSize;
}

int sbCreateSemaphore(void)
{
	ee_sema_t sema;

	sema.option = sema.attr = 0;
	sema.init_count = 1;
	sema.max_count = 1;
	return CreateSema(&sema);
}

//0 = Not ISO disc image, GAME_FORMAT_OLD_ISO = legacy ISO disc image (filename follows old naming requirement), GAME_FORMAT_ISO = plain ISO image.
static int isValidIsoName(char *name, int *pNameLen)
{
	// Old ISO image naming format: SCUS_XXX.XX.ABCDEFGHIJKLMNOP.iso

	// Minimum is 17 char, GameID (11) + "." (1) + filename (1 min.) + ".iso" (4)
	int size = strlen(name);
	if(stricmp(&name[size - 4], ".iso") == 0)
	{
		if ((size >= 17) && (name[4] == '_') && (name[8] == '.') && (name[11] == '.')) {
			*pNameLen = size - 16;
			return GAME_FORMAT_OLD_ISO;
		}else if(size >= 5) {
			*pNameLen = size - 4;
			return GAME_FORMAT_ISO;
		}
	}

	return 0;
}

static inline int GetStartupExecName(const char *path, char *filename, int maxlength)
{
	char *SystemCNF, *NextLine, *p;
	int result, size;
	FILE *file;

	if((file = fopen(path, "r")) != NULL)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);
		if((SystemCNF = malloc(size + 1)) != NULL)
		{
			fread(SystemCNF, 1, size, file);
			fclose(file);
			SystemCNF[size]='\0';

			NextLine = strtok(SystemCNF, "\n\r");
			while((NextLine != NULL) && (strncmp(NextLine, "BOOT2", 5) != 0))
			{
				NextLine = strtok(NULL, "\n\r");
			}

			free(SystemCNF);

			if(NextLine != NULL && strcmp(strtok(NextLine, " ="), "BOOT2") == 0)
			{
				if((p = strtok(NULL, " =")) != NULL && strncmp(p, "cdrom0:\\", 8)==0)
				{
					strncpy(filename, p+8, maxlength);	/* Skip the device name part of the path ("cdrom0:\"). */
					filename[maxlength]='\0';
					LOG("Startup EXEC path: %s\n", filename);
					result = 0;
				}else{
					LOG("BOOT 2 errror: Unsupported boot device.\n");
					result = -EINVAL;
				}
			}else{
				LOG("BOOT 2 line not found.\n");
				result = -EINVAL;
			}
		}else{
			result = -ENOMEM;
		}
	}
	else result = -ENOENT;

	return result;
}

static int scanForISO(char* path, char type, struct game_list_t** glist) {
	int fd, NameLen, count = 0, format, MountFD;
	char fullpath[256], startup[GAME_STARTUP_MAX];
	iox_dirent_t record;

	if ((fd = fileXioDopen(path)) > 0) {
		while (fileXioDread(fd, &record) > 0) {
			if ((format = isValidIsoName(record.name, &NameLen)) > 0) {
				base_game_info_t *game;

				if(NameLen > ISO_GAME_NAME_MAX) continue;	//Skip files that cannot be supported properly.

				if(format == GAME_FORMAT_OLD_ISO)
				{
					struct game_list_t *next = (struct game_list_t*)malloc(sizeof(struct game_list_t));

					if(next != NULL)
					{
						next->next = *glist;
						*glist = next;

						game = &(*glist)->gameinfo;

						strncpy(game->name, &record.name[GAME_STARTUP_MAX], NameLen);
						game->name[NameLen] = '\0';
						strncpy(game->startup, record.name, GAME_STARTUP_MAX - 1);
						game->startup[GAME_STARTUP_MAX - 1] = '\0';
						strncpy(game->extension, &record.name[GAME_STARTUP_MAX + NameLen], sizeof(game->extension));
						game->extension[sizeof(game->extension) - 1] = '\0';
					}else{
						//Out of memory.
						break;
					}
				}else{
					sprintf(fullpath, "%s/%s", path, record.name);
					if((MountFD = fileXioMount("iso:", fullpath, FIO_MT_RDONLY)) >= 0)
					{
						if(GetStartupExecName("iso:/SYSTEM.CNF;1", startup, GAME_STARTUP_MAX - 1) == 0)
						{
							struct game_list_t *next = (struct game_list_t*)malloc(sizeof(struct game_list_t));

							if(next != NULL)
							{
								next->next = *glist;
								*glist = next;

								game = &(*glist)->gameinfo;

								strcpy(game->startup, startup);
								strncpy(game->name, record.name, NameLen);
								game->name[NameLen] = '\0';
								strncpy(game->extension, &record.name[NameLen], sizeof(game->extension));
								game->extension[sizeof(game->extension) - 1] = '\0';
							}else{
								//Out of memory.
								fileXioUmount("iso:");
								break;
							}
						}else{
							//Unable to parse SYSTEM.CNF.
							fileXioUmount("iso:");
							continue;
						}

						fileXioUmount("iso:");
					}else{
						//Unable to mount game.
						continue;
					}
				}

				game->parts = 1;
				game->media = type;
				game->format = format;
				game->sizeMB = (record.stat.size >> 20) | (record.stat.hisize << 12);

				count++;
			}
		}
		fileXioDclose(fd);
	}else{
		count = fd;
	}

	return count;
}

int sbReadList(base_game_info_t **list, const char* prefix, int *fsize, int* gamecount) {
	int fd, size, id = 0, result;
	int count;
	char path[256];

	free(*list);
	*list = NULL;
	*fsize = -1;
	*gamecount = 0;

	// temporary storage for the game names
	struct game_list_t *dlist_head = NULL;

	// count iso games in "cd" directory
	snprintf(path, sizeof(path), "%sCD", prefix);
	count = scanForISO(path, 0x12, &dlist_head);

	// count iso games in "dvd" directory
	snprintf(path, sizeof(path), "%sDVD", prefix);
	if((result = scanForISO(path, 0x14, &dlist_head)) >= 0){
		count = count<0?result:count+result;
	}

	// count and process games in ul.cfg
	snprintf(path, sizeof(path), "%sul.cfg", prefix);
	fd = openFile(path, O_RDONLY);
	if(fd >= 0) {
		char buffer[0x040];

		if(count < 0) count = 0;
		size = getFileSize(fd);
		*fsize = size;
		count += size / 0x040;

		if (count > 0) {
			if((*list = (base_game_info_t*)malloc(sizeof(base_game_info_t) * count)) != NULL) {
				while (size > 0) {
					fileXioRead(fd, buffer, 0x40);

					base_game_info_t *g = &(*list)[id++];

					// to ensure no leaks happen, we copy manually and pad the strings
					memcpy(g->name, buffer, UL_GAME_NAME_MAX);
					g->name[UL_GAME_NAME_MAX] = '\0';
					memcpy(g->startup, &buffer[UL_GAME_NAME_MAX + 3], GAME_STARTUP_MAX);
					g->startup[GAME_STARTUP_MAX] = '\0';
					g->extension[0] = '\0';
					memcpy(&g->parts, &buffer[47], 1);
					memcpy(&g->media, &buffer[48], 1);
					g->format = GAME_FORMAT_USBLD;
					g->sizeMB = -1;
					size -= 0x40;
				}
			}
		}
		fileXioClose(fd);
	}
	else if (count > 0){
		*list = (base_game_info_t*)malloc(sizeof(base_game_info_t) * count);
	}

	if(*list != NULL) {
		// copy the dlist into the list
		while ((id < count) && dlist_head) {
			// copy one game, advance
			struct game_list_t *cur = dlist_head;
			dlist_head = dlist_head->next;

			memcpy(&(*list)[id++], &cur->gameinfo, sizeof(base_game_info_t));
			free(cur);
		}
	}else
		count = 0;

	if(count > 0) *gamecount = count;

	return count;
}

u32 sbGetISO9660MaxLBA(const char *path) {
	u32 maxLBA;
	FILE *file;

	if((file = fopen(path, "rb")) != NULL) {
		fseek(file, 16*2048+80, SEEK_SET);
		if(fread(&maxLBA, sizeof(maxLBA), 1, file) != 1)
			maxLBA = 0;
		fclose(file);
	}else{
		maxLBA = 0;
	}

	return maxLBA;
}

int sbProbeISO9660(const char *path, base_game_info_t* game, u32 layer1_offset) {
	int result;
	FILE *file;
	char buffer[6];

	result = -1;
	if(game->media == 0x14) {	//Only DVDs can have multiple layers.
		if((file = fopen(path, "rb")) != NULL) {
			if(fseek(file, layer1_offset * 2048, SEEK_SET) == 0) {
				if((fread(buffer, 1, sizeof(buffer), file) == sizeof(buffer)) &&
					((buffer[0x00] == 1) && (!strncmp(&buffer[0x01], "CD001", 5)))) {
					result = 0;
				}
			}
			fclose(file);
		}
	}

	return result;
}

int sbProbeISO9660_64(const char *path, base_game_info_t* game, u32 layer1_offset) {
	int result, fd;
	char buffer[6];

	result = -1;
	if(game->media == 0x14) {	//Only DVDs can have multiple layers.
		if((fd = fileXioOpen(path, O_RDONLY, 0666)) >= 0) {
			if(fileXioLseek64(fd, (u64)layer1_offset * 2048, SEEK_SET) == (u64)layer1_offset * 2048) {
				if((fileXioRead(fd, buffer, sizeof(buffer)) == sizeof(buffer)) &&
					((buffer[0x00] == 1) && (!strncmp(&buffer[0x01], "CD001", 5)))) {
					result = 0;
				}
			}
			fileXioClose(fd);
		}else result = fd;
	}

	return result;
}

int sbPrepare(base_game_info_t* game, config_set_t* configSet, int size_cdvdman, void** cdvdman_irx, int* patchindex) {
	int i;
	static const struct cdvdman_settings_common cdvdman_settings_common_sample={
		0x69, 0x69,
		0x1234,
		0x39393939,
		0x00000000,
		"B00BS"
	};
	struct cdvdman_settings_common *settings;

	unsigned int compatmask = 0;
	configGetInt(configSet, CONFIG_ITEM_COMPAT, &compatmask);

	char gameid[5];
	configGetDiscIDBinary(configSet, gameid);

	for (i = 0, settings = NULL; i < size_cdvdman; i+=4) {
		if (!memcmp((void*)((u8*)cdvdman_irx + i), &cdvdman_settings_common_sample, sizeof(cdvdman_settings_common_sample))) {
			settings=(struct cdvdman_settings_common*)((u8*)cdvdman_irx + i);
			break;
		}
	}
	if (settings == NULL) return -1;

	if(game != NULL){
		settings->NumParts = game->parts;
		settings->media = game->media;
	}
	settings->flags = 0;

	if (compatmask & COMPAT_MODE_1) {
		settings->flags |= IOPCORE_COMPAT_ACCU_READS;
	}

	if (compatmask & COMPAT_MODE_2) {
		settings->flags |= IOPCORE_COMPAT_ALT_READ;
	}

	if (compatmask & COMPAT_MODE_4) {
		settings->flags |= IOPCORE_COMPAT_0_PSS;
	}

	if (compatmask & COMPAT_MODE_5) {
		settings->flags |= IOPCORE_COMPAT_EMU_DVDDL;
	}

	if (compatmask & COMPAT_MODE_6) {
		settings->flags |= IOPCORE_ENABLE_POFF;
	}

#ifdef GSM
	gEnableGSM = 0;
	if (configGetInt(configSet, CONFIG_ITEM_ENABLEGSM, &gEnableGSM) && gEnableGSM) {
		//Load the rest of the per-game GSM configuration, only if GSM is enabled.
		configGetInt(configSet, CONFIG_ITEM_GSMVMODE, &gGSMVMode);
		configGetInt(configSet, CONFIG_ITEM_GSMXOFFSET, &gGSMXOffset);
		configGetInt(configSet, CONFIG_ITEM_GSMYOFFSET, &gGSMYOffset);
		configGetInt(configSet, CONFIG_ITEM_GSMSKIPVIDEOS, &gGSMSkipVideos);

	}
#endif

	// patch cdvdman timer
	int timer = 0;
	u32 cdvdmanTimer = 0;
	if (configGetInt(configSet, CONFIG_ITEM_CDVDMAN_TIMER, &timer)) {
		cdvdmanTimer = timer * 250;
	}
	settings->cb_timer = cdvdmanTimer;

	*patchindex = i;

	// game id
	memcpy(settings->DiscID, gameid, 5);

	return compatmask;
}

static void sbRebuildULCfg(base_game_info_t **list, const char* prefix, int gamecount, int excludeID) {
	char path[256];
	snprintf(path, sizeof(path), "%sul.cfg", prefix);

	file_buffer_t* fileBuffer = openFileBuffer(path, O_WRONLY | O_CREAT | O_TRUNC, 0, 4096);
	if (fileBuffer) {
		int i;
		char buffer[0x40];
		base_game_info_t* game;

		memset(buffer, 0, 0x40);
		buffer[32] = 0x75; // u
		buffer[33] = 0x6C; // l
		buffer[34] = 0x2E; // .
		buffer[53] = 0x08; // just to be compatible with original ul.cfg

		for (i = 0; i < gamecount; i++) {
			game = &(*list)[i];

			if (game->format == GAME_FORMAT_USBLD  && (i != excludeID)) {
				memset(buffer, 0, UL_GAME_NAME_MAX);
				memset(&buffer[UL_GAME_NAME_MAX + 3], 0, GAME_STARTUP_MAX);

				memcpy(buffer, game->name, UL_GAME_NAME_MAX);
				memcpy(&buffer[UL_GAME_NAME_MAX + 3], game->startup, GAME_STARTUP_MAX);
				buffer[47] = game->parts;
				buffer[48] = game->media;

				writeFileBuffer(fileBuffer, buffer, 0x40);
			}
		}

		closeFileBuffer(fileBuffer);
	}
}

void sbDelete(base_game_info_t **list, const char* prefix, const char* sep, int gamecount, int id) {
	char path[256];
	base_game_info_t* game = &(*list)[id];

	if (game->format != GAME_FORMAT_USBLD) {
		if (game->media == 0x12)
			snprintf(path, sizeof(path), "%sCD%s%s.%s%s", prefix, sep, game->startup, game->name, game->extension);
		else
			snprintf(path, sizeof(path), "%sDVD%s%s.%s%s", prefix, sep, game->startup, game->name, game->extension);
		fileXioRemove(path);
	} else {
		char *pathStr = "%sul.%08X.%s.%02x";
		unsigned int crc = USBA_crc32(game->name);
		int i = 0;
		do {
			snprintf(path, sizeof(path), pathStr, prefix, crc, game->startup, i++);
			fileXioRemove(path);
		} while(i < game->parts);

		sbRebuildULCfg(list, prefix, gamecount, id);
	}
}

void sbRename(base_game_info_t **list, const char* prefix, const char* sep, int gamecount, int id, char* newname) {
	char oldpath[256], newpath[256];
	base_game_info_t* game = &(*list)[id];

	if (game->format != GAME_FORMAT_USBLD) {
		if (game->media == 0x12) {
			snprintf(oldpath, sizeof(oldpath), "%sCD%s%s.%s%s", prefix, sep, game->startup, game->name, game->extension);
			snprintf(newpath, sizeof(newpath), "%sCD%s%s.%s%s", prefix, sep, game->startup, newname, game->extension);
		} else {
			snprintf(oldpath, sizeof(oldpath), "%sDVD%s%s.%s%s", prefix, sep, game->startup, game->name, game->extension);
			snprintf(newpath, sizeof(newpath), "%sDVD%s%s.%s%s", prefix, sep, game->startup, newname, game->extension);
		}
		fileXioRename(oldpath, newpath);
	} else {
		memset(game->name, 0, UL_GAME_NAME_MAX);
		memcpy(game->name, newname, UL_GAME_NAME_MAX);

		char *pathStr = "%sul.%08X.%s.%02x";
		unsigned int oldcrc = USBA_crc32(game->name);
		unsigned int newcrc = USBA_crc32(newname);
		int i = 0;
		do {
			snprintf(oldpath, sizeof(oldpath), pathStr, prefix, oldcrc, game->startup, i);
			snprintf(newpath, sizeof(newpath), pathStr, prefix, newcrc, game->startup, i++);
			fileXioRename(oldpath, newpath);
		} while(i < game->parts);

		sbRebuildULCfg(list, prefix, gamecount, -1);
	}
}

config_set_t* sbPopulateConfig(base_game_info_t* game, const char* prefix, const char* sep) {
	char path[256];
#if OPL_IS_DEV_BUILD
	snprintf(path, sizeof(path), "%sCFG-DEV%s%s.cfg", prefix, sep, game->startup);
#else
	snprintf(path, sizeof(path), "%sCFG%s%s.cfg", prefix, sep, game->startup);
#endif
	config_set_t* config = configAlloc(0, NULL, path);
	configRead(config);

	configSetStr(config, CONFIG_ITEM_NAME, game->name);
	if (game->sizeMB != -1)
		configSetInt(config, CONFIG_ITEM_SIZE, game->sizeMB);

	configSetStr(config, CONFIG_ITEM_FORMAT, game->format != GAME_FORMAT_USBLD ? "ISO" : "UL");
	configSetStr(config, CONFIG_ITEM_MEDIA, game->media == 0x12 ? "CD" : "DVD");

	configSetStr(config, CONFIG_ITEM_STARTUP, game->startup);

	return config;
}

void sbCreateFolders(const char *path, int createDiscImgFolders) {
	// update Themes
	char fullpath[256];

#if OPL_IS_DEV_BUILD
	sprintf(fullpath, "%sCFG-DEV", path);
#else
	sprintf(fullpath, "%sCFG", path);
#endif
	fileXioMkdir(fullpath, 0777);

	sprintf(fullpath, "%sTHM", path);
	fileXioMkdir(fullpath, 0777);

	sprintf(fullpath, "%sART", path);
	fileXioMkdir(fullpath, 0777);

	if(createDiscImgFolders)
	{
		sprintf(fullpath, "%sCD", path);
		fileXioMkdir(fullpath, 0777);

		sprintf(fullpath, "%sDVD", path);
		fileXioMkdir(fullpath, 0777);
	}

#ifdef VMC
	sprintf(fullpath, "%sVMC", path);
	fileXioMkdir(fullpath, 0777);
#endif

#ifdef CHEAT
	sprintf(fullpath, "%sCHT", path);
	fileXioMkdir(fullpath, 0777);
#endif
}
