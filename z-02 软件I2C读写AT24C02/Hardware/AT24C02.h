#ifndef __AT24C02_H__
#define __AT24C02_H__

void AT24C02_WriteByte(uint8_t WordAddress,uint8_t data);
uint8_t AT24C02_ReadByte(uint8_t WordAddress);

#endif
