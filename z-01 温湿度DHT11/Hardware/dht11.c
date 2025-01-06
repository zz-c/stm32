#include "dht11.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_Pin_1

static void DHT11_SetPinOutput(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

static void DHT11_SetPinInput(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

static void delay_us(uint32_t us) {
    SysTick->LOAD = 72 * us; // STM32F103，系统时钟 72 MHz
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL = 0;
}

void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能 GPIOA 时钟
    DHT11_SetPinOutput();
    GPIO_SetBits(DHT11_PORT, DHT11_PIN); // 设置 PA1 高电平
}

uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0};
    uint32_t time = 0;

    // 1. 发送起始信号
    DHT11_SetPinOutput();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN); // 拉低
    delay_us(18000);                       // 延时 18ms
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);   // 拉高
    delay_us(20);                          // 等待 20µs
    DHT11_SetPinInput();                   // 设置为输入模式

    // 2. 检测 DHT11 响应信号
    time = 0;
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 等待引脚拉低
        if (++time > 1000) return 1;                       // 超时
        delay_us(1);
    }
    time = 0;
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 等待引脚拉高
        if (++time > 1000) return 1;                        // 超时
        delay_us(1);
    }
    time = 0;
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 等待引脚再次拉低
        if (++time > 1000) return 1;                       // 超时
        delay_us(1);
    }

    // 3. 接收 40 位数据
    for (int i = 0; i < 40; i++) {
        // 等待低电平开始
        time = 0;
        while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
            if (++time > 1000) return 1; // 超时
            delay_us(1);
        }

        // 测量高电平持续时间
        delay_us(30); // 等待 30µs
        if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
            data[i / 8] |= (1 << (7 - (i % 8))); // 如果高电平超过 30µs，则为 1
        }

        // 等待高电平结束
        time = 0;
        while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
            if (++time > 1000) return 1; // 超时
            delay_us(1);
        }
    }

    // 4. 校验和验证
    if (data[4] != (data[0] + data[1] + data[2] + data[3])) return 1;

    *humidity = data[0];
    *temperature = data[2];
    return 0;
}
