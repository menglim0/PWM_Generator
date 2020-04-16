

#include "lcd.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "stm32f10x.h"
u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色
void LCD_Writ_Bus(char dat)   //串行数据写入
{	
	u8 i;			  
  

	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}			
}

void LCD_WR_DATA8(char da) //发送数据-8位参数
{	//OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da);  
	//OLED_CS_Set();
}  
 void LCD_WR_DATA(int da)
{//	OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
	//OLED_CS_Set();
}	  
void LCD_WR_REG(char da)	 
{	//	OLED_CS_Clr();
    OLED_DC_Clr();
	LCD_Writ_Bus(da);
	//OLED_CS_Set();
}
 void LCD_WR_REG_DATA(int reg,int da)
{	//OLED_CS_Clr();
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
	//OLED_CS_Set();
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);					 						 
}

void Lcd_Init()
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
	
 	GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5);	


	//OLED_CS_Clr();  //打开片选使能
	 OLED_RST_Clr();
	delay_ms(20);
	OLED_RST_Set();
	delay_ms(20);
	OLED_BLK_Set();
	
//************* Start Initial Sequence **********// 
LCD_WR_REG(0x36); 
LCD_WR_DATA8(0x00);

LCD_WR_REG(0x3A); 
LCD_WR_DATA8(0x05);

LCD_WR_REG(0xB2);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x00);
LCD_WR_DATA8(0x33);
LCD_WR_DATA8(0x33);

LCD_WR_REG(0xB7); 
LCD_WR_DATA8(0x35);  

LCD_WR_REG(0xBB);
LCD_WR_DATA8(0x19);

LCD_WR_REG(0xC0);
LCD_WR_DATA8(0x2C);

LCD_WR_REG(0xC2);
LCD_WR_DATA8(0x01);

LCD_WR_REG(0xC3);
LCD_WR_DATA8(0x12);   

LCD_WR_REG(0xC4);
LCD_WR_DATA8(0x20);  

LCD_WR_REG(0xC6); 
LCD_WR_DATA8(0x0F);    

LCD_WR_REG(0xD0); 
LCD_WR_DATA8(0xA4);
LCD_WR_DATA8(0xA1);

LCD_WR_REG(0xE0);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2B);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x54);
LCD_WR_DATA8(0x4C);
LCD_WR_DATA8(0x18);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x0B);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x23);

LCD_WR_REG(0xE1);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2C);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x44);
LCD_WR_DATA8(0x51);
LCD_WR_DATA8(0x2F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x20);
LCD_WR_DATA8(0x23);

LCD_WR_REG(0x21); 

LCD_WR_REG(0x11); 
//Delay (120); 

LCD_WR_REG(0x29); 
 
} 

//清屏函数
//Color:要清屏的填充色
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}


//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 
//画一个大点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
}  



//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	Address_set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//color:颜色
//num:数值(0~4294967295);	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 
//显示2个数字
//x,y:起点坐标
//num:数值(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}




































