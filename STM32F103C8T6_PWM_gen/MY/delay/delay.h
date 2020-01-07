#ifndef __DELAY_H
#define __DELAY_H
#include "sys.h"

void delay_init();	 
void delay(int i);  
/*延时：1us的i倍*/
void delay_us(int i);       
/*等待一个按键，我的主板是用P2.0与GND之间接一个按键*/
//void delay_ms(int i);   
void delay_ms(uint16_t nms);
#endif
