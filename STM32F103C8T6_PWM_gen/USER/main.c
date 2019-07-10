/*-------------------------------------------------------------------------------
文件名称：main.c
文件描述：控制LED2，LED3闪烁
硬件平台：尼莫M3S开发板
编写整理：shifang
固件库  ：V3.5
技术论坛：www.doflye.net
备    注：通过简单修改可以移植到其他开发板，部分资料来源于网络。
---------------------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
//#include "intrins.h"
#include "led.h"
#include "lcd.h"
//#include "chinese_code.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "adc.h"
#include "stdint.h"
//#include "oled.h"
//#include "bmp.h"
#include "usart.h"
#include "spi.h"	
#include "display.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "My_InitTask.h" 
/*
sbit lcd_cs1  =  P3^4;//CS
sbit lcd_reset=  P3^5;//RST
sbit lcd_sclk =  P1^0;//串行时钟
sbit lcd_rs   =  P3^3;//RS	  
sbit lcd_sid  =  P1^1;//串行数据 
*/
	
/*
LCD Port: PD0-8
PWM Port: PD12 PC6 PA0/1/8 PB5
ADC Port: PA2-7,PB0,1;PC0,1,2,3
*/
// ADC1转换的电压值通过MDA方式传到SRAM
// PA2 -7,PB0,1;PC0,1,2,3

// 局部变量，用于保存转换计算后的电压值	

//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              CLK   接PA5（SCL）-->PB2
//              MOSI   接PA7（SDA）-->PB3
//              RES  接PB0-->PB4
//              DC   接PB1--> PB5
//							BLK  接A4 可以悬空-->PB6
//							MISO 可以不接
//              ---------------------------------------------------------------- 
 
/******************USART0 PA9/PA10****/

u8 txbuf[16];

/*for dispaly 1.8TFT below*/

u8 state=0;
void beepms(u16 va);
void xianshi(void);//显示信息  
void refshow(void);//刷新显示

/*RTOS*/

void Task1 (void *data);

void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色 
}

void xianshi()//显示信息
{ 
	u8 index_Show;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;  	
	LCD_ShowString(4,0,200,12,12,"CH FREQ DUTY");
for(index_Show=0;index_Show<9;index_Show++)	
	{
		LCD_ShowString(8,(index_Show+1)*12,200,12,12,"CH1 Freq: 1024 Duty: 100%");  

	}
}

void refshow(void)	 //刷新显示
{
	switch(state)
	{
		case 0:
		LCD_Clear(WHITE);
	    xianshi();
		break;
		case 1:
		LCD_Clear(BLACK);	
		break;
		case 2:
		LCD_Clear(RED);
		break;
		case 3:
		LCD_Clear(GREEN);
		break;
		case 4:
		LCD_Clear(BLUE);
		break;
	}	
}



uint16_t Get_Adc_Filter(uint16_t value,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=value;
		//delay(5);
	}
	return temp_val/times;
} 	


void LED_Init_G14(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			 //板上LED编号 D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);

}


