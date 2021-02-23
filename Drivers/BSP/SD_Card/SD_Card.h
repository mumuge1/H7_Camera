#ifndef _SD_CARD_H
#define _SD_CARD_H
#include "main.h"

#define SD_TIMEOUT      	((uint32_t)100000000)
#define SD_TRANSFER_OK     	((uint8_t)0x00)
#define SD_TRANSFER_BUSY   	((uint8_t)0x01)
void Get_SD_Info(void);


uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt);
uint8_t SD_GetCardState(void);
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt);

#endif/*_SD_CARD_H*/

