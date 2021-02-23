#include "decode_dma.h"
#include <stdio.h>


#define CHUNK_SIZE_IN  ((uint32_t)(400))   /* 输入数据大小，单位字节 */
#define CHUNK_SIZE_OUT ((uint32_t)(400))   /* 输出数据大小，单位字节 */

__IO uint32_t Jpeg_HWDecodingEnd = 0; /* 解码完成标志，0表示开始解码，1表示解码完成 */


uint32_t FrameBufferAddress;          /* JPEG解码后数据地址 */
uint32_t JPEGSourceAddress;           /* JPEG数据源地址 */
uint32_t Input_frameSize;             /* JPEG源数据大小 */
uint32_t Input_frameIndex;            /* JPEG解码过程中，已经解码的数据大小 */


extern JPEG_HandleTypeDef hjpeg;
JPEG_ConfTypeDef      JPEG_Info;

uint8_t image1_decode[12800];
/*
*********************************************************************************************************
*	函 数 名: JPEG_Decode_DMA
*	功能说明: JPEG解码
*	形    参: hjpeg               JPEG_HandleTypeDef句柄指针
*             FrameSourceAddress  数据地址
*             FrameSize           数据大小
*             DestAddress         目的数据地址
*	返 回 值: HAL_ERROR表示配置失败，HAL_OK表示配置成功
*             HAL_BUSY表示忙（操作中），HAL_TIMEOUT表示时间溢出
*********************************************************************************************************
*/
//JPEG_Decode_DMA(&JPEG_Handle, (uint32_t)_ac1, sizeof(_ac1) , SDRAM_APP_BUF);
uint32_t JPEG_Decode_DMA(uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress)
{
	JPEGSourceAddress =  FrameSourceAddress ;
	FrameBufferAddress = DestAddress;
	Input_frameIndex = 0;
	Input_frameSize = FrameSize;

	/* 设置标志，0表示开始解码，1表示解码完成 */
	Jpeg_HWDecodingEnd = 0;

	/* 启动JPEG解码 */
	HAL_JPEG_Decode_DMA(&hjpeg ,(uint8_t *)JPEGSourceAddress ,CHUNK_SIZE_IN ,(uint8_t *)FrameBufferAddress ,CHUNK_SIZE_OUT);

	return HAL_OK;
}

/*
*********************************************************************************************************
*	函 数 名: HAL_JPEG_GetDataCallback
*	功能说明: JPEG回调函数，用于从输入地址获取新数据继续解码
*	形    参: hjpeg          JPEG_HandleTypeDef 句柄指针
*             NbDecodedData  上一轮已经解码的数据大小，单位字节  
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
	uint32_t inDataLength; 
	
	/* 更新已经解码的数据大小 */
	Input_frameIndex += NbDecodedData;
	
	/* 如果当前已经解码的数据小于总文件大小，继续解码 */
	if( Input_frameIndex < Input_frameSize)
	{
		/* 更新解码数据位置 */
		JPEGSourceAddress = JPEGSourceAddress + NbDecodedData;

		/* 更新下一轮要解码的数据大小 */
		if((Input_frameSize - Input_frameIndex) >= CHUNK_SIZE_IN)
		{
			inDataLength = CHUNK_SIZE_IN;
		}
		else
		{
			inDataLength = Input_frameSize - Input_frameIndex;
		}    
	}
	else
	{
		inDataLength = 0; 
	}
	
	/* 更新输入缓冲 */
	HAL_JPEG_ConfigInputBuffer(hjpeg,(uint8_t *)JPEGSourceAddress, inDataLength);    
}
/*
*********************************************************************************************************
*	函 数 名: HAL_JPEG_DataReadyCallback
*	功能说明: JPEG回调函数，用于输出缓冲地址更新
*	形    参: hjpeg         JPEG_HandleTypeDef 句柄指针
*             pDataOut      输出数据缓冲
*             OutDataLength 输出数据大小，单位字节
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
	/* 更新JPEG输出地址 */  
	FrameBufferAddress += OutDataLength;

	HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)FrameBufferAddress, CHUNK_SIZE_OUT); 
}

/*
*********************************************************************************************************
*	函 数 名: HAL_JPEG_ErrorCallback
*	功能说明: JPEG错误回调
*	形    参: hjpeg    JPEG_HandleTypeDef 句柄指针
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	//Error_Handler(__FILE__, __LINE__);
}

/*
*********************************************************************************************************
*	函 数 名: HAL_JPEG_DecodeCpltCallback
*	功能说明: JPEG解码完成中断
*	形    参: hjpeg    JPEG_HandleTypeDef 句柄指针
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
	Jpeg_HWDecodingEnd = 1; 
}
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
	printf("ChromaSubsampling:%d\r\n",pInfo->ChromaSubsampling);
	printf("ColorSpace:%d\r\n",pInfo->ColorSpace);
	printf("ImageHeight:%d\r\n",pInfo->ImageHeight);
	printf("ImageQuality:%d\r\n",pInfo->ImageQuality);
	printf("ImageWidth:%d\r\n",pInfo->ImageWidth);
}
