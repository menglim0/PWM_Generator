#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define DMA_Rec_Len 200

extern bool Usart1_Receive_Complete;
extern u8  DMA_Rece_Buf[DMA_Rec_Len];
extern u16  Usart1_Rec_Cnt;
void uart_init(u32 bound);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void Usart1_Send(u8 *buf,u8 len);

#endif


