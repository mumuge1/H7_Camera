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
//#include "main.h"
#include "SD_Card.h"
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
#include <stdio.h>
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	switch (pdrv) {
	case DEV_SD_CARD :
		stat = SD_GetCardState();
		return stat;
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
		res=SD_ReadDisk(buff,sector,count);
		while(res)//读出错
			{
				MX_SDMMC1_SD_Init();	//重新初始化SD卡
				res=SD_ReadDisk(buff,sector,count);	
			}
			break;

	case DEV_FLASH :
		// translate the arguments here
        sector+=SECTOR_OFFSET;
		for(;count>0;count--)
			{
				res+=W25Qx_Read(buff,sector<<12,4096);
				sector++;
				buff+=512;
			}

		// translate the reslut code here

		return RES_OK;
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
		res=SD_WriteDisk((uint8_t*)buff,sector,count);
			while(res)//写出错
			{
				MX_SDMMC1_SD_Init();	//重新初始化SD卡
				res=SD_WriteDisk((uint8_t*)buff,sector,count);	
			}
			break;

	case DEV_FLASH :
		for(;count>0;count--)
			{										    
				W25QXX_Write((uint8_t*)buff,sector<<12,4096);
				sector++;
				buff+=512;
			}
			res=0;
		
		return res;
	}

	return res;
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
                *(WORD *) buff = 512;
                status = RES_OK;break;
            case GET_BLOCK_SIZE  :
                *(DWORD *) buff = CardInfo.LogBlockSize;
				status = RES_OK;break;
		}
	case DEV_FLASH :

		// Process of the command for the MMC/SD card
        switch(cmd) {
            case GET_SECTOR_COUNT:
                *(DWORD *) buff = 2048;
                break;
            case GET_SECTOR_SIZE :
                *(WORD *) buff = 4096;
                break;
            case GET_BLOCK_SIZE  :
                *(DWORD *) buff = 8;
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
