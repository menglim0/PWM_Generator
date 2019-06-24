#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"

//#include "intrins.h"
#include "led.h"
//#include "chinese_code.h"

#define uchar unsigned char 

#define uint unsigned int
#define ulong unsigned long

#define lcd_cs1_on  GPIO_SetBits(GPIOD,GPIO_Pin_4)
#define lcd_cs1_off  GPIO_ResetBits(GPIOD,GPIO_Pin_4)

#define lcd_reset_on  GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define lcd_reset_off  GPIO_ResetBits(GPIOD,GPIO_Pin_2)

#define lcd_sclk_on  GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define lcd_sclk_off  GPIO_ResetBits(GPIOD,GPIO_Pin_6)

#define lcd_rs_on  GPIO_SetBits(GPIOD,GPIO_Pin_0)
#define lcd_rs_off  GPIO_ResetBits(GPIOD,GPIO_Pin_0)

#define lcd_sid_on  GPIO_SetBits(GPIOD,GPIO_Pin_8)
#define lcd_sid_off  GPIO_ResetBits(GPIOD,GPIO_Pin_8)


#define key 0
//GPIO_SetBits(GPIOB,GPIO_Pin_5)
///GPIO_ResetBits(GPIOB,GPIO_Pin_5)
/*sbit key=P2^0;		�����ӿڣ�P2.0����GND֮���һ������*/
//sbit Rom_IN=P3^1;   /*�ֿ�IC�ӿڶ���:Rom_IN�����ֿ�IC��SI*/
//sbit Rom_OUT=P3^2;  /*�ֿ�IC�ӿڶ���:Rom_OUT�����ֿ�IC��SO*/
//sbit Rom_SCK=P3^7;  /*�ֿ�IC�ӿڶ���:Rom_SCK�����ֿ�IC��SCK*/
//sbit Rom_CS=P3^6;   /*�ֿ�IC�ӿڶ���Rom_CS�����ֿ�IC��CS#*/
/*

#define Rom_IN_on  GPIO_WriteBit(GPIOD, GPIO_Pin_7,1)
#define Rom_IN_off  GPIO_WriteBit(GPIOD, GPIO_Pin_7,0)

#define Rom_OUT_on  GPIO_SetBits(GPIOD,GPIO_Pin_5)
#define Rom_OUT_off  GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define Rom_SCK_on  GPIO_SetBits(GPIOD,GPIO_Pin_3)
#define Rom_SCK_off  GPIO_ResetBits(GPIOD,GPIO_Pin_3)

#define Rom_CS_on  GPIO_SetBits(GPIOD,GPIO_Pin_1)
#define Rom_CS_off  GPIO_ResetBits(GPIOD,GPIO_Pin_1)
*/
#define Rom_IN_on  0
#define Rom_IN_off  0

#define Rom_OUT_on  0
#define Rom_OUT_off  0

#define Rom_SCK_on  0
#define Rom_SCK_off  0

#define Rom_CS_on  0
#define Rom_CS_off  0
void delay(int i);  
/*��ʱ��1us��i��*/
void delay_us(int i);       
/*�ȴ�һ���������ҵ���������P2.0��GND֮���һ������*/
void waitkey();
//дָ�LCDģ��
void transfer_command_lcd(int data1);
//д���ݵ�LCDģ��
void transfer_data_lcd(int data1);

void initial_lcd();

void lcd_address(int x,int y, int x_total,int y_total);

/*����*/
void clear_screen();

void test(int x,int y);

//д��һ��16x16����ĺ����ַ������ַ���������躬�д��֣�
//������Ĳ�����(ҳ���У������ַ�����
void display_string_16x16(uchar column, uchar page,uchar *text);

/*��ʾ32*32����ĺ��ֻ��ͬ��32*32�����ͼ��*/
void disp_32x32(int x,int y,unsigned char  *dp);

/*��ʾ32*32����ĺ��ֻ��ͬ��32*32�����ͼ��*/
void disp_32x32_En(int x,int y,unsigned char  *dp);

void disp_16x8x4_Char(int x,int y,unsigned char const  *dp);
void disp_16x8x1_Char(int x,int y,unsigned char const  *dp);

void disp_16x16x4_Char(int x,int y,unsigned char const  *dp);
void disp_16x16x1_Char(int x,int y,unsigned char const  *dp);


void display_PWM_Channel(u8 channel_x,u8 line_x);

void display_PWM_Freq(u8 channel_x,u8 line_x,int freq);
void display_PWM_DutyCycle(u8 channel_x,u8 line_x,u8 dutycycle);
/*��ʾ48*48����ĺ��ֻ�ͼ��*/
void disp_48x48(int x,int y,char *dp);

/*��ʾ64*48����ĺ��ֻ�ͼ��*/
void disp_64x48(int x,int y,char *dp);

/*��ʾ196*96�����ͼ��*/
void disp_192x96(int x,int y,uchar const *dp);

/****��ָ�����Ѷ�ֿ�IC***/
void send_command_to_ROM( uchar datu );

//�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�

static uchar get_data_from_ROM( );

//��ָ����ַ��������д��Һ����ָ����page,column)������
void get_and_write_16x16(ulong fontaddr,uchar column,uchar page);
void display_string_16x8(uchar column, uchar page,uchar *text);

//��ָ����ַ��������д��Һ����ָ����page,column)������
void get_and_write_8x16(ulong fontaddr,uchar column,uchar page);

void display_GB2312_string(uchar column,uchar page,uchar *text);


#endif
