#ifndef __DELAY_H
#define __DELAY_H
#include "sys.h"

void delay_init();	 
void delay(int i);  
/*��ʱ��1us��i��*/
void delay_us(int i);       
/*�ȴ�һ���������ҵ���������P2.0��GND֮���һ������*/
//void delay_ms(int i);   
void delay_ms(uint16_t nms);
#endif
