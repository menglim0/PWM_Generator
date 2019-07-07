#include "display.h"
#include "lcd.h"


void display_Ch_Fre_Duty(u8 ch,u16 freq,u8 duty)
{
	u8 channel;
	u16 pos_Freq_x,pos_Freq_y,pos_Duty_x,pos_Duty_y;
	//LCD_Clear(WHITE); //«Â∆¡
		channel = (ch-1)*20+35;
//void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
		//LCD_ShowxNum(26,channel,ch,1);


		LCD_ShowxNum(pos_Freq_x,pos_Freq_y,94,channel,freq,4);

		LCD_ShowxNum(pos_Duty_x,pos_Duty_y,184,channel,duty,3);	

}


