#include "display.h"
#include "lcd.h"

u8 state=0;

void display_Ch_Fre_Duty(u8 ch,u16 freq,u8 duty)
{
//	u8 channel;
	u16 pos_Freq_x=36+48,pos_Freq_y=16,pos_Duty_x=80+40+40,pos_Duty_y=16;
	//LCD_Clear(WHITE); //清屏
	//channel = (ch-1)*20+35;
	pos_Freq_y=(ch+1)*24+4;
	pos_Duty_y=pos_Freq_y;

		//LCD_Fill(pos_Freq_x,pos_Freq_y,pos_Freq_x+4*8,(ch+1)*24+4+16,0xFFFF);
		LCD_ShowNum(pos_Freq_x,pos_Freq_y,freq,4);

		//LCD_Fill(pos_Duty_x,pos_Duty_y,pos_Duty_x+3*8,(ch+1)*24+4+16,0xFFFF);
		LCD_ShowNum(pos_Duty_x,pos_Duty_y,duty,4);	
}



void xianshi()//显示信息
{ 
	u8 index_Show;
	u8 pos_x;
	//u16 lx,ly;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	  LCD_ShowString(80,5,"PWM Display");
for(index_Show=0;index_Show<8;index_Show++)
	{
	pos_x=index_Show*24+28;
	LCD_ShowString(10,pos_x,"Ch :Freq");
	LCD_ShowNum(10+16,pos_x,index_Show+1,1);
	LCD_ShowString(128,pos_x,"Duty");
	LCD_ShowString(196,pos_x,"%");
	}
	//LCD_ShowString(10,5,"LCD_W:");	LCD_ShowNum(70,55,LCD_W,3);
	//LCD_ShowString(110,5,"LCD_H:");LCD_ShowNum(160,55,LCD_H,3);	
	//lx=10;ly=75;	


	
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


