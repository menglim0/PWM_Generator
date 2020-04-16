/*-------------------------------------------------------------------------------
�ļ����ƣ�main.c
�ļ�����������LED2��LED3��˸
Ӳ��ƽ̨����ĪM3S������
��д����shifang
�̼���  ��V3.5
������̳��www.doflye.net
��    ע��ͨ�����޸Ŀ�����ֲ�����������壬����������Դ�����硣
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

/* Scheduler includes. 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"*/

#include "My_InitTask.h" 
/*
sbit lcd_cs1  =  P3^4;//CS
sbit lcd_reset=  P3^5;//RST
sbit lcd_sclk =  P1^0;//����ʱ��
sbit lcd_rs   =  P3^3;//RS	  
sbit lcd_sid  =  P1^1;//�������� 
*/
	
/*
LCD Port: PD0-8
PWM Port: PD12 PC6 PA0/1/8 PB5
ADC Port: PA2-7,PB0,1;PC0,1,2,3
*/
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
// PA2 -7,PB0,1;PC0,1,2,3

// �ֲ����������ڱ���ת�������ĵ�ѹֵ	

//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              CLK   ��PA5��SCL��-->PB2
//              MOSI   ��PA7��SDA��-->PB3
//              RES  ��PB0-->PB4
//              DC   ��PB1--> PB5
//							BLK  ��A4 ��������-->PB6
//							MISO ���Բ���
//              ---------------------------------------------------------------- 
 
/******************USART0 PA9/PA10****/

u8 txbuf[16];

/*for dispaly 1.8TFT below*/


/*RTOS*/

void Task1 (void *data);


#define Freq_Position_X 100
#define Duty_Position_X 100




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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			 //����LED��� D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);

}


