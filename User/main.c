#include "stm32f10x.h"
#include "DHT11.h"
#include "OLED.h"
#include "LED.h"
#include "Delay.h"
#include "AD.h"
#include "PWM.h"
#include "Motor.h"
#include "Servo.h"
#include "sys.h"
#include "usart.h"
#include "esp8266.h"
#include "MqttKit.h"
#include "onenet.h"
#include <stdio.h>
#include <string.h>
#include "cJSON.h"


int fputc(int ch, FILE *f)
{
	// 向USART1发送数据
	USART_SendData(USART1, (unsigned char)ch);
	// 等待发送完成
	while (!(USART1->SR & USART_FLAG_TXE));
	// 返回发送的数据
	return (ch);
}

int main(void)
{
	uint8_t DHT_Buffer[5];	// 存储DHT11传感器的数据
	char Temp[20];	// 定义字符数组Temp，存储温度数据
	char Humi[20];	// 存储湿度数据
	
	unsigned short timeCount = 0;	

	char MQ135_Buffer[5];	// 存储MQ135传感器的数据
	uint16_t ADValue;	// 存储ADC的值
	float MQ135Value = 0;	// 存储MQ135的PPM值


	/*外设初始化*/
	OLED_Init(); //	OLED
	AD_Init();	// ADC
	Motor_Init();	//电机
	Servo_Init();	//舵机
	LED_Init();		// LED
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// NVIC优先级分组
	Usart1_Init(115200);	// 串口1波特率
	Usart2_Init(115200);	// 串口2波特率
	ESP8266_Init();		// ESP8266

	OLED_ShowString(2,1,"ServerConnect...");	// 显示连接服务器字符，直至连接成功

	unsigned char *dataPtr = NULL;
	const char *topics[] = {"STM32IOT/FAN","STM32IOT/LED"};	// 订阅主题名，用于接收控制指令
	while(OneNet_DevLink())	// 循环等待与服务器连接成功
	Delay_ms(500);
	OneNet_Subscribe(topics, 2);	//订阅主题，个数
	OLED_Clear();	//清屏
	while (1)
	{
			OLED_ShowString(1,2,"Air Detector");	//第一行显示固定字符串
			DHT11_GetData(DHT_Buffer);	// DHT11获取数据
			sprintf(Temp, "Temp:%d.%d", DHT_Buffer[2], DHT_Buffer[3]);	// 将温度数据转换为字符串
			sprintf(Humi, "Humi:%d.%d", DHT_Buffer[0], DHT_Buffer[1]);	// 将湿度数据转换为字符串

			OLED_ShowString(1, 2, (char *)Temp);	// 在OLED屏幕上显示温度
			OLED_ShowString(2, 2, (char *)Humi);	//显示湿度

			ADValue = AD_GetAverage(ADC_Channel_1, 20);		 // 获取ADC的值
			MQ135Value = (float)ADValue / 4095 * 3.3 * 1000; // 将ADC值转换为MQ135的PPM值
			sprintf(MQ135_Buffer, "MQ:%.2f", MQ135Value);	 // 将MQ135的PPM值转换为字符串
			OLED_ShowString(3, 2, (char *)MQ135_Buffer);	 // 在OLED屏幕上显示MQ135的PPM值
			Delay_ms(500);

			UsartPrintf(USART_DEBUG,"Temp:%d.%d,Humi:%d.%d,MQ:%.2f \r\n",DHT_Buffer[2], DHT_Buffer[3],DHT_Buffer[0], DHT_Buffer[1],MQ135Value);		//串口打印传感器信息

			if (DHT_Buffer[2] >= 35 || DHT_Buffer[0] >= 80 || MQ135Value >= 150)	//满足任一条件则开启电机排气
			{
				MotorON();
				Servo_SetAngle(90);
			}
			else
			{
				MotorOFF();
				Servo_SetAngle(0);
			}

	if(++timeCount >= 500)								
			{
			//上传传感器数值
			OneNet_Publish("STM32IOT/Temp",(char *)Temp);
			OneNet_Publish("STM32IOT/Humi",(char *)Humi);
			OneNet_Publish("STM32IOT/MQ135",MQ135_Buffer);
			timeCount = 0;
			ESP8266_Clear();	//清理缓存

		}		
			//网络数据处理
			dataPtr = ESP8266_GetIPD(3);
			if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);	//平台返回数据检测
			Delay_ms(2000);
	}
}


