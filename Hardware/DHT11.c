#include "stm32f10x.h" // Device header
#include "Delay.h"

// 向DHT11的数据线发送一个位
void DHT11_DLine(uint8_t BitValue) //
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)BitValue);
}

// 初始化DHT11的GPIO口为输出模式
void DHT11_GPIO_Init_OUT(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 初始化DHT11的GPIO口为输入模式
void DHT11_GPIO_Init_In(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t DHT11_Start(void)
{
    DHT11_GPIO_Init_OUT(); // 设置为输出模式

    DHT11_DLine(0); // 拉低至少18ms
    Delay_ms(20);
    DHT11_DLine(1);                                // 释放总线
    DHT11_GPIO_Init_In();                          // 设置为输入模式
    Delay_us(20);                                  // 等待20-40us
    if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) // 检查DHT11响应
    {
        while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
            ;
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
            ;
        return 1;
    }
    return 0;
}

uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, temp;
    for (uint8_t i = 0; i < 8; i++)
    {
        temp <<= 1;
        while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
            ;
        Delay_us(28);
        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET ? (temp |= 0x01) : (temp &= ~0x01);
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
            ;
    }
    return temp;
}

uint8_t DHT11_GetData(uint8_t buffer[])
{
    if (DHT11_Start())
    {
        buffer[0] = DHT11_Read_Byte();
        buffer[1] = DHT11_Read_Byte();
        buffer[2] = DHT11_Read_Byte();
        buffer[3] = DHT11_Read_Byte();
        buffer[4] = DHT11_Read_Byte();
    }
    return (buffer[0] + buffer[1] + buffer[2] + buffer[3] == buffer[4]) ? 1 : 0;
}
