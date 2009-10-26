#include "loader.h"

extern void *cdvdman_irx;
extern int size_cdvdman_irx;

extern void *usbd_irx;
extern int size_usbd_irx;

extern void *smbman_irx;
extern int size_smbman_irx;

extern void *dummy_irx;
extern int size_dummy_irx;

/*----------------------------------------------------------------------------------------*/
/* Copy 'size' bytes of 'eedata' from EE to 'iopptr' in IOP.                              */
/*----------------------------------------------------------------------------------------*/
 void CopyToIop(void *eedata, unsigned int size, void *iopptr){
	SifDmaTransfer_t dmadata;
	register int id;

	dmadata.src = eedata;
	dmadata.dest = iopptr;
	dmadata.size = size;
	dmadata.attr = 0;

	do
	{
		id = SifSetDma(&dmadata, 1);
	} while (!id);

	while (SifDmaStat(id) > 0) {;} 
}

/*----------------------------------------------------------------------------------------*/
/* Replace a module in a IOPRP image.                                                      */
/*----------------------------------------------------------------------------------------*/
int Patch_Mod(ioprp_t *ioprp_img, const char *name, void *modptr, int modsize)
{
	int       i, offset_in, bytes_to_move, next_mod_offset, diff;
	romdir_t *romdir_in;
	u8 *p = (u8 *)ioprp_img->data_in;

	// arrange modsize to next 16 bytes align
	if (modsize % 0x10)
		modsize = (modsize + 0x10) & 0xfffffff0;
		
	offset_in  = 0;
	romdir_in  = (romdir_t*)ioprp_img->data_in;

	// scan for module offset & size
	while(strlen(romdir_in->fileName) > 0)
	{ 
		if (!strcmp(romdir_in->fileName, name))
			break;

		if ((romdir_in->fileSize % 0x10)==0)
			offset_in += romdir_in->fileSize;
		else
			offset_in += (romdir_in->fileSize + 0x10) & 0xfffffff0;

		romdir_in++;
	}
	
	// if module found
	if (offset_in) {
		if ((romdir_in->fileSize % 0x10)==0)
			next_mod_offset = offset_in + romdir_in->fileSize;
		else
			next_mod_offset = offset_in + ((romdir_in->fileSize + 0x10) & 0xfffffff0);

		bytes_to_move = ioprp_img->size_in - next_mod_offset;
				
		// and greater than one in IOPRP img
		if (modsize >= romdir_in->fileSize) {
			diff = modsize - romdir_in->fileSize;
			if (diff > 0) {
				if (bytes_to_move > 0) {
					for (i = ioprp_img->size_in; i >= next_mod_offset; i--)
						p[i + diff] = p[i];		
												
					ioprp_img->size_in += diff;
				}
			}
		}
		// or smaller than one in IOPRP
		else {
			diff = romdir_in->fileSize - modsize;
			if (diff > 0) {
				if (bytes_to_move > 0) {
					for (i = 0; i < bytes_to_move; i++)
						p[offset_in + modsize + i] = p[next_mod_offset + i];
								
					ioprp_img->size_in -= diff;
				}
			}	
		}
		
		// finally replace module
		DIntr();
		ee_kmode_enter();
		memcpy((void *)(ioprp_img->data_in+offset_in), modptr, modsize);
		ee_kmode_exit();
		EIntr();		
		
		// correct filesize in romdir
		romdir_in->fileSize = modsize;
	}
	
	return 1;
}

