/*-------------------------------------------------------------------------------
文件名称：led.c
文件描述：根据硬件连接配置LED端口，打开对应的寄存器        
备    注：无
---------------------------------------------------------------------------------*/
#include "lcd.h"
#include "stm32f10x.h"

//#include "intrins.h"
#include "led.h"
#include "chinese_code.h"
#include "delay.h"

#define Freq_Pos_Thound 7
#define Freq_Pos_Hund 8
#define Freq_Pos_Ten 9
#define Freq_Pos_Unit 10
#define Freq_Pos_H 11
#define Freq_Pos_Z 12
#define Duty_Pos_Hund 14
#define Duty_Pos_Ten 15
#define Duty_Pos_Unit 16
#define Duty_Pos_Perc 17
		

/*等待一个按键，我的主板是用P2.0与GND之间接一个按键*/
void waitkey()
{
 repeat:
  	if (key==1) goto repeat;
	else delay(2000);
}


//写指令到LCD模块
void transfer_command_lcd(int data1)   
{
	char i;
	lcd_cs1_off;
	lcd_rs_off;
	for(i=0;i<8;i++)
	{
		delay(1);
		lcd_sclk_off;	
		if(data1&0x80) lcd_sid_on;
		else lcd_sid_off;
		delay(1);
		lcd_sclk_on;
		data1<<=1;
	}
	lcd_cs1_on;
	delay(1);
}

//写数据到LCD模块
void transfer_data_lcd(int data1)
{
	char i;
	lcd_cs1_off;
	lcd_rs_on;
	for(i=0;i<8;i++)
	{
		delay(1);
		lcd_sclk_off;
		if(data1&0x80) lcd_sid_on;
		else lcd_sid_off;
		
		delay(1);
		lcd_sclk_on;
		data1<<=1;
	}
	lcd_cs1_on;
	delay(1);
}


void initial_lcd()
{   
	lcd_reset_off;
	delay(100);
	lcd_reset_on;
	delay(100);

	transfer_command_lcd(0x30);   //EXT=0
	transfer_command_lcd(0x94);   //Sleep out
	transfer_command_lcd(0x31);   //EXT=1
	transfer_command_lcd(0xD7);   //Autoread disable
	transfer_data_lcd(0X9F);      // 

	transfer_command_lcd(0x32);   //Analog SET
	transfer_data_lcd(0x00);  		//OSC Frequency adjustment
	transfer_data_lcd(0x01);  		//Frequency on booster capacitors->6KHz
	transfer_data_lcd(0x03);  		//Bias=1/11

	transfer_command_lcd(0x20);   // Gray Level
	transfer_data_lcd(0x01); 
	transfer_data_lcd(0x03);
	transfer_data_lcd(0x05);
	transfer_data_lcd(0x07); 
	transfer_data_lcd(0x09);
	transfer_data_lcd(0x0b);
	transfer_data_lcd(0x0d);
	transfer_data_lcd(0x10);
	transfer_data_lcd(0x11);
	transfer_data_lcd(0x13);
	transfer_data_lcd(0x15);
	transfer_data_lcd(0x17);
	transfer_data_lcd(0x19);
	transfer_data_lcd(0x1b);
	transfer_data_lcd(0x1d);
	transfer_data_lcd(0x1f);

	transfer_command_lcd(0x30);   //EXT=0
	transfer_command_lcd(0x75);   //Page Address setting
	transfer_data_lcd(0X00);      // XS=0
	transfer_data_lcd(0X14);      // XE=159 0x28
	transfer_command_lcd(0x15);   //Clumn Address setting
	transfer_data_lcd(0X00);      // XS=0
	transfer_data_lcd(0Xff);      // XE=256

	transfer_command_lcd(0xBC);    //Data scan direction
	transfer_data_lcd(0x00);  		 //MX.MY=Normal
	transfer_data_lcd(0xA6);
	
	transfer_command_lcd(0x0C);    //数据格式选择,0x0C是低位在前D0-D7,0x08是高位在前D7-D0

	transfer_command_lcd(0xCA);    //Display Control
	transfer_data_lcd(0X00);  		 // 
	transfer_data_lcd(0X9F);  		 //Duty=160
	transfer_data_lcd(0X20);  		 //Nline=off

	transfer_command_lcd(0xF0);    //Display Mode 
	transfer_data_lcd(0X10);  		 //10=Monochrome Mode,11=4Gray

	transfer_command_lcd(0x81);    //EV control
	transfer_data_lcd(0x38);   		 //VPR[5-0]
	transfer_data_lcd(0x03);  		 //粗调对比度
	transfer_command_lcd(0x20);    //Power control
	transfer_data_lcd(0x0B);   		 //D0=regulator ; D1=follower ; D3=booste,  on:1 off:0
	delay_us(100);           
	transfer_command_lcd(0xAF);    //Display on
	
}