//-----------------------------
int main()
{		
	bool LED_State_01,Usart_Config_State;
	int delayI,cnt=0,channel_i;
	uint16_t ADC_ConvertedValueLocal,ADC_ConvertedValueLocal2; 
unsigned int adcx_Freq_Raw[8],adcx_DutyCycle_Raw[8],adcx_Freq_Old[8],adcx_DutyCycle_Old[8];
	uint8_t adcx_Freq_Changed[8],adcx_DutyCycle_Changed[8];
	uint16_t dutycycle;
	uint8_t ADC_index_i, Send_Buf_index;
		u16 i=0;	 
	u8 key=0;
	
	LED_Init_G14();
  LED_Init();	
	
			NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
		uart_init(115200);	 //串口初始化为9600
	/* */
		My_InitTask();

		TIM1_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM1,800);         //设置占空比为1/3
	 
	 		TIM2_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM2,800);         //设置占空比为1/3
		
			TIM3_PWM_Init(6009,1000);	 //不分频。PWM频率=72000/900=8Khz
		TIM_SetCompare4(TIM3,800);	
	 
	 	 		TIM4_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM4,1800);          //设置占空比为1/3
	 
	 	 
	 //	 TIM5_Int_Init(6009,100);	
		//TIM_SetCompare2(TIM5,800);         //设置占空比为1/3
	 
	 	// 	 		TIM8_Int_Init(6009,100);	
		//TIM_SetCompare1(TIM8,800);         //设置占空比为1/3
	
	//ADC1_Init();
	Adc_Init();
	
	if(0)
	{
		//initial_lcd(); 									//对液晶模块进行初始化设置
		delay(500);
		//clear_screen();	
		//display_string_16x16(32,4,"豹宏实业有限公司");
			//	display_string_16x16(32,6,"豹宏实业有限公司");
				/**/
		delay(50000);
		delay(50000);
		//clear_screen();	
	}
	
	
		delay(500);
	SPI1_Init();	//SPI1初始化
	LCD_Init();	  			
 	POINT_COLOR=RED;//设置字体为红色 
	xianshi();	   //显示信息

	
	//xTaskCreate( Task1, ( signed portCHAR *) "Task1", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);
	//xTaskCreate( Task2, ( signed portCHAR *) "Task2", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);
	//xTaskCreate( Task3, ( signed portCHAR *) "Task3", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY), 	NULL);
   //	xTaskCreate( Task4, ( signed portCHAR *) "Task4", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY), NULL);
	/*xTaskCreate( Task5, ( signed portCHAR *) "Task5", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);
	xTaskCreate( Task6, ( signed portCHAR *) "Task6", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY+1), NULL);*/
	

	/* Start the scheduler. */
	//vTaskStartScheduler();
	
	Usart_Config_State=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
	
	while(1)
	{
	
	delay(500);
			
		if(Usart_Config_State==FALSE)
		{
			if(Usart1_Receive_Complete==TRUE)
			{
				Usart1_Send(DMA_Rece_Buf,Usart1_Rec_Cnt);
				Usart1_Receive_Complete=FALSE;
			}
		}
		
for(ADC_index_i=0;ADC_index_i<4;ADC_index_i++)
		{
			adcx_Freq_Raw[ADC_index_i] = Get_Adc_Average(ADC_index_i+1,100); // 读取转换的AD值
			adcx_DutyCycle_Raw[ADC_index_i] = Get_Adc_Average(ADC_index_i+5,100); // 读取转换的AD值
		}
		
//		adcx_Freq_Raw[0] = Get_Adc_Average(ADC_Channel_1,100); // 读取转换的AD值
//		adcx_Freq_Raw[1] = Get_Adc_Average(ADC_Channel_2,100); // 读取转换的AD值
//		adcx_Freq_Raw[2] = Get_Adc_Average(ADC_Channel_3,100); // 读取转换的AD值
//		adcx_Freq_Raw[3] = Get_Adc_Average(ADC_Channel_4,100); // 读取转换的AD值
//		
//		adcx_DutyCycle_Raw[0] = Get_Adc_Average(ADC_Channel_5,100); // 读取转换的AD值
//		adcx_DutyCycle_Raw[1] = Get_Adc_Average(ADC_Channel_6,100); // 读取转换的AD值
//		adcx_DutyCycle_Raw[2] = Get_Adc_Average(ADC_Channel_7,100); // 读取转换的AD值
//		adcx_DutyCycle_Raw[3] = Get_Adc_Average(ADC_Channel_8,100); // 读取转换的AD值
		
	//	adcx_Freq_Raw[4] = Get_Adc_Average(ADC_Channel_5,100); // 读取转换的AD值
	//	adcx_Freq_Raw[5] = Get_Adc_Average(ADC_Channel_6,100); // 读取转换的AD值
	//	adcx_Freq_Raw[6] = Get_Adc_Average(ADC_Channel_7,100); // 读取转换的AD值
		//adcx_Freq_Raw[6] = Get_Adc_Average(ADC_Channel_0,100); // 读取转换的AD值
		//adcx_Freq_Raw[7] = Get_Adc_Average(ADC_Channel_1,100); // 读取转换的AD值
		
		

		
		
		
//		adcx_DutyCycle_Raw[4] = Get_Adc_Average(ADC_Channel_12,100); // 读取转换的AD值
//		adcx_DutyCycle_Raw[5] = Get_Adc_Average(ADC_Channel_13,100); // 读取转换的AD值
//		adcx_DutyCycle_Raw[6] = Get_Adc_Average(ADC_Channel_14,100); // 读取转换的AD值
//		adcx_DutyCycle_Raw[7] = Get_Adc_Average(ADC_Channel_15,100); // 读取转换的AD值
//		
		
		for(ADC_index_i=0;ADC_index_i<4;ADC_index_i++)
		{
		//adcx[ADC_index_i] = Get_Adc_Average(ADC_index_i+1,100); // 读取转换的AD值
			adcx_Freq[ADC_index_i]=Get_Adc_Filter(adcx_Freq_Raw[ADC_index_i],100);
			adcx_DutyCycle[ADC_index_i]=Get_Adc_Filter(adcx_DutyCycle_Raw[ADC_index_i],100);
			
			if(adcx_Freq[ADC_index_i]!=adcx_Freq_Old[ADC_index_i])
			{
			adcx_Freq_Changed[ADC_index_i]=1;
				if(ADC_index_i==2)
				{
				//	TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);//TIM3中断使能
				}
			}
			else
			{
			adcx_Freq_Changed[ADC_index_i]=0;
			}
			
			if(adcx_DutyCycle[ADC_index_i]!=adcx_DutyCycle_Old[ADC_index_i])
			{
			adcx_DutyCycle_Changed[ADC_index_i]=1;
			}
			else
			{
			adcx_DutyCycle_Changed[ADC_index_i]=0;
			}

			
			adcx_Freq_Old[ADC_index_i] =adcx_Freq[ADC_index_i];
			adcx_DutyCycle_Old[ADC_index_i] =adcx_DutyCycle[ADC_index_i];
		}
		
		if(Usart_Config_State==TRUE)
		{
			
				for(channel_i=0;channel_i<4;channel_i++)
				{
					//Send_Buf_index=channel_i/4;
					dutycycle=(adcx_DutyCycle[channel_i]*100)/4095;
					
						txbuf[0+channel_i*4]=channel_i;					
						txbuf[1+channel_i*4]=dutycycle;
						txbuf[2+channel_i*4]=adcx_Freq[channel_i]>>8;
						txbuf[3+channel_i*4]=adcx_Freq[channel_i]&0xFF;
					
					
					delay(500);
				}
			Usart1_Send(txbuf,16);
		}
			
	
		
		ADC_ConvertedValueLocal2=adcx_Freq[0];
		//display_PWM_Channel(1,1);
		
		
		
		for(channel_i=0;channel_i<4;channel_i++)
		{
			if((adcx_Freq_Changed[channel_i]==1) || (adcx_DutyCycle_Changed[channel_i]==1))
			{
			//display_PWM_Channel(channel_i+1,channel_i);
//			display_PWM_Freq(channel_i+1,channel_i*2,adcx_Freq[channel_i]);
//			display_PWM_DutyCycle(channel_i+1,channel_i*2,adcx_DutyCycle[channel_i]/100);
				
				
	
			PWM_Freq_DC(channel_i,adcx_DutyCycle[channel_i],adcx_Freq[channel_i]);
				
				
	
				
						
				/*1.3 OLED display*/
				//display_Ch_Fre_Duty(channel_i+1,adcx_Freq[channel_i],adcx_DutyCycle[channel_i]);
			}
		}
		
	
		
		//disp_16x8x4_Char(32,1,jing2[]);
		for(delayI=0;delayI<100;delayI++)
		{
			delay(200); 
		}
			cnt=cnt+1;
		if(cnt>=8)
		{
		cnt = 0;
			//clear_screen();	
		}
		//while(1);	
// 		waitkey();
	
	}
}

void Task1 (void *data)
{
	data = data;

	while (1) 
	{
		 
   PCout(13)=!PCout(13);            //  点亮LED 
		vTaskDelay( 1000 / portTICK_RATE_MS ); 
	
		

	}
}


/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
