#include "decode_dma.h"
#include <stdio.h>


#define CHUNK_SIZE_IN  ((uint32_t)(400))   /* �������ݴ�С����λ�ֽ� */
#define CHUNK_SIZE_OUT ((uint32_t)(400))   /* ������ݴ�С����λ�ֽ� */

__IO uint32_t Jpeg_HWDecodingEnd = 0; /* ������ɱ�־��0��ʾ��ʼ���룬1��ʾ������� */


uint32_t FrameBufferAddress;          /* JPEG��������ݵ�ַ */
uint32_t JPEGSourceAddress;           /* JPEG����Դ��ַ */
uint32_t Input_frameSize;             /* JPEGԴ���ݴ�С */
uint32_t Input_frameIndex;            /* JPEG��������У��Ѿ���������ݴ�С */


extern JPEG_HandleTypeDef hjpeg;
JPEG_ConfTypeDef      JPEG_Info;

uint8_t image1_decode[12800];
/*
*********************************************************************************************************
*	�� �� ��: JPEG_Decode_DMA
*	����˵��: JPEG����
*	��    ��: hjpeg               JPEG_HandleTypeDef���ָ��
*             FrameSourceAddress  ���ݵ�ַ
*             FrameSize           ���ݴ�С
*             DestAddress         Ŀ�����ݵ�ַ
*	�� �� ֵ: HAL_ERROR��ʾ����ʧ�ܣ�HAL_OK��ʾ���óɹ�
*             HAL_BUSY��ʾæ�������У���HAL_TIMEOUT��ʾʱ�����
*********************************************************************************************************
*/
//JPEG_Decode_DMA(&JPEG_Handle, (uint32_t)_ac1, sizeof(_ac1) , SDRAM_APP_BUF);
uint32_t JPEG_Decode_DMA(uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress)
{
	JPEGSourceAddress =  FrameSourceAddress ;
	FrameBufferAddress = DestAddress;
	Input_frameIndex = 0;
	Input_frameSize = FrameSize;

	/* ���ñ�־��0��ʾ��ʼ���룬1��ʾ������� */
	Jpeg_HWDecodingEnd = 0;

	/* ����JPEG���� */
	HAL_JPEG_Decode_DMA(&hjpeg ,(uint8_t *)JPEGSourceAddress ,CHUNK_SIZE_IN ,(uint8_t *)FrameBufferAddress ,CHUNK_SIZE_OUT);

	return HAL_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_GetDataCallback
*	����˵��: JPEG�ص����������ڴ������ַ��ȡ�����ݼ�������
*	��    ��: hjpeg          JPEG_HandleTypeDef ���ָ��
*             NbDecodedData  ��һ���Ѿ���������ݴ�С����λ�ֽ�  
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
	uint32_t inDataLength; 
	
	/* �����Ѿ���������ݴ�С */
	Input_frameIndex += NbDecodedData;
	
	/* �����ǰ�Ѿ����������С�����ļ���С���������� */
	if( Input_frameIndex < Input_frameSize)
	{
		/* ���½�������λ�� */
		JPEGSourceAddress = JPEGSourceAddress + NbDecodedData;

		/* ������һ��Ҫ��������ݴ�С */
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
	
	/* �������뻺�� */
	HAL_JPEG_ConfigInputBuffer(hjpeg,(uint8_t *)JPEGSourceAddress, inDataLength);    
}
/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_DataReadyCallback
*	����˵��: JPEG�ص�������������������ַ����
*	��    ��: hjpeg         JPEG_HandleTypeDef ���ָ��
*             pDataOut      ������ݻ���
*             OutDataLength ������ݴ�С����λ�ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
	/* ����JPEG�����ַ */  
	FrameBufferAddress += OutDataLength;

	HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)FrameBufferAddress, CHUNK_SIZE_OUT); 
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_ErrorCallback
*	����˵��: JPEG����ص�
*	��    ��: hjpeg    JPEG_HandleTypeDef ���ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	//Error_Handler(__FILE__, __LINE__);
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_DecodeCpltCallback
*	����˵��: JPEG��������ж�
*	��    ��: hjpeg    JPEG_HandleTypeDef ���ָ��
*	�� �� ֵ: ��
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
