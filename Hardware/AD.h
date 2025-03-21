#ifndef __ADC_H
#define __ADC_H


void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
uint16_t AD_GetAverage(uint8_t ADC_Channel,uint8_t Times);
#endif
