#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"
void ADC1_Init(void);

void Adc_Init(void);
uint16_t Get_Adc(uint8_t ch);   
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times); 
#endif /* __ADC_H */
/*----------------------�·��� ������̳��www.doflye.net--------------------------*/
