#include "stm32f10x.h"
#include "vios.h"


//u16 ADC_SimpleConvertValue[4];


/******AD input Convert The par is ADC_Channel_x***********/
u16 Get_Adc_Back(u8 ch) 
{
//Set ADC regerular channel
ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );  ////  
ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //
while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//
return ADC_GetConversionValue(ADC1);  //
}
