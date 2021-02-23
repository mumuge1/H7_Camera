#ifndef _DECODE_DMA_H
#define _DECODE_DMA_H
#include "main.h"
uint32_t JPEG_Decode_DMA(uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress);
extern __IO uint32_t Jpeg_HWDecodingEnd;
extern uint8_t image1_decode[160*80];
#endif/*_DECODE_DMA_H*/
