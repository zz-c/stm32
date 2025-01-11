#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2c.h"
#include "AT24C02.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();
	MyI2C_Init();
	
	// 测试地址
	MyI2C_Start();
	MyI2C_SendByte(0xA0);
	uint8_t ACK = MyI2C_ReceiveByte();
	MyI2C_Stop();
	OLED_ShowNum(1, 1, ACK, 3);
	
	// 测试读写
	AT24C02_WriteByte(0, 'z');
	Delay_ms(1000);
	uint8_t readData = AT24C02_ReadByte(0);
	OLED_ShowChar(2, 1, readData);
	while (1)
	{
		
	}
}
