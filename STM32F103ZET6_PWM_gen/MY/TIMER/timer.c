#include "timer.h"
#include "stm32f10x.h"
#include "led.h"
#include "stm32f10x_tim.h"
//#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

uint16_t adcx_DutyCycle[6],adcx_Freq[6];
uint8_t channel_i;

void TIM2_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);    //disable JTAG
 //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); 
    //��ʼ��TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*��һ����ü���*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
   TIM_CtrlPWMOutputs(TIM2, ENABLE);/*��һ������ֻ���timer1��timer8*/
    TIM_Cmd(TIM2, ENABLE); 


}

void TIM4_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3������ӳ��  TIM4_CH1->PD12
	
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
 
    //��ʼ��TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*��һ����ü���*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
   //TIM_CtrlPWMOutputs(TIM4, ENABLE);/*��һ������ֻ���timer1��timer8*/
    TIM_Cmd(TIM4, ENABLE); 
}

void TIM5_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
// 
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);
 
    //��ʼ��TIM3
//	TIM_TimeBaseInitStruct.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

//    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
// 
//    TIM_OCStructInit(&TIM_OCInitStruct);/*��һ����ü���*/
//    
//    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStruct.TIM_Pulse = 100;
//    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC2Init(TIM5, &TIM_OCInitStruct);
// 
//    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
//    TIM_ARRPreloadConfig(TIM5, ENABLE);
//   //TIM_CtrlPWMOutputs(TIM4, ENABLE);/*��һ������ֻ���timer1��timer8*/
//    TIM_Cmd(TIM5, ENABLE); 
}

void TIM8_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM8, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_GPIOC, ENABLE);
 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
 
    //��ʼ��TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*��һ����ü���*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM8, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM8, ENABLE);
   TIM_CtrlPWMOutputs(TIM8, ENABLE);/*��һ������ֻ���timer1��timer8*/
    TIM_Cmd(TIM8, ENABLE); 
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		LED1=!LED1;
		//	PWM_Freq_DC(2,adcx_DutyCycle[2],adcx_Freq[2]);
			TIM_ITConfig(TIM3,TIM_IT_Update ,DISABLE);//TIM3�ж�ʹ��
		}
}




//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
  //TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);//TIM3�ж�ʹ��
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}

void TIM1_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);
 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
 
    //��ʼ��TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*��һ����ü���*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_OCInitStruct.TIM_Pulse = 36000;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);/*��һ������ֻ���timer1��timer8*/
    TIM_Cmd(TIM1, ENABLE); 


}

void PWM_Freq_DC(uint8_t ch,uint16_t dutycycle, uint16_t freq)
{
	uint16_t arr_peroid,compare_dutycycle,Var_psc=0,i;
	
	uint32_t arr_peroid_long;
	
	arr_peroid_long = 72000000/(freq);
	
for(i=0;i<100;i++)
	{
		if(arr_peroid_long>65535)
		{
			Var_psc++;
			
		}
		else
		{break;}
	}
	
	arr_peroid_long	=arr_peroid_long/	(Var_psc+1);
	arr_peroid = arr_peroid_long;	
	compare_dutycycle = (arr_peroid*(dutycycle/100))/100;
	
	switch (ch)
	{	 	 	 		
		case 0:
		 TIM1->ARR = arr_peroid-1;
		TIM1->PSC =Var_psc;
		TIM1->CCR2 = compare_dutycycle;
		break;
		
		
		case 1:

				 TIM2->ARR = arr_peroid-1;
		TIM2->PSC =Var_psc;
		TIM2->CCR2 = compare_dutycycle;
		break;
				
		case 2:
		//TIM3_PWM_Init(arr_peroid,Var_psc);	 //����Ƶ��PWMƵ��=72000/900=8Khz
		  /* Set the Prescaler value */
		 TIM3->ARR = arr_peroid-1;
		TIM3->PSC =Var_psc;
		TIM3->CCR2 = compare_dutycycle;
		//TIM_SetCompare2(TIM3,compare_dutycycle);	
		break;
						
		case 3:

		TIM4->ARR = arr_peroid-1;
		TIM4->PSC =Var_psc;
		TIM4->CCR2 = compare_dutycycle;
		break;
								
		case 4:
		 TIM5->ARR = arr_peroid-1;
		TIM5->PSC =Var_psc;
		TIM5->CCR2 = compare_dutycycle;
		break;
										
		case 5:
		 TIM8->ARR = arr_peroid-1;
		TIM8->PSC =Var_psc;
		TIM8->CCR2 = compare_dutycycle;
		break;
		
		default:
		break;
		
												
	}

}