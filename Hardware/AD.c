#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void AD_Init(void)
{
	//开启RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//ADC分频器

	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	//AD初始化
	ADC_DeInit(ADC1);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//设置为单次转换模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//不开启扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1;		//顺序进行规则转化的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);	//使能AD
	
	ADC_ResetCalibration(ADC1);	//使能复位校准 
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//等待复位校准结束
	ADC_StartCalibration(ADC1);//开启AD校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);//等待校准结束
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADC通道,采样时间为239.5周期	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回ADC1规则组的转换结果
}

uint16_t AD_GetAverage(uint8_t ADC_Channel,uint8_t Times)	//获取ADC采样平均值
{
	uint32_t Temp_Value=0;
	uint8_t i;
	for(i=0; i < Times;i++)
	{
		Temp_Value += AD_GetValue(ADC_Channel);
		Delay_ms(5);
	}
	return Temp_Value/Times;
}

