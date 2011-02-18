#include "sys/fcntl.h"
#include "include/usbld.h"
#include "include/lang.h"
#include "include/gui.h"
#include "include/supportbase.h"
#include "include/hddsupport.h"
#include "include/util.h"
#include "include/themes.h"
#include "include/textures.h"
#include "include/ioman.h"
#include "include/system.h"

extern void *hdd_cdvdman_irx;
extern int size_hdd_cdvdman_irx;

extern void *hdd_pcmcia_cdvdman_irx;
extern int size_hdd_pcmcia_cdvdman_irx;

extern void *hdd_hdpro_cdvdman_irx;
extern int size_hdd_hdpro_cdvdman_irx;

extern void *ps2dev9_irx;
extern int size_ps2dev9_irx;

extern void *ps2atad_irx;
extern int size_ps2atad_irx;

extern void *hdpro_atad_irx;
extern int size_hdpro_atad_irx;

extern void *ps2hdd_irx;
extern int size_ps2hdd_irx;

extern void *ps2fs_irx;
extern int size_ps2fs_irx;

#ifdef VMC
extern void *hdd_mcemu_irx;
extern int size_hdd_mcemu_irx;
#endif

static int hddForceUpdate = 1;
static char *hddPrefix = "pfs0:";
static int hddGameCount = 0;
static hdl_games_list_t* hddGames;

const char *oplPart = "hdd0:+OPL";

static int hddHDProKitDetected = 0;

// forward declaration
static item_list_t hddGameList;

static void hddInitModules(void) {

	hddLoadModules();

	// update Themes
	char path[255];
	sprintf(path, "%sTHM", hddPrefix);
	thmAddElements(path, "/");

#ifdef VMC
	sprintf(path, "%sVMC", hddPrefix);
	checkCreateDir(path);
#endif
}

// HD Pro Kit is mapping the 1st word in ROM0 seg as a main ATA controller,
// The pseudo ATA controller registers are accessed (input/ouput) by writing
// an id to the main ATA controller
#define HDPROreg_IO8	      (*(volatile unsigned char *)0xBFC00000) 
#define CDVDreg_STATUS        (*(volatile unsigned char *)0xBF40200A) 

static int hddCheckHDProKit(void)
{
	int ret = 0;

	DIntr();
	ee_kmode_enter();
	// HD Pro IO start commands sequence
	HDPROreg_IO8 = 0x72;
	CDVDreg_STATUS = 0;
	HDPROreg_IO8 = 0x34;
	CDVDreg_STATUS = 0;
	HDPROreg_IO8 = 0x61;
	CDVDreg_STATUS = 0;
	u32 res = HDPROreg_IO8;
	CDVDreg_STATUS = 0;
	ee_kmode_exit();
	EIntr();

	FlushCache(0);
	FlushCache(2);

	DIntr();
	ee_kmode_enter();

	// check result
	if ((res & 0xff) == 0xe7) {
		HDPROreg_IO8 = 0xe3;
		CDVDreg_STATUS = 0;
		// HD Pro IO finish commands sequence
		HDPROreg_IO8 = 0xf3;
		CDVDreg_STATUS = 0;
		ret = 1;
	}
	ee_kmode_exit();
	EIntr();

	FlushCache(0);
	FlushCache(2);

	if (ret)
		LOG("hddCheckHDPro() HD Pro Kit detected!\n");

	return ret;
}

