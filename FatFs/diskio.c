/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25qxx.h"
#include "sdmmc.h"
#include "main.h"
#define SD_READ_TIMEOUT 0xff
/* Definitions of physical drive number for each drive */
#define DEV_SD_CARD		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_FLASH       1


//偏移1M    SECTOR_OFFSET = 256
#define SECTOR_OFFSET   0    /*扇区偏移量*/
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	switch (pdrv) {
	case DEV_SD_CARD :
		if(HAL_SD_GetCardState(&hsd1) != HAL_SD_STATE_ERROR )
			return RES_OK;
		else
			return RES_ERROR;
	case DEV_FLASH :
        stat = W25Qx_Init();
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res=RES_PARERR;
    if (!count)return RES_PARERR;

	switch (pdrv) {
	case DEV_SD_CARD :
		if(HAL_SD_ReadBlocks(&hsd1, (uint8_t*)buff, sector, count, SD_READ_TIMEOUT*count) != HAL_OK)
			return RES_ERROR;
		while (HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER){}
			return RES_OK;

	case DEV_FLASH :
		// translate the arguments here
        sector+=SECTOR_OFFSET;
		res = W25Qx_Read(buff, sector<<12, count<<12);

		// translate the reslut code here

		return res;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
    if (!count)return RES_PARERR;

	switch (pdrv) {
	case DEV_SD_CARD :
		if(HAL_SD_WriteBlocks(&hsd1, (uint8_t*)buff, sector, count, SD_READ_TIMEOUT*count) != HAL_OK)
			return RES_ERROR;
		while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER){}
			return RES_OK;

	case DEV_FLASH :
		// translate the arguments here
        sector+=SECTOR_OFFSET;
        W25Qx_Erase_Block(sector<<12);
		res = W25Qx_Write((uint8_t *)buff, sector<<12, count<<12);
		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT status = RES_PARERR;
	HAL_SD_CardInfoTypeDef CardInfo;
	switch (pdrv) {
	case DEV_SD_CARD :
		HAL_SD_GetCardInfo(&hsd1, &CardInfo);
		// Process of the command for the RAM drive
        switch(cmd) {
			case CTRL_SYNC:
				status = RES_OK;
				break;
            case GET_SECTOR_COUNT:
                *(DWORD *) buff = CardInfo.LogBlockNbr;
                status = RES_OK;break;
            case GET_SECTOR_SIZE :
                *(WORD *) buff = CardInfo.LogBlockSize;
                status = RES_OK;break;
            case GET_BLOCK_SIZE  :
                *(DWORD *) buff = CardInfo.LogBlockSize / 512;
				status = RES_OK;break;
		}
	case DEV_FLASH :

		// Process of the command for the MMC/SD card
        switch(cmd) {
            case GET_SECTOR_COUNT:
                *(DWORD *) buff = 4096 - SECTOR_OFFSET;
                break;
            case GET_SECTOR_SIZE :
                *(WORD *) buff = 4096;
                break;
            case GET_BLOCK_SIZE  :
                *(DWORD *) buff = 1;
                break;
        }
        status = RES_OK;break;
        default:status = RES_PARERR;
	}

	return status;
}

DWORD get_fattime(void)
{
    return 0;
}
