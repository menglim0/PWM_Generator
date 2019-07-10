
/*  -------------------------Includes-----------------------------------------*/
#include "My_InitTask.h"  

#define ADC1_DR_Address    ((u32)0x4001244C)

unsigned int AD_value;
unsigned int AD_value_group[2];



#define  N   50      //The sample freq is 50 
#define  M  12      //12 adc channel

/* Private variables ---------------------------------------------------------*/


vu16 ADC_ConvertedValue[4];
ErrorStatus HSEStartUpStatus;


/*  -------------------------Functions-----------------------------------------*/
/**** stm32�ĳ�ʼ������ȫ�����������  *****/
void My_InitTask(void)
{
   /* Configure the system clocks */
  	RCC_Configuration();
  	SysTick_Configuration();
    
  /* NVIC Configuration */
  	//NVIC_Configuration();

 }



void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
   
  /* Enable GPIOB, GPIOF and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF | 
                         RCC_APB2Periph_AFIO, ENABLE);
	
	/* Enable DMA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
}


/*void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
  // Configure PC. as Output push-pull 
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_Init(GPIOF, &GPIO_InitStructure);     
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
}
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO,ENABLE);	
															//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;             //����LED D2�˿ڹҽӵ�PG14�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOC, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;             //����LED D2�˿ڹҽӵ�PG14�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//���ù����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������

	/*  ��������˿�*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; 			//����LED E0�˿ڹҽӵ�0�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   		//���ù�����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOE, &GPIO_InitStructure);				   	//���˿�GPIOE���г�ʼ������
	 
	 //PA0/1/2 ??????????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//void NVIC_Configuration(void)
//{
// 
// //NVIC_InitTypeDef NVIC_InitStructure;
//  
//#ifdef  VECT_TAB_RAM  
//  /* Set the Vector Table base location at 0x20000000 */ 
//  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
//#else  /* VECT_TAB_FLASH  */
//  /* Set the Vector Table base location at 0x08000000 */ 
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
//#endif

//  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//  
//}

void SysTick_Configuration(void)
{
  /* Select AHB clock(HCLK) as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

}


void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//����ģʽ  CR1
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;						    //ɨ��ģʽʹ��
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//����ת��ģʽʹ��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//����ת�����ⲿ�¼�--��  CR2
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ת����������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;								//ת����ͨ����Ϊ1
  ADC_Init(ADC1, &ADC_InitStructure);

  //ADC_TempSensorVrefintCmd(ENABLE);
  /* ADC1 regular channel14 configuration */ 
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );

	/* Enable ADC1 DMA */															  
  ADC_DMACmd(ADC1, DISABLE);		                          						  //����ADC1����DMA����
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);														  //ʹ��ADC1

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);													  //����ADC1��λУ׼�Ĵ��� 
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));									  //���У׼�Ĵ����Ƿ�λ��� 

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);													  //����ADC1 У׼
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));									      //���У׼�Ƿ���� 
     
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC1, ENABLE);										  //�����������ADC1ת��

}

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* DMA channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 6;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
}


void AD_ConvertFunction(void)
{
	AD_value=ADC_GetConversionValue(ADC1);
 	//while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		AD_value_group[0] = ADC_ConvertedValue[0];
		AD_value_group[1] = ADC_ConvertedValue[1];
}