void hddLoadModules(void) {
	int ret;
	static char hddarg[] = "-o" "\0" "4" "\0" "-n" "\0" "20";

	LOG("hddLoadModules()\n");

	gHddStartup = 4;

	ret = sysLoadModuleBuffer(&ps2dev9_irx, size_ps2dev9_irx, 0, NULL);
	if (ret < 0) {
		gHddStartup = -1;
		return;
	}

	gHddStartup = 3;

	// try to detect HD Pro Kit (not the connected HDD),
	// if detected it loads the specific ATAD module
	hddHDProKitDetected = hddCheckHDProKit();
	if (hddHDProKitDetected)
		ret = sysLoadModuleBuffer(&hdpro_atad_irx, size_hdpro_atad_irx, 0, NULL);
	else
		ret = sysLoadModuleBuffer(&ps2atad_irx, size_ps2atad_irx, 0, NULL);
	if (ret < 0) {
		gHddStartup = -1;
		return;
	}

	gHddStartup = 2;

	ret = sysLoadModuleBuffer(&ps2hdd_irx, size_ps2hdd_irx, sizeof(hddarg), hddarg);
	if (ret < 0) {
		gHddStartup = -1;
		return;
	}

	gHddStartup = 1;

	ret = sysLoadModuleBuffer(&ps2fs_irx, size_ps2fs_irx, 0, NULL);
	if (ret < 0) {
		gHddStartup = -1;
		return;
	}

	LOG("hddLoadModules: modules loaded\n");

	hddSetIdleTimeout(gHDDSpindown * 12); // gHDDSpindown [0..20] -> spindown [0..240] -> seconds [0..1200]

	ret = fileXioMount(hddPrefix, oplPart, FIO_MT_RDWR);
	if (ret < 0) {
		fileXioUmount(hddPrefix);
		fileXioMount(hddPrefix, oplPart, FIO_MT_RDWR);
	}

	gHddStartup = 0;
}

void hddInit(void) {
	LOG("hddInit()\n");
	hddForceUpdate = 1;

	ioPutRequest(IO_CUSTOM_SIMPLEACTION, &hddInitModules);

	hddGameList.enabled = 1;
}

item_list_t* hddGetObject(int initOnly) {
	if (initOnly && !hddGameList.enabled)
		return NULL;
	return &hddGameList;
}

static int hddNeedsUpdate(void) {
	if (hddForceUpdate) {
		hddForceUpdate = 0;
		return 1;
	}
	
	return 0;
}

static int hddUpdateGameList(void) {
	int ret = hddGetHDLGamelist(&hddGames);
	if (ret != 0)
		hddGameCount = 0;
	else
		hddGameCount = hddGames->count;
	return hddGameCount;
}

static int hddGetGameCount(void) {
	return hddGameCount;
}

static void* hddGetGame(int id) {
	return (void*) &hddGames->games[id];
}

static char* hddGetGameName(int id) {
	return hddGames->games[id].name;
}

static int hddGetGameNameLength(int id) {
	return HDL_GAME_NAME_MAX + 1;
}

static char* hddGetGameStartup(int id) {
	return hddGames->games[id].startup;
}

#ifndef __CHILDPROOF
static void hddDeleteGame(int id) {
	hddDeleteHDLGame(&hddGames->games[id]);
	hddForceUpdate = 1;
}

static void hddRenameGame(int id, char* newName) {
	hdl_game_info_t* game = &hddGames->games[id];
	strcpy(game->name, newName);
	hddSetHDLGameInfo(&hddGames->games[id]);
	hddForceUpdate = 1;
}
#endif

static int hddGetGameCompatibility(int id, int *dmaMode) {
	return configGetCompatibility(hddGames->games[id].startup, hddGameList.mode, dmaMode);
}

static void hddSetGameCompatibility(int id, int compatMode, int dmaMode) {
	configSetCompatibility(hddGames->games[id].startup, hddGameList.mode, compatMode, dmaMode);
}

