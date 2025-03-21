#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void AD_Init(void)
{
	//����RCCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//ADC��Ƶ��

	//����GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	//AD��ʼ��
	ADC_DeInit(ADC1);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����Ϊ����ת��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//������ɨ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;		//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��AD
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼ 
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);//����ADУ׼
	while (ADC_GetCalibrationStatus(ADC1) == SET);//�ȴ�У׼����
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//����ADC1�������ת�����
}

uint16_t AD_GetAverage(uint8_t ADC_Channel,uint8_t Times)	//��ȡADC����ƽ��ֵ
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

