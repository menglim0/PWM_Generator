#ifndef MY_INITTASK_H
#define MY_INITTASK_H


#include "stm32f10x.h"
	
void My_InitTask(void); 
   
void RCC_Configuration(void);
//void NVIC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);

/* Private function prototypes -----------------------------------------------*/
void ADC_Configuration(void);
//void RCC_Configuration(void);
//void GPIO_Configuration(void);
//void NVIC_Configuration(void);
void DMA_Configuration(void);

void AD_ConvertFunction(void);





#endif /* MY_INITTASK_H */
















