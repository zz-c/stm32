#include "MyI2c.h"

#define AT24C02_ADDRESS		0xA0

/**
  * @brief  AT24C02写入一个字节
  * @param  WordAddress 要写入字节的地址
  * @param  Data 要写入的数据
  * @retval 无
  */
void AT24C02_WriteByte(unsigned char WordAddress,unsigned char Data)
{
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

/**
  * @brief  AT24C02读取一个字节
  * @param  WordAddress 要读出字节的地址
  * @retval 读出的数据
  */
unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress);
	MyI2C_ReceiveAck();
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS|0x01);
	MyI2C_ReceiveAck();
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	return Data;
}