/*----------------------------------------------------------------------------------------*/
/* Place additionnal modules in a EELOADCNF type image.                                   */
/*----------------------------------------------------------------------------------------*/
int Patch_EELOADCNF_Img(ioprp_t *ioprp_img)
{
	int       offset_in, offset_out;
	romdir_t *romdir_in, *romdir_out;
	char *ptr;

	offset_in  = 0;
	offset_out = 0;

	romdir_in  = (romdir_t*)ioprp_img->data_in;
	romdir_out = (romdir_t*)ioprp_img->data_out;

	// First search for EXTINFO
	while(strcmp(romdir_in->fileName, "EXTINFO")) {
		if (!strcmp(romdir_in->fileName, "ROMDIR")) {
			// copy ROMDIR to IOPRP out
			memcpy((void*)ioprp_img->data_out+offset_out, (void*)ioprp_img->data_in+offset_in, romdir_in->fileSize);
			// adjust ROMDIR size (room space for replacement modules)
			romdir_out->fileSize += 0x20;			
		}
			
		// arrange irx size to next 16 bytes multiple 
		if ((romdir_in->fileSize % 0x10)==0)
			offset_in += romdir_in->fileSize;
		else
			offset_in += (romdir_in->fileSize + 0x10) & 0xfffffff0;

		if ((romdir_out->fileSize % 0x10)==0)
			offset_out += romdir_out->fileSize;
		else
			offset_out += (romdir_out->fileSize + 0x10) & 0xfffffff0;
			
		romdir_in++;
		romdir_out++;		
	}
		
	u8 *p2 = (u8 *)ioprp_img->data_out;
	u8 *p = (u8 *)ioprp_img->data_in;
	
	// adding room space before EXTINFO (in ROMDIR)
	memcpy(&p2[offset_in+0x20], &p[offset_in], romdir_in->fileSize);
	
	// adding room space IN EXTINFO for both CDVDMAN/CDVDFSV
	romdir_out->fileSize += 0x18 + 0x18;
		
	offset_in  = 0;
	offset_out = 0;

	romdir_in  = (romdir_t*)ioprp_img->data_in;
	romdir_out = (romdir_t*)ioprp_img->data_out;
	
	while(strlen(romdir_in->fileName) > 0) {
		// copying IOPBTCONF from EELOADCNF		
		if (!strcmp(romdir_in->fileName, "IOPBTCONF")) {
			memcpy((void*)ioprp_img->data_out+offset_out, (void*)ioprp_img->data_in+offset_in, romdir_in->fileSize);
			
			// correcting NCDVDMAN (used on some Slims) into CDVDMAN into IOPBTCONF
			ptr = strstr(ioprp_img->data_in+offset_in, "NCDVDMAN");
			if (ptr) {
				char *ptr2 = strstr(ioprp_img->data_out+offset_out, "NCDVDMAN");
				int sz = (int)((u32)(ioprp_img->data_in+offset_in+romdir_in->fileSize) - (u32)ptr - 1);
				memcpy(&ptr2[0], &ptr[1], sz);
				ptr2[sz] = 0;
				romdir_out->fileSize--;
			}
			
			// correcting XCDVDMAN into CDVDMAN into IOPBTCONF
			ptr = strstr(ioprp_img->data_in+offset_in, "XCDVDMAN");
			if (ptr) {
				char *ptr2 = strstr(ioprp_img->data_out+offset_out, "XCDVDMAN");
				int sz = (int)((u32)(ioprp_img->data_in+offset_in+romdir_in->fileSize) - (u32)ptr - 1);
				memcpy(&ptr2[0], &ptr[1], sz);
				ptr2[sz] = 0;
				romdir_out->fileSize--;
			}		
			
			// adding CDVDMAN & CDVDFSV: 0 size, they'll be patched later in this function
			romdir_out++;
			memcpy(romdir_out, "CDVDMAN\0\0\0\x18\0\0\0\0\0", 0x10);
			romdir_out++;
			memcpy(romdir_out, "CDVDFSV\0\0\0\x18\0\0\0\0\0", 0x10);			
			break;
		}
		// modify EXTINFO for CDVDMAN/CDVDFSV
		else if (!strcmp(romdir_in->fileName, "EXTINFO")) {
			// without this, the replacement modules appended to the Mini Image fails to load
			memcpy((void*)ioprp_img->data_out+offset_out+romdir_in->fileSize, "\0\0\x04\x01\x12\x04\x88\x19\x99\x99\0\x02\0\0\x08\x03""CDVDMAN\0", 0x18);
			memcpy((void*)ioprp_img->data_out+offset_out+romdir_in->fileSize+0x18, "\0\0\x04\x01\x12\x04\x88\x19\x99\x99\0\x02\0\0\x08\x03""CDVDFSV\0", 0x18);
		}		
		
		// arrange irx size to next 16 bytes multiple 
		if ((romdir_in->fileSize % 0x10)==0)
			offset_in += romdir_in->fileSize;
		else
			offset_in += (romdir_in->fileSize + 0x10) & 0xfffffff0;

		if ((romdir_out->fileSize % 0x10)==0)
			offset_out += romdir_out->fileSize;
		else
			offset_out += (romdir_out->fileSize + 0x10) & 0xfffffff0;
			
		romdir_in++;
		romdir_out++;				
	}	

	ioprp_img->size_out = offset_out;

	// now we can patch IMG
	memcpy(ioprp_img->data_in, ioprp_img->data_out, ioprp_img->size_out);
	ioprp_img->size_in = ioprp_img->size_out;
		
	offset_in  = 0;
	offset_out = 0;

	romdir_in  = (romdir_t*)ioprp_img->data_in;
	romdir_out = (romdir_t*)ioprp_img->data_out;
	
	while(strlen(romdir_in->fileName) > 0) {
		if (!strcmp(romdir_in->fileName, "CDVDMAN")){
			DIntr(); // get back cdvdman drv from kernel ram		
			ee_kmode_enter();
			memcpy((void*)((u32)ioprp_img->data_out+offset_out), cdvdman_irx, size_cdvdman_irx);
			ee_kmode_exit();
			EIntr();
			romdir_out->fileSize = size_cdvdman_irx;
		}
		else if (!strcmp(romdir_in->fileName, "CDVDFSV")) {
			DIntr(); // get back dummy drv from kernel ram		
			ee_kmode_enter();
			if (GameMode == USB_MODE) {
				memcpy((void*)((u32)ioprp_img->data_out+offset_out), usbd_irx, size_usbd_irx);
				romdir_out->fileSize = size_usbd_irx;
			}
			else if (GameMode == ETH_MODE) {
				memcpy((void*)((u32)ioprp_img->data_out+offset_out), dummy_irx, size_dummy_irx);
				romdir_out->fileSize = size_dummy_irx;
			}
			ee_kmode_exit();
			EIntr();
			
		}
		else if((strcmp(romdir_in->fileName, "ROMDIR")) && (strcmp(romdir_in->fileName, "IOPBTCONF"))) {
			memcpy((void*)((u32)ioprp_img->data_out+offset_out), (void*)((u32)ioprp_img->data_in+offset_in), romdir_in->fileSize);
		}
		
		// arrange irx size to next 16 bytes multiple 
		if ((romdir_in->fileSize % 0x10)==0)
			offset_in += romdir_in->fileSize;
		else
			offset_in += (romdir_in->fileSize + 0x10) & 0xfffffff0;

		if ((romdir_out->fileSize % 0x10)==0)
			offset_out += romdir_out->fileSize;
		else
			offset_out += (romdir_out->fileSize + 0x10) & 0xfffffff0;

		romdir_in++;
		romdir_out++;
	}

	ioprp_img->size_out = offset_out;

	return 1;
}

/*----------------------------------------------------------------------------------------*/
/* NOP delay.                                                                             */
/*----------------------------------------------------------------------------------------*/
 void delay(int count)
{
	int i, ret;

	for (i  = 0; i < count; i++) {
		ret = 0x01000000;
		while(ret--) asm("nop\nnop\nnop\nnop");
	}
}


/*----------------------------------------------------------------------------------------*/
/* Apply SBV patchs.                                                                      */
/*----------------------------------------------------------------------------------------*/
 void Sbv_Patch(void)
{
	sbv_patch_enable_lmb();
	sbv_patch_disable_prefix_check();
}