/*写LCD 行列地址：X为起始的列地址，Y为起始的行地址，x_total,y_total分别为列地址及行地址的起点到终点的差值 */
void lcd_address(int x,int y, int x_total,int y_total)
{
	x=x-1;
	y=y+7;
	
	transfer_command_lcd(0x15);	//Set Column Address
	transfer_data_lcd(x);		
	transfer_data_lcd(x+x_total-1);		

	transfer_command_lcd(0x75);	//Set Page Address	
	transfer_data_lcd(y);		
	transfer_data_lcd(y+y_total-1);	
	transfer_command_lcd(0x30);	
	transfer_command_lcd(0x5c);	

}


/*清屏*/
void clear_screen()
{
	int i,j;	
	lcd_address(0,0,256,17);
	for(i=0;i<17;i++)
	{
		for(j=0;j<256;j++)
		{
			transfer_data_lcd(0x00);
		}
	}
}


void test(int x,int y)
{
	int i,j;	
	lcd_address(x,y,256,16);

	for(i=0;i<16;i++)
	{
		for(j=0;j<256;j++)
		{
			transfer_data_lcd(0xff);
		}
	}
}


//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(uchar column, uchar page,uchar *text)
{
	uchar i,j,k;
	uint address;
	j=0;
	while(text[j]!= '\0')
	{
		i=0;
		address=1;
		while(Chinese_text_16x16[i]> 0x7e)
		{
			if(Chinese_text_16x16[i] == text[j])
			{
				if(Chinese_text_16x16[i+1] == text[j+1])
				{
					address=i*16;
					break;
				}
			}
			i +=2;
		}
		if(column>255)
		{
			column=0;
			page+=2;
		}
		if(address !=1)
		{
			lcd_address(column,page,16,2);
			for(k=0;k<2;k++)
			{
				for(i=0;i<16;i++)
				{
					transfer_data_lcd(Chinese_code_16x16[address]);
					address++;
				}
			}
			j +=2;
		}
		else
		{
			lcd_address(column,page,16,2);
			for(k=0;k<2;k++)
			{
				for(i=0;i<16;i++)
				{
					transfer_data_lcd(0x00);
				}
			}
			j++;
		}
		column+=16;
	}
}

