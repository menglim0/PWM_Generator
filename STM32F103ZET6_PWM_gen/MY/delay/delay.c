/*-------------------------------------------------------------------------------
文件名称：led.c
文件描述：根据硬件连接配置LED端口，打开对应的寄存器        
备    注：无
---------------------------------------------------------------------------------*/
#include "delay.h"


/*延时：1毫秒的i倍*/
void delay(int i)               
{
	int j,k;
	for(j=0;j<i;j++)
		for(k=0;k<110;k++);
}


/*延时：1us的i倍*/
void delay_us(int i)               
{
	int j,k;
	for(j=0;j<i+10;j++)
		for(k=0;k<1;k++);
}

void delay_ms(int i)               
{
	delay(i); 
}



