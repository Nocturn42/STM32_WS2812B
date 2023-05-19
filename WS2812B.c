#include "WS2812B.h"

void ws281x_init(void)
{
    RCC_AHB1PeriphClockCmd(PORT_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DATA_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(DATA_PORT, DATA_PIN);
}

void ws281x_sendBit(uint32_t data)
{
    uint8_t i;
    for (i = 0; i < 24; i++) {
        if (((data >> i) & 0x01) == 1) {
            ws281x_sendHigh();
        }
        else {
            ws281x_sendLow();
        }
    }
}

void ws281x_sendLow(void)
{
    // High 0.4 us, Low 0.85 us
    DATA_PORT->BSRRL = DATA_PIN; // 16Mhz
    __nop();
    __nop();
    DATA_PORT->BSRRH = DATA_PIN;
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
}

void ws281x_sendHigh(void)
{
    // High 0.8 us, Low 0.45 us
    DATA_PORT->BSRRL = DATA_PIN; // 16Mhz
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    DATA_PORT->BSRRH = DATA_PIN;
    __nop();
    __nop();
}

void ws281x_Reset(void)
{
    // Low above 50 us
    DATA_PORT->BSRRH = DATA_PIN;
    ws281x_delay(1000);
}

void ws281x_delay(unsigned int delay_num)
{
    while (delay_num--) {
        __nop();
    }
}
