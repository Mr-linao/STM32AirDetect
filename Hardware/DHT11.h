#ifndef __DHT11_H
#define __DHT11_H

void DHT11_DLine(uint8_t BitValue);

void DHT11_GPIO_Init_OUT(void);

void DHT11_GPIO_Init_In(void);

uint8_t DHT11_Start(void);

uint8_t DHT11_Read_Byte(void);

uint8_t DHT11_GetData(uint8_t buffer[]);

#endif
