#include "SD_Card.h"
#include "main.h"
#include <stdio.h>
extern SD_HandleTypeDef hsd1;

void Get_SD_Info(void)
{
	HAL_SD_CardInfoTypeDef CardInfo1;
	if(HAL_SD_GetCardInfo(&hsd1, &CardInfo1) == HAL_OK)
	{
		printf("CardType:%d\r\n",	 CardInfo1.CardType);
		printf("CardVersion:%d\r\n", CardInfo1.CardVersion);
		printf("Class:%d\r\n",   	 CardInfo1.Class);
		printf("RelCardAdd:%d\r\n",  CardInfo1.RelCardAdd);
		printf("BlockNbr:%d\r\n",    CardInfo1.BlockNbr);
		printf("BlockSize:%d\r\n",   CardInfo1.BlockSize);
		printf("LogBlockNbr:%d\r\n", CardInfo1.LogBlockNbr);
		printf("LogBlockSize:%d\r\n",CardInfo1.LogBlockSize);
		printf("CardSpeed:%d\r\n",   CardInfo1.CardSpeed);
	}
}
uint8_t SD_GetCardState(void)
{
	//printf("state:%d",HAL_SD_GetCardState(&hsd1));
  return((HAL_SD_GetCardState(&hsd1)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}
uint8_t SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    __disable_irq();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_ReadBlocks(&hsd1, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的读操作
	
	//等待SD卡读完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }
    __enable_irq();//开启总中断
    return sta;	
}
uint8_t SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    __disable_irq();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_WriteBlocks(&hsd1,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的写操作
		
	//等待SD卡写完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }    
	__enable_irq();//开启总中断
    return sta;
}