#ifdef VMC
static int hddPrepareMcemu(hdl_game_info_t* game) {
	char vmc[2][32];
	char vmc_path[255];
	u32 vmc_size;
	pfs_inode_t pfs_inode;
	apa_header part_hdr;
	hdd_vmc_infos_t hdd_vmc_infos;
	vmc_superblock_t vmc_superblock;
	int i, j, fd, part_valid = 0, size_mcemu_irx = 0;

	configGetVMC(game->startup, vmc[0], HDD_MODE, 0);
	configGetVMC(game->startup, vmc[1], HDD_MODE, 1);

	if(!vmc[0][0] && !vmc[1][0]) return 0;  // skip if both empty

	// virtual mc informations
	memset(&hdd_vmc_infos, 0, sizeof(hdd_vmc_infos_t));
	
	fileXioUmount(hddPrefix);
	fd = fileXioOpen(oplPart, O_RDONLY, FIO_S_IRUSR | FIO_S_IWUSR | FIO_S_IXUSR | FIO_S_IRGRP | FIO_S_IWGRP | FIO_S_IXGRP | FIO_S_IROTH | FIO_S_IWOTH | FIO_S_IXOTH);
	if(fd >= 0) {
		if(fileXioIoctl2(fd, APA_IOCTL2_GETHEADER, NULL, 0, (void*)&part_hdr, sizeof(apa_header)) == sizeof(apa_header)) {
			if(part_hdr.nsub <= 4) {
				hdd_vmc_infos.parts[0].start = part_hdr.start;
				hdd_vmc_infos.parts[0].length = part_hdr.length;
				LOG("hdd_vmc_infos.parts[0].start : 0x%X\n", hdd_vmc_infos.parts[0].start);
				LOG("hdd_vmc_infos.parts[0].length : 0x%X\n", hdd_vmc_infos.parts[0].length);
				for(i = 0; i < part_hdr.nsub; i++) {
					hdd_vmc_infos.parts[i+1].start = part_hdr.subs[i].start;
					hdd_vmc_infos.parts[i+1].length = part_hdr.subs[i].length;
					LOG("hdd_vmc_infos.parts[%d].start : 0x%X\n", i+1, hdd_vmc_infos.parts[i+1].start);
					LOG("hdd_vmc_infos.parts[%d].length : 0x%X\n", i+1, hdd_vmc_infos.parts[i+1].length);
				}
				part_valid = 1;
			}
			
		}
		fileXioClose(fd);
	}

	if(!part_valid) return -1;

	fileXioMount(hddPrefix, oplPart, FIO_MT_RDWR); // if this fails, something is really screwed up
	for(i=0; i<2; i++) {
		if(!vmc[i][0]) // skip if empty
			continue;

		memset(&vmc_superblock, 0, sizeof(vmc_superblock_t));
		hdd_vmc_infos.active = 0;

		snprintf(vmc_path, 255, "%sVMC/%s.bin", hddPrefix, vmc[i]);

		fd = fileXioOpen(vmc_path, O_RDWR, FIO_S_IRUSR | FIO_S_IWUSR | FIO_S_IXUSR | FIO_S_IRGRP | FIO_S_IWGRP | FIO_S_IXGRP | FIO_S_IROTH | FIO_S_IWOTH | FIO_S_IXOTH);

		if (fd >= 0) {
			size_mcemu_irx = -1;
			LOG("%s open\n", vmc_path);

			vmc_size = fileXioLseek(fd, 0, SEEK_END);
			fileXioLseek(fd, 0, SEEK_SET);
			fileXioRead(fd, (void*)&vmc_superblock, sizeof(vmc_superblock_t));
			fileXioLseek(fd, 0, SEEK_SET);

			LOG("File size            : 0x%X\n", vmc_size);
			LOG("Magic     : %s\n", vmc_superblock.magic);
			LOG("Card type : %d\n", vmc_superblock.mc_type);

			if(!strncmp(vmc_superblock.magic, "Sony PS2 Memory Card Format", 27) && vmc_superblock.mc_type == 0x2) {
				hdd_vmc_infos.flags            = vmc_superblock.mc_flag & 0xFF;
				hdd_vmc_infos.flags           |= 0x100;
				hdd_vmc_infos.specs.page_size  = vmc_superblock.page_size;                                       
				hdd_vmc_infos.specs.block_size = vmc_superblock.pages_per_block;                                 
				hdd_vmc_infos.specs.card_size  = vmc_superblock.pages_per_cluster * vmc_superblock.clusters_per_card;

				LOG("hdd_vmc_infos.flags            : 0x%X\n", hdd_vmc_infos.flags           );
				LOG("hdd_vmc_infos.specs.page_size  : 0x%X\n", hdd_vmc_infos.specs.page_size );
				LOG("hdd_vmc_infos.specs.block_size : 0x%X\n", hdd_vmc_infos.specs.block_size);
				LOG("hdd_vmc_infos.specs.card_size  : 0x%X\n", hdd_vmc_infos.specs.card_size );

				if(vmc_size == hdd_vmc_infos.specs.card_size * hdd_vmc_infos.specs.page_size) {
					// Check vmc inode block chain (write operation can cause damage)
					if(fileXioIoctl2(fd, PFS_IOCTL2_GET_INODE, NULL, 0, (void*)&pfs_inode, sizeof(pfs_inode_t)) == sizeof(pfs_inode_t)) {
						if(pfs_inode.number_data <= 11) {
							for(j = 0; j < pfs_inode.number_data-1; j++) {
								hdd_vmc_infos.active = 1;
								hdd_vmc_infos.blocks[j].number  = pfs_inode.data[j+1].number;
								hdd_vmc_infos.blocks[j].subpart = pfs_inode.data[j+1].subpart;
								hdd_vmc_infos.blocks[j].count   = pfs_inode.data[j+1].count;
								LOG("hdd_vmc_infos.blocks[%d].number     : 0x%X\n", j, hdd_vmc_infos.blocks[j].number );
								LOG("hdd_vmc_infos.blocks[%d].subpart    : 0x%X\n", j, hdd_vmc_infos.blocks[j].subpart);
								LOG("hdd_vmc_infos.blocks[%d].count      : 0x%X\n", j, hdd_vmc_infos.blocks[j].count  );
							}
						} // else Vmc file too much fragmented
					}
				}
			}
			fileXioClose(fd);
		}
		for (j=0; j<size_hdd_mcemu_irx; j++) {
			if (((u32*)&hdd_mcemu_irx)[j] == (0xC0DEFAC0 + i)) {
				if(hdd_vmc_infos.active)
					size_mcemu_irx = size_hdd_mcemu_irx;
				memcpy(&((u32*)&hdd_mcemu_irx)[j], &hdd_vmc_infos, sizeof(hdd_vmc_infos_t));
				break;
			}
		}
	}
	return size_mcemu_irx;
}
#endif

