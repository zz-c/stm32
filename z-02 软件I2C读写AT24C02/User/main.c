#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2c.h"

unsigned int Num;
#define AT24C02_ADDRESS		0xA0

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	MyI2C_Init();
	
	MyI2C_Start();
	MyI2C_SendByte(0xA0);
	uint8_t ACK = MyI2C_ReceiveByte();
	MyI2C_Stop();
	
	OLED_ShowNum(1, 1, ACK, 3);
	
	MyI2C_Start();
	MyI2C_SendByte(0xA0);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x03);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	
	Delay_ms(1000);

	
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0);
	MyI2C_ReceiveAck();
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS|0x01);
	MyI2C_ReceiveAck();
	Num=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	OLED_ShowNum(2, 1, Num, 3);
	while (1)
	{
		
	}
}
