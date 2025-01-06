#include "dht11.h"
#include "stm32f1xx_hal.h"

static void DHT11_SetPinOutput(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

static void DHT11_SetPinInput(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

void DHT11_Init(void) {
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
}

uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0};
    uint32_t last_tick;

    // Start signal
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(18); // Pull low for at least 18 ms
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    DHT11_SetPinInput();

    // Wait for DHT11 response
    last_tick = HAL_GetTick();
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) {
        if (HAL_GetTick() - last_tick > 2) return 1; // Timeout
    }
    last_tick = HAL_GetTick();
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET) {
        if (HAL_GetTick() - last_tick > 2) return 1; // Timeout
    }
    last_tick = HAL_GetTick();
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) {
        if (HAL_GetTick() - last_tick > 2) return 1; // Timeout
    }

    // Read 40 bits data
    for (int i = 0; i < 40; i++) {
        // Wait for the start of the bit
        last_tick = HAL_GetTick();
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET) {
            if (HAL_GetTick() - last_tick > 2) return 1; // Timeout
        }
        // Measure the duration of the high pulse
        last_tick = HAL_GetTick();
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) {
            if (HAL_GetTick() - last_tick > 2) return 1; // Timeout
        }
        if ((HAL_GetTick() - last_tick) > 40) {
            data[i / 8] |= (1 << (7 - (i % 8))); // High pulse duration > 40us -> bit = 1
        }
    }

    // Verify checksum
    if (data[4] != (data[0] + data[1] + data[2] + data[3])) return 1;

    *humidity = data[0];
    *temperature = data[2];
    return 0;
}