//-----------------------------
int main()
{		
	bool LED_State_01,Usart_Config_State;
	uint16_t delayI,cnt=0,channel_i;
	uint16_t ADC_ConvertedValueLocal,ADC_ConvertedValueLocal2; 
unsigned int adcx_Freq_Raw[8],adcx_DutyCycle_Raw[8],adcx_Freq_Old[8],adcx_DutyCycle_Old[8];
	uint8_t adcx_Freq_Changed[8],adcx_DutyCycle_Changed[8];
	uint16_t dutycycle;
	uint8_t ADC_index_i, Send_Buf_index;
		u16 i=0;	 
	u8 key=0;
	
	//LED_Init_G14();
  LED_Init();	
	delay_init();	
			NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
		uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	/* */
		//My_InitTask();

		TIM1_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM1,800);         //����ռ�ձ�Ϊ1/3
	 
	 	TIM2_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM2,800);         //����ռ�ձ�Ϊ1/3
		
		TIM3_PWM_Init(6009,1000);	 //����Ƶ��PWMƵ��=72000/900=8Khz
		TIM_SetCompare4(TIM3,800);	
	 
	 	TIM4_Int_Init(6009,1000);	
		TIM_SetCompare1(TIM4,1800);          //����ռ�ձ�Ϊ1/3
	 
	 	 
	 //	 TIM5_Int_Init(6009,100);	
		//TIM_SetCompare2(TIM5,800);         //����ռ�ձ�Ϊ1/3
	 
	 	// 	 		TIM8_Int_Init(6009,100);	
		//TIM_SetCompare1(TIM8,800);         //����ռ�ձ�Ϊ1/3
	
	//ADC1_Init();
	Adc_Init();
	
	if(0)
	{
		//initial_lcd(); 									//��Һ��ģ����г�ʼ������
		delay(500);
		//clear_screen();	
		//display_string_16x16(32,4,"����ʵҵ���޹�˾");
			//	display_string_16x16(32,6,"����ʵҵ���޹�˾");
				/**/
		delay(50000);
		delay(50000);
		//clear_screen();	
	}
	
	
		delay(500);
	//SPI1_Init();	//SPI1��ʼ��
	delay(500);
	Lcd_Init();	
	LCD_Clear(WHITE); //����	
	delay(500);
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	xianshi();	   //��ʾ��Ϣ

	
	Usart_Config_State=!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
	
	
	while(1)
	{
	
	delay(1000);
			
		if(Usart_Config_State==FALSE)
		{
			if(Usart1_Receive_Complete==TRUE)
			{
				
				if(Usart1_Rec_Cnt==16)
				{
					for(channel_i=0;channel_i<4;channel_i++)
					{
						
						adcx_Freq[channel_i+4]=(DMA_Rece_Buf[2+channel_i*4]<<8)+DMA_Rece_Buf[3+channel_i*4];
						adcx_DutyCycle[channel_i+4]=DMA_Rece_Buf[1+channel_i*4];
//						txbuf[0+channel_i*4]=channel_i;					
//						txbuf[1+channel_i*4]=dutycycle;
//						txbuf[2+channel_i*4]=adcx_Freq[channel_i]>>8;
//						txbuf[3+channel_i*4]=adcx_Freq[channel_i]&0xFF;
					
					}
				}
				//Usart1_Send(DMA_Rece_Buf,Usart1_Rec_Cnt);
				Usart1_Receive_Complete=FALSE;
			}
		}
		
for(ADC_index_i=0;ADC_index_i<4;ADC_index_i++)
		{
			adcx_Freq_Raw[ADC_index_i] = Get_Adc_Average(ADC_index_i+1,100); // ��ȡת����ADֵ
			adcx_DutyCycle_Raw[ADC_index_i] = Get_Adc_Average(ADC_index_i+5,100); // ��ȡת����ADֵ
		}
		
//		adcx_Freq_Raw[0] = Get_Adc_Average(ADC_Channel_1,100); // ��ȡת����ADֵ
//		adcx_Freq_Raw[1] = Get_Adc_Average(ADC_Channel_2,100); // ��ȡת����ADֵ
//		adcx_Freq_Raw[2] = Get_Adc_Average(ADC_Channel_3,100); // ��ȡת����ADֵ
//		adcx_Freq_Raw[3] = Get_Adc_Average(ADC_Channel_4,100); // ��ȡת����ADֵ
//		
//		adcx_DutyCycle_Raw[0] = Get_Adc_Average(ADC_Channel_5,100); // ��ȡת����ADֵ
//		adcx_DutyCycle_Raw[1] = Get_Adc_Average(ADC_Channel_6,100); // ��ȡת����ADֵ
//		adcx_DutyCycle_Raw[2] = Get_Adc_Average(ADC_Channel_7,100); // ��ȡת����ADֵ
//		adcx_DutyCycle_Raw[3] = Get_Adc_Average(ADC_Channel_8,100); // ��ȡת����ADֵ
		
	//	adcx_Freq_Raw[4] = Get_Adc_Average(ADC_Channel_5,100); // ��ȡת����ADֵ
	//	adcx_Freq_Raw[5] = Get_Adc_Average(ADC_Channel_6,100); // ��ȡת����ADֵ
	//	adcx_Freq_Raw[6] = Get_Adc_Average(ADC_Channel_7,100); // ��ȡת����ADֵ
		//adcx_Freq_Raw[6] = Get_Adc_Average(ADC_Channel_0,100); // ��ȡת����ADֵ
		//adcx_Freq_Raw[7] = Get_Adc_Average(ADC_Channel_1,100); // ��ȡת����ADֵ		

		
//		adcx_DutyCycle_Raw[4] = Get_Adc_Average(ADC_Channel_12,100); // ��ȡת����ADֵ
//		adcx_DutyCycle_Raw[5] = Get_Adc_Average(ADC_Channel_13,100); // ��ȡת����ADֵ
//		adcx_DutyCycle_Raw[6] = Get_Adc_Average(ADC_Channel_14,100); // ��ȡת����ADֵ
//		adcx_DutyCycle_Raw[7] = Get_Adc_Average(ADC_Channel_15,100); // ��ȡת����ADֵ
//		
		
		for(ADC_index_i=0;ADC_index_i<4;ADC_index_i++)
		{
		//adcx[ADC_index_i] = Get_Adc_Average(ADC_index_i+1,100); // ��ȡת����ADֵ
			adcx_Freq[ADC_index_i]=Get_Adc_Filter(adcx_Freq_Raw[ADC_index_i],100);
			adcx_DutyCycle[ADC_index_i]=Get_Adc_Filter(adcx_DutyCycle_Raw[ADC_index_i],100);
			
			if(adcx_DutyCycle[ADC_index_i]>2050)
			{
				adcx_DutyCycle[ADC_index_i]=2050;
			}
			else if(adcx_DutyCycle[ADC_index_i]<50)
			{
			adcx_DutyCycle[ADC_index_i]=50;
			}
			
			adcx_DutyCycle[ADC_index_i] = (adcx_DutyCycle[ADC_index_i]-50)/20;
			
			
			if(adcx_Freq[ADC_index_i]!=adcx_Freq_Old[ADC_index_i])
			{
			adcx_Freq_Changed[ADC_index_i]=1;
				if(ADC_index_i==2)
				{
				//	TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);//TIM3�ж�ʹ��
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
					dutycycle=adcx_DutyCycle[channel_i];
					
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
				
				
	
			//PWM_Freq_DC(channel_i,adcx_DutyCycle[channel_i],adcx_Freq[channel_i]);
				
					//display_Ch_Fre_Duty(channel_i,adcx_Freq[channel_i],adcx_DutyCycle[channel_i]);	
	
				
						
				/*1.3 OLED display*/
				//display_Ch_Fre_Duty(channel_i+1,adcx_Freq[channel_i],adcx_DutyCycle[channel_i]);
			}
		}
		
		if(Usart_Config_State==FALSE)
		{
				for(channel_i=0;channel_i<8;channel_i++)
				{
					display_Ch_Fre_Duty(channel_i,adcx_Freq[channel_i],adcx_DutyCycle[channel_i]);	
				}
		}
	
		
		//disp_16x8x4_Char(32,1,jing2[]);
		for(delayI=0;delayI<100;delayI++)
		{
			delay(200); 
		}
			cnt=cnt+1;
		
		for(i=0;i<8;i++)
		{

		}
		if(cnt>=100)
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
		 
   PCout(13)=!PCout(13);            //  ����LED 
		vTaskDelay( 10); 
	
		

	}
}


/*----------------------�·��� ������̳��www.doflye.net--------------------------*/
