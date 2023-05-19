#ifndef _WS2812B_H_
#define _WS2812B_H_

#include "stm32f4xx.h"

#define PORT_RCC    RCC_AHB1Periph_GPIOA
#define DATA_PORT   GPIOA
#define DATA_PIN    GPIO_Pin_5

void ws281x_init(void);
void ws281x_sendBit(uint32_t data);
void ws281x_sendLow(void);
void ws281x_sendHigh(void);
void ws281x_Reset(void);
void ws281x_delay(unsigned int delay_num);

#endif