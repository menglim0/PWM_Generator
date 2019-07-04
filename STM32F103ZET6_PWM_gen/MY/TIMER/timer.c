#include "timer.h"
#include "stm32f10x.h"
#include "led.h"
#include "stm32f10x_tim.h"
//#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

uint16_t adcx_DutyCycle[6],adcx_Freq[6];
uint8_t channel_i;

void TIM2_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);    //disable JTAG
 //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); 
    //初始化TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*这一步最好加上*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
   TIM_CtrlPWMOutputs(TIM2, ENABLE);/*这一个函数只针对timer1和timer8*/
    TIM_Cmd(TIM2, ENABLE); 


}

void TIM4_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3部分重映射  TIM4_CH1->PD12
	
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
 
    //初始化TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*这一步最好加上*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
   //TIM_CtrlPWMOutputs(TIM4, ENABLE);/*这一个函数只针对timer1和timer8*/
    TIM_Cmd(TIM4, ENABLE); 
}

void TIM5_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能定时器3时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
// 
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);
 
    //初始化TIM3
//	TIM_TimeBaseInitStruct.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

//    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
// 
//    TIM_OCStructInit(&TIM_OCInitStruct);/*这一步最好加上*/
//    
//    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStruct.TIM_Pulse = 100;
//    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC2Init(TIM5, &TIM_OCInitStruct);
// 
//    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
//    TIM_ARRPreloadConfig(TIM5, ENABLE);
//   //TIM_CtrlPWMOutputs(TIM4, ENABLE);/*这一个函数只针对timer1和timer8*/
//    TIM_Cmd(TIM5, ENABLE); 
}

void TIM8_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM8, ENABLE);	//使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_GPIOC, ENABLE);
 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
 
    //初始化TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*这一步最好加上*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM8, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM8, ENABLE);
   TIM_CtrlPWMOutputs(TIM8, ENABLE);/*这一个函数只针对timer1和timer8*/
    TIM_Cmd(TIM8, ENABLE); 
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		LED1=!LED1;
		//	PWM_Freq_DC(2,adcx_DutyCycle[2],adcx_Freq[2]);
			TIM_ITConfig(TIM3,TIM_IT_Update ,DISABLE);//TIM3中断使能
		}
}




//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
  //TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);//TIM3中断使能
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

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
 
    //初始化TIM3
	TIM_TimeBaseInitStruct.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
 
    TIM_OCStructInit(&TIM_OCInitStruct);/*这一步最好加上*/
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_OCInitStruct.TIM_Pulse = 36000;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);
 
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);/*这一个函数只针对timer1和timer8*/
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
		//TIM3_PWM_Init(arr_peroid,Var_psc);	 //不分频。PWM频率=72000/900=8Khz
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