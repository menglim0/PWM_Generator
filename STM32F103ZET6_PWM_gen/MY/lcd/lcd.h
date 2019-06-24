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
/*sbit key=P2^0;		按键接口，P2.0口与GND之间接一个按键*/
//sbit Rom_IN=P3^1;   /*字库IC接口定义:Rom_IN就是字库IC的SI*/
//sbit Rom_OUT=P3^2;  /*字库IC接口定义:Rom_OUT就是字库IC的SO*/
//sbit Rom_SCK=P3^7;  /*字库IC接口定义:Rom_SCK就是字库IC的SCK*/
//sbit Rom_CS=P3^6;   /*字库IC接口定义Rom_CS就是字库IC的CS#*/
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
/*延时：1us的i倍*/
void delay_us(int i);       
/*等待一个按键，我的主板是用P2.0与GND之间接一个按键*/
void waitkey();
//写指令到LCD模块
void transfer_command_lcd(int data1);
//写数据到LCD模块
void transfer_data_lcd(int data1);

void initial_lcd();

void lcd_address(int x,int y, int x_total,int y_total);

/*清屏*/
void clear_screen();

void test(int x,int y);

//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(uchar column, uchar page,uchar *text);

/*显示32*32点阵的汉字或等同于32*32点阵的图像*/
void disp_32x32(int x,int y,unsigned char  *dp);

/*显示32*32点阵的汉字或等同于32*32点阵的图像*/
void disp_32x32_En(int x,int y,unsigned char  *dp);

void disp_16x8x4_Char(int x,int y,unsigned char const  *dp);
void disp_16x8x1_Char(int x,int y,unsigned char const  *dp);

void disp_16x16x4_Char(int x,int y,unsigned char const  *dp);
void disp_16x16x1_Char(int x,int y,unsigned char const  *dp);


void display_PWM_Channel(u8 channel_x,u8 line_x);

void display_PWM_Freq(u8 channel_x,u8 line_x,int freq);
void display_PWM_DutyCycle(u8 channel_x,u8 line_x,u8 dutycycle);
/*显示48*48点阵的汉字或图像*/
void disp_48x48(int x,int y,char *dp);

/*显示64*48点阵的汉字或图像*/
void disp_64x48(int x,int y,char *dp);

/*显示196*96点阵的图像*/
void disp_192x96(int x,int y,uchar const *dp);

/****送指令到晶联讯字库IC***/
void send_command_to_ROM( uchar datu );

//从晶联讯字库IC中取汉字或字符数据（1个字节）

static uchar get_data_from_ROM( );

//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_16x16(ulong fontaddr,uchar column,uchar page);
void display_string_16x8(uchar column, uchar page,uchar *text);

//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_8x16(ulong fontaddr,uchar column,uchar page);

void display_GB2312_string(uchar column,uchar page,uchar *text);


#endif
