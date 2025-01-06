#ifndef DHT11_H
#define DHT11_H

#include "stm32f1xx_hal.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

void DHT11_Init(void);
uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity);

#endif