static void hddLaunchGame(int id) {
	int i, size_irx = 0;
	void** irx = NULL;
	char filename[32];

	hdl_game_info_t* game = &hddGames->games[id];

	if (gRememberLastPlayed) {
		configSetStr(configGetByType(CONFIG_LAST), "last_played", game->startup);
		saveConfig(CONFIG_LAST, 0);
	}

	char gid[5];
	configGetDiscIDBinary(hddGames->games[id].startup, gid);

	int dmaType = 0, dmaMode = 0, compatMode = 0;
	compatMode = configGetCompatibility(hddGames->games[id].startup, hddGameList.mode, &dmaMode);
	if(dmaMode < 3)
		dmaType = 0x20;
	else {
		dmaType = 0x40;
		dmaMode -= 3;
	}
	hddSetTransferMode(dmaType, dmaMode);
	hddSetIdleTimeout(gHDDSpindown * 12);

	if (hddHDProKitDetected) {
		size_irx = size_hdd_hdpro_cdvdman_irx;
		irx = &hdd_hdpro_cdvdman_irx;
	}
	else if (sysPcmciaCheck()) {
		size_irx = size_hdd_pcmcia_cdvdman_irx;
		irx = &hdd_pcmcia_cdvdman_irx;
	}
	else {
		size_irx = size_hdd_cdvdman_irx;
		irx = &hdd_cdvdman_irx;
	}

	for (i=0;i<size_irx;i++){
		if(!strcmp((const char*)((u32)irx+i),"######    GAMESETTINGS    ######")){
			break;
		}
	}

	if (compatMode & COMPAT_MODE_2) {
		u32 alt_read_mode = 1;
		memcpy((void*)((u32)irx+i+35),&alt_read_mode,1);
	}
	if (compatMode & COMPAT_MODE_5) {
		u32 no_dvddl = 1;
		memcpy((void*)((u32)irx+i+36),&no_dvddl,4);
	}
	if (compatMode & COMPAT_MODE_4) {
		u32 no_pss = 1;
		memcpy((void*)((u32)irx+i+40),&no_pss,4);
	}

	// patch 48bit flag
	u8 flag_48bit = hddIs48bit() & 0xff;
	memcpy((void*)((u32)irx+i+34), &flag_48bit, 1);

	// patch start_sector
	memcpy((void*)((u32)irx+i+44), &game->start_sector, 4);

	for (i=0;i<size_irx;i++){
		if(!strcmp((const char*)((u32)irx+i),"B00BS")){
			break;
		}
	}
	// game id
	memcpy((void*)((u32)irx+i), &gid, 5);

	// patches cdvdfsv
	void *cdvdfsv_irx;
	int size_cdvdfsv_irx;

	sysGetCDVDFSV(&cdvdfsv_irx, &size_cdvdfsv_irx);
	u32 *p = (u32 *)cdvdfsv_irx;
	for (i = 0; i < (size_cdvdfsv_irx >> 2); i++) {
		if (*p == 0xC0DEC0DE) {
			if (compatMode & COMPAT_MODE_7)
				*p = 1;
			else
				*p = 0;
			break;
		}
		p++;
	}

#ifdef VMC
	int size_mcemu_irx = hddPrepareMcemu(game);
	if (size_mcemu_irx == -1) {
		if (guiMsgBox(_l(_STR_ERR_VMC_CONTINUE), 1, NULL))
			size_mcemu_irx = 0;
		else
			return;
	}
#endif

	sprintf(filename,"%s",game->startup);
	shutdown(NO_EXCEPTION); // CAREFUL: shutdown will call hddCleanUp, so hddGames/game will be freed
	FlushCache(0);

#ifdef VMC
	sysLaunchLoaderElf(filename, "HDD_MODE", size_irx, irx, size_mcemu_irx, &hdd_mcemu_irx, compatMode, compatMode & COMPAT_MODE_1);
#else
	sysLaunchLoaderElf(filename, "HDD_MODE", size_irx, irx, compatMode, compatMode & COMPAT_MODE_1);
#endif
}

