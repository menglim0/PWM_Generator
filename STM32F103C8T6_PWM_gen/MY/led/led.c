/*-------------------------------------------------------------------------------
�ļ����ƣ�led.c
�ļ�����������Ӳ����������LED�˿ڣ��򿪶�Ӧ�ļĴ���        
��    ע����
---------------------------------------------------------------------------------*/
#include "led.h"
/*-------------------------------------------------------------------------------
�������ƣ�LED_Init
������������ʼ��LED��ض˿ڣ��򿪶˿�ʱ�ӣ����ö˿����  
�����������
���ز�������
��    ע����
---------------------------------------------------------------------------------*/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //��PB��ʱ��
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //�������
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 //GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	//�˿��ٶ�
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//�˿�ģʽ����Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//��ʼ����Ӧ�Ķ˿�
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //GPIO_Init(GPIOE, &GPIO_InitStructure);
}
