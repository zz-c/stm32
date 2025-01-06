#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "dht11.h"
uint8_t temperature = 0, humidity = 0;

int main(void)
{
	OLED_Init();		//OLED初始化
	OLED_ShowString(1, 1, "temperature:");
	OLED_ShowString(3, 1, "humidity:");
	
	//SystemInit();         // 初始化系统时钟
	DHT11_Init();         // 初始化 DHT11

	while (1) {
			if (DHT11_Read(&temperature, &humidity) == 0) {
				// 数据读取成功
				OLED_ShowNum(2, 1, temperature, 5);
				OLED_ShowNum(4, 1, humidity, 5);
			} else {
				// 数据读取失败
				OLED_ShowString(2, 1, "fail");
				OLED_ShowString(4, 1, "fail");
			}
			Delay_s(2); // 延时 2 秒
	}
}
