/*-------------------------------------------------------------------------------
文件名称：main.c
文件描述：控制LED2，LED3闪烁
硬件平台：尼莫M3S开发板
编写整理：shifang
固件库  ：V3.5
技术论坛：www.doflye.net
备    注：通过简单修改可以移植到其他开发板，部分资料来源于网络。
---------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
//#include "intrins.h"
#include "led.h"
#include "lcd.h"
#include "chinese_code.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "adc.h"
#include "stdint.h"
#include "oled.h"
#include "bmp.h"

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

//-----------------------------
int main()
{		
	
	int delayI,cnt=0,channel_i;
	uint16_t ADC_ConvertedValueLocal,ADC_ConvertedValueLocal2; 
unsigned int adcx_Freq_Raw[6],adcx_DutyCycle_Raw[6],adcx_Freq_Old[6],adcx_DutyCycle_Old[6];
	uint8_t adcx_Freq_Changed[6],adcx_DutyCycle_Changed[6];
	
	uint8_t ADC_index_i;
  LED_Init();	
	
			NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	/* */
		TIM3_PWM_Init(8999,100);	 //不分频。PWM频率=72000/900=8Khz
		TIM_SetCompare2(TIM3,800);	

		TIM1_Int_Init(7009,100);	
		TIM_SetCompare1(TIM1,800);         //设置占空比为1/3
	 
	 		TIM2_Int_Init(6009,100);	
		TIM_SetCompare1(TIM2,800);         //设置占空比为1/3
	 
	 	 		TIM4_Int_Init(6009,100);	
		TIM_SetCompare1(TIM4,800);          //设置占空比为1/3
	 
	 	 
	 	 		TIM5_Int_Init(6009,100);	
		TIM_SetCompare2(TIM5,800);         //设置占空比为1/3
	 
	 	 	 		TIM8_Int_Init(6009,100);	
		TIM_SetCompare1(TIM8,800);         //设置占空比为1/3
	
	//ADC1_Init();
	Adc_Init();
	initial_lcd(); 									//对液晶模块进行初始化设置
	delay(500);
	clear_screen();	
	display_string_16x16(32,4,"豹宏实业有限公司");
		//	display_string_16x16(32,6,"豹宏实业有限公司");
			/**/
	delay(50000);
	delay(50000);


	clear_screen();	 
		
	while(1)
	{
	


		/**/
		adcx_Freq_Raw[0] = Get_Adc_Average(ADC_Channel_2,100); // 读取转换的AD值
		adcx_Freq_Raw[1] = Get_Adc_Average(ADC_Channel_3,100); // 读取转换的AD值
		adcx_Freq_Raw[2] = Get_Adc_Average(ADC_Channel_4,100); // 读取转换的AD值
		adcx_Freq_Raw[3] = Get_Adc_Average(ADC_Channel_5,100); // 读取转换的AD值
		adcx_Freq_Raw[4] = Get_Adc_Average(ADC_Channel_6,100); // 读取转换的AD值
		adcx_Freq_Raw[5] = Get_Adc_Average(ADC_Channel_7,100); // 读取转换的AD值
		adcx_DutyCycle_Raw[0] = Get_Adc_Average(ADC_Channel_8,100); // 读取转换的AD值
		adcx_DutyCycle_Raw[1] = Get_Adc_Average(ADC_Channel_9,100); // 读取转换的AD值
		adcx_DutyCycle_Raw[2] = Get_Adc_Average(ADC_Channel_10,100); // 读取转换的AD值
		adcx_DutyCycle_Raw[3] = Get_Adc_Average(ADC_Channel_11,100); // 读取转换的AD值
		adcx_DutyCycle_Raw[4] = Get_Adc_Average(ADC_Channel_12,100); // 读取转换的AD值
		adcx_DutyCycle_Raw[5] = Get_Adc_Average(ADC_Channel_13,100); // 读取转换的AD值
		
		for(ADC_index_i=0;ADC_index_i<6;ADC_index_i++)
		{
		//adcx[ADC_index_i] = Get_Adc_Average(ADC_index_i+1,100); // 读取转换的AD值
			adcx_Freq[ADC_index_i]=Get_Adc_Filter(adcx_Freq_Raw[ADC_index_i],100);
			adcx_DutyCycle[ADC_index_i]=Get_Adc_Filter(adcx_DutyCycle_Raw[ADC_index_i],100);
			
			if(adcx_Freq[ADC_index_i]!=adcx_Freq_Old[ADC_index_i])
			{
			adcx_Freq_Changed[ADC_index_i]=1;
				if(ADC_index_i==2)
				{
					TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);//TIM3中断使能
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
	
		
		
		ADC_ConvertedValueLocal2=adcx_Freq[0];
		//display_PWM_Channel(1,1);
		
		for(channel_i=0;channel_i<6;channel_i++)
		{
			if((adcx_Freq_Changed[channel_i]==1) || (adcx_DutyCycle_Changed[channel_i]==1))
			{
			display_PWM_Channel(channel_i+1,channel_i);
			display_PWM_Freq(channel_i+1,channel_i*2,adcx_Freq[channel_i]);
			display_PWM_DutyCycle(channel_i+1,channel_i*2,adcx_DutyCycle[channel_i]/100);

			PWM_Freq_DC(channel_i,adcx_DutyCycle[channel_i],adcx_Freq[channel_i]);
			}
		}
		
	//	PWM_Freq_DC(3,adcx_DutyCycle[0],adcx_Freq[0]);
		//test(1,1)

		/*
		disp_16x8x4_Char(1,3,PWM1);
		disp_16x8x1_Char(5,3,Number[1]);*/
		/*
		disp_16x8x4_Char(1,4,PWM1);
		disp_16x8x4_Char(1,5,PWM1);
		disp_16x8x4_Char(1,3,PWM1);
		disp_16x8x4_Char(1,4,PWM1);
		disp_16x8x4_Char(1,5,PWM1);
		disp_16x8x4_Char(1,3,PWM1);
		disp_16x8x4_Char(1,4,PWM1);
		disp_16x8x4_Char(1,5,PWM1);*/
		
		//disp_16x8x4_Char(32,1,jing2[]);
		for(delayI=0;delayI<10;delayI++)
		{
			//delay(50); 
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




/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
