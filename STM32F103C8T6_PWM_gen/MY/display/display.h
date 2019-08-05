#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "sys.h"
#include "stdlib.h"	   
#define LCD_W 240
#define LCD_H 240
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
	

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý


void display_Ch_Fre_Duty(u8 ch,u16 freq,u8 duty);
void Load_Drow_Dialog(void);
void xianshi();

#endif