void display_string_16x8(uchar column, uchar page,uchar *text)
{
	uchar i,j,k;
	uint address;
	j=0;
	while(text[j]!= '\0')
	{
		i=0;
		address=1;

			if(Chinese_text_16x8[i] == text[j])
			{
				if(Chinese_text_16x8[i+1] == text[j+1])
				{
					address=i*8;
					break;
				}
			}
			i +=2;
		
		if(column>255)
		{
			column=0;
			page+=2;
		}
		if(address !=1)
		{
			lcd_address(column,page,8,2);
			for(k=0;k<2;k++)
			{
				for(i=0;i<8;i++)
				{
					transfer_data_lcd(Chinese_code_16x8[address]);
					address++;
				}
			}
			j +=2;
		}
		else
		{
			lcd_address(column,page,8,2);
			for(k=0;k<2;k++)
			{
				for(i=0;i<16;i++)
				{
					transfer_data_lcd(0x00);
				}
			}
			j++;
		}
		column+=16;
	}
}
/*显示32*32点阵的汉字或等同于32*32点阵的图像*/
void disp_32x32(int x,int y,unsigned char  *dp)
{
	int i,j; 
	lcd_address(x,y,32,4);
	for(i=0;i<4;i++)
	{
		for(j=0;j<32;j++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}

/*显示32*32点阵的汉字或等同于32*32点阵的图像*/
void disp_16x8x4_Char(int x,int y,unsigned char const  *dp)
{
	int i,j; 
	lcd_address(x,y,40,2);
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}

void disp_16x16x4_Char(int x,int y,unsigned char const  *dp)
{
disp_16x16x1_Char(x,y,dp);
disp_16x16x1_Char(x,y+2,dp+16);
disp_16x16x1_Char(x,y+4,dp+32);
}


void disp_16x16x1_Char(int x,int y,unsigned char const  *dp)
{
	int i,j,start_pos; 
	start_pos=8*x+1;
	lcd_address(start_pos,y,8,2);
		for(i=0;i<2;i++)
	{
		for(j=0;j<8;j++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}

/*显示32*32点阵的汉字或等同于32*32点阵的图像*/
void disp_16x8x1_Char(int x,int y,unsigned char const  *dp)
{
	int i,j,start_pos; 
	start_pos=8*x+1;
	lcd_address(start_pos,y,8,1);

		for(j=0;j<8;j++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}

}

/*显示48*48点阵的汉字或图像*/
void disp_48x48(int x,int y,char *dp)
{
	int i,j;
	lcd_address(x,y,48,6);
	for(i=0;i<6;i++)	  
	{
		for(j=0;j<48;j++)  
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}


/*显示64*48点阵的汉字或图像*/
void disp_64x48(int x,int y,char *dp)
{
	int i,j;
	lcd_address(x,y,55,6);
	for(i=0;i<6;i++)	  
	{
		for(j=0;j<55;j++)  
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}


/*显示196*96点阵的图像*/
void disp_192x96(int x,int y,uchar const *dp)
{
	int i,j;
	lcd_address(x,y,192,12);
	for(i=0;i<12;i++)	  
	{
		for(j=0;j<192;j++)  
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}


/****送指令到晶联讯字库IC***/
void send_command_to_ROM( uchar datu )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
			Rom_SCK_off;
		if(datu&0x80)
			Rom_IN_on ;
		else
			Rom_IN_on ;
			datu = datu<<1;
			Rom_SCK_on;
			delay_us(1);
	}
}

//从晶联讯字库IC中取汉字或字符数据（1个字节）

static uchar get_data_from_ROM( )
{
	uchar i;
	uchar ret_data=0;
	for(i=0;i<8;i++)
	{
		Rom_OUT_on;
		Rom_SCK_off;
		//delay_us(1);
		ret_data=ret_data<<1;
		if( 1 )
			ret_data=ret_data+1;
		else
			ret_data=ret_data+0;
		Rom_SCK_on;
		//delay_us(1);
	}
	return(ret_data);
}



//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_16x16(ulong fontaddr,uchar column,uchar page)
{
	uchar i,j,disp_data;
	Rom_CS_off;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位

	lcd_address(column,page,16,2);
	for(j=0;j<2;j++)
	{
		for(i=0; i<16; i++ )
	  {
			disp_data=get_data_from_ROM();
			transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	Rom_CS_on;
}

//从指定地址读出数据写到液晶屏指定（page,column)座标中
void get_and_write_8x16(ulong fontaddr,uchar column,uchar page)
{
	uchar i,j,disp_data;
	Rom_CS_off;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);	 //地址的高8位,共24位
	send_command_to_ROM((fontaddr&0xff00)>>8);		 //地址的中8位,共24位
	send_command_to_ROM(fontaddr&0xff);				 //地址的低8位,共24位

	lcd_address(column,page,8,2);
	for(j=0;j<2;j++)
	{
		for(i=0; i<8; i++ )
	  {
			disp_data=get_data_from_ROM();
			transfer_data_lcd(disp_data);	//写数据到LCD,每写完1字节的数据后列地址自动加1
		}
	}
	Rom_CS_on;
}


//****************************************************************

ulong  fontaddr=0;
void display_GB2312_string(uchar column,uchar page,uchar *text)
{
	uchar i= 0,temp1,temp2;
	temp1=column;
	temp2=page;
	
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);

			get_and_write_16x16(fontaddr,column,page);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;					
		}

		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			//国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
	
			get_and_write_16x16(fontaddr,column,page);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;		
		}
			
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{									
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			

			get_and_write_8x16(fontaddr,column,page);	 //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=1;
			column+=8;
		}
		else
			i++;
	}		
}

void display_PWM_Channel(u8 channel_x,u8 line_x)
{
	u8 line_Ofset=3;
	
	//disp_16x16x4_Char(1,line_x*2+line_Ofset,PWM_16);
	//isp_16x16x1_Char(1,line_x*2+1,P_16);
	disp_16x16x1_Char(1,line_x*2+1,P_16);
	disp_16x16x1_Char(2,line_x*2+1,W_16);
	disp_16x16x1_Char(3,line_x*2+1,M_16);
	disp_16x16x1_Char(4,line_x*2+1,Number[channel_x]);
	disp_16x16x1_Char(5,line_x*2+1,colon);
	//disp_16x16x4_Char(3,line_x*2+1,P_16);
	//disp_16x16x4_Char(5,line_x*2+1,P_16);
	/*
		disp_16x8x4_Char(1,line_x+line_Ofset,PWM1);
		disp_16x8x1_Char(3,line_x+line_Ofset,Number[channel_x]);
		disp_16x8x1_Char(4,line_x+line_Ofset,colon);
	*/
}

void display_PWM_Freq(u8 channel_x,u8 line_x,int freq)
{
	u8 line_Ofset=1;
	u8 Thousand_Bit,Hund_Bit,Ten_Bit,Unit_bit;

	
  Thousand_Bit = freq/1000;
	Hund_Bit = freq%1000/100;
	Ten_Bit = freq%100/10;
	Unit_bit= freq%10;
	


	
	if(Thousand_Bit ==0)
	{
	disp_16x16x1_Char(Freq_Pos_Thound,line_x+line_Ofset,Zero_Clear);
		if(Hund_Bit==0)
		{
			disp_16x16x1_Char(Freq_Pos_Hund,line_x+line_Ofset,Zero_Clear);
			if(Ten_Bit==0)
			{
				disp_16x16x1_Char(Freq_Pos_Ten,line_x+line_Ofset,Zero_Clear);
			
				if(Unit_bit==0)
				{
					disp_16x16x1_Char(Freq_Pos_Unit,line_x+line_Ofset,Zero_Clear);
				}
			}
			else
			{
			disp_16x16x1_Char(Freq_Pos_Ten,line_x+line_Ofset,Number[Ten_Bit]);
			disp_16x16x1_Char(Freq_Pos_Unit,line_x*2+line_Ofset,Number[Unit_bit]);
			}
		}
		else
		{
		disp_16x16x1_Char(Freq_Pos_Hund,line_x+line_Ofset,Number[Hund_Bit]);
		disp_16x16x1_Char(Freq_Pos_Ten,line_x+line_Ofset,Number[Ten_Bit]);
		disp_16x16x1_Char(Freq_Pos_Unit,line_x+line_Ofset,Number[Unit_bit]);
		}
			
		
	}
	else
	{
		//disp_16x8x4_Char(1,line_x+line_Ofset,PWM1);
		disp_16x16x1_Char(Freq_Pos_Thound,line_x+line_Ofset,Number[Thousand_Bit]);
		disp_16x16x1_Char(Freq_Pos_Hund,line_x+line_Ofset,Number[Hund_Bit]);
		disp_16x16x1_Char(Freq_Pos_Ten,line_x+line_Ofset,Number[Ten_Bit]);
		disp_16x16x1_Char(Freq_Pos_Unit,line_x+line_Ofset,Number[Unit_bit]);
	}
	
	disp_16x16x1_Char(Freq_Pos_H,line_x+line_Ofset,H);
	disp_16x16x1_Char(Freq_Pos_Z,line_x+line_Ofset,Z);
	
		
}

void display_PWM_DutyCycle(u8 channel_x,u8 line_x,u8 dutycycle)
{
	u8 line_Ofset=1;
	
		u8 Thousand_Bit,Hund_Bit,Ten_Bit,Unit_bit;	
//Thousand_Bit = dutycycle/1000;
	Hund_Bit = dutycycle%1000/100;
	Ten_Bit = dutycycle%100/10;
	Unit_bit= dutycycle%10;
	
		if(Hund_Bit==0)
		{
			disp_16x16x1_Char(Duty_Pos_Hund,line_x+line_Ofset,Zero_Clear);
			if(Ten_Bit==0)
			{
				disp_16x16x1_Char(Duty_Pos_Ten,line_x+line_Ofset,Zero_Clear);
			disp_16x16x1_Char(Duty_Pos_Unit,line_x+line_Ofset,Number[Unit_bit]);
			}
				else
				{
				disp_16x16x1_Char(Duty_Pos_Ten,line_x+line_Ofset,Number[Ten_Bit]);
				disp_16x16x1_Char(Duty_Pos_Unit,line_x+line_Ofset,Number[Unit_bit]);
				}
		}
		else
		{
		disp_16x16x1_Char(Duty_Pos_Hund,line_x+line_Ofset,Number[Hund_Bit]);
		disp_16x16x1_Char(Duty_Pos_Ten,line_x+line_Ofset,Number[Ten_Bit]);
		disp_16x16x1_Char(Duty_Pos_Unit,line_x+line_Ofset,Number[Unit_bit]);
			
		}
		disp_16x16x1_Char(Duty_Pos_Perc,line_x+line_Ofset,Percent);
/*
	disp_16x16x1_Char(Duty_Pos_Hund,line_x+line_Ofset,Number[Hund_Bit]);
		disp_16x16x1_Char(Duty_Pos_Ten,line_x+line_Ofset,Number[Ten_Bit]);
		disp_16x16x1_Char(Duty_Pos_Unit,line_x+line_Ofset,Number[Unit_bit]);
		disp_16x16x1_Char(Duty_Pos_Perc,line_x+line_Ofset,Percent);*/
}