static config_set_t* hddGetConfig(int id) {
	config_set_t* config = configAlloc(0, NULL, NULL);
	hdl_game_info_t* game = &hddGames->games[id];
	configSetStr(config, "#Format", "HDL");
	/*if (game->disctype == 0x12)
		configSetStr(config, "#Media", "CD");
	else
		configSetStr(config, "#Media", "DVD");*/ // TODO must ask Jimmi/Polo about the HDL header format
	configSetStr(config, "#Name", game->name);
	configSetStr(config, "#Startup", game->startup);
	configSetInt(config, "#Size", game->total_size_in_kb / 1024);
	return config;
}

static int hddGetImage(char* folder, int isRelative, char* value, char* suffix, GSTEXTURE* resultTex, short psm) {
	char path[255];
	if (isRelative)
		sprintf(path, "%s%s/%s_%s", hddPrefix, folder, value, suffix);
	else
		sprintf(path, "%s%s_%s", folder, value, suffix);
	return texDiscoverLoad(resultTex, path, -1, psm);
}

static void hddCleanUp(int exception) {
	if (hddGameList.enabled) {
		LOG("hddCleanUp()\n");

		hddFreeHDLGamelist(hddGames);

		if ((exception & UNMOUNT_EXCEPTION) == 0)
			fileXioUmount(hddPrefix);
	}
}

#ifdef VMC
static int hddCheckVMC(char* name, int createSize) {
	return sysCheckVMC(hddPrefix, "/", name, createSize);
}
#endif

static item_list_t hddGameList = {
		HDD_MODE, 0, 0, MENU_MIN_INACTIVE_FRAMES, "HDD Games", _STR_HDD_GAMES, &hddInit, &hddNeedsUpdate, &hddUpdateGameList,
#ifdef __CHILDPROOF
		&hddGetGameCount, &hddGetGame, &hddGetGameName, &hddGetGameNameLength, &hddGetGameStartup, NULL, NULL, &hddGetGameCompatibility,
#else
		&hddGetGameCount, &hddGetGame, &hddGetGameName, &hddGetGameNameLength, &hddGetGameStartup, &hddDeleteGame, &hddRenameGame, &hddGetGameCompatibility,
#endif
#ifdef VMC
		&hddSetGameCompatibility, &hddLaunchGame, &hddGetConfig, &hddGetImage, &hddCleanUp, &hddCheckVMC, HDD_ICON
#else
		&hddSetGameCompatibility, &hddLaunchGame, &hddGetConfig, &hddGetImage, &hddCleanUp, HDD_ICON
#endif
};
