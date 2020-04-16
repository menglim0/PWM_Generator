#include "display.h"
#include "lcd.h"

u8 state=0;

void display_Ch_Fre_Duty(u8 ch,u16 freq,u8 duty)
{
	u8 channel;
	u16 pos_Freq_x=92+40,pos_Freq_y=16,pos_Duty_x=172+40+40,pos_Duty_y=16;
	//LCD_Clear(WHITE); //清屏
		channel = (ch-1)*20+35;
	pos_Freq_y=(ch+1)*24+4;
	pos_Duty_y=pos_Freq_y;
//void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
		//LCD_ShowxNum(26,channel,ch,1);

	
	//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
//void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
			//LCD_Fill(pos_Freq_x,(ch+3)*18+4,pos_Freq_x+4*8,(ch+3)*18+4+16,0xFFFF);
			LCD_Fill(pos_Freq_x,pos_Freq_y,pos_Freq_x+4*8,(ch+1)*24+4+16,0xFFFF);
		LCD_ShowxNum(pos_Freq_x,pos_Freq_y,freq,4,16,0x01);
		LCD_Fill(pos_Duty_x,pos_Duty_y,pos_Duty_x+3*8,(ch+1)*24+4+16,0xFFFF);
		LCD_ShowxNum(pos_Duty_x,pos_Duty_y,duty,3,16,0x01);	
		
		
		//LCD_ShowNum(pos_Freq_x,pos_Freq_y,freq,4,16);

		//LCD_ShowNum(pos_Duty_x,pos_Duty_y,duty,4,16);	
}

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
	//LCD_ShowString(4,0,200,12,12,"CH FREQ DUTY");
for(index_Show=0;index_Show<8;index_Show++)	
	{
		/*
		LCD_ShowString(12,(index_Show+3)*18+4,300,16,16,"PWM1 Freq:1024 Duty:100 %");
<<<<<<< HEAD
		LCD_ShowxNum(12+24,(index_Show+3)*18+4,index_Show,1,16,0x00);

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
//void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)	
=======
		*/
		LCD_ShowString(12,(index_Show+1)*24+4,300,16,16,"PWM  Frequency:1024 DutyCycle:100 %");
		
		LCD_ShowxNum(36,(index_Show+1)*24+4,index_Show+1,1,16,0x01);
>>>>>>> origin/master
		display_Ch_Fre_Duty(index_Show,1000,50);
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


