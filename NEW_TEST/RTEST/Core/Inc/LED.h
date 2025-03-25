/*
 * LED.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32l4xx_hal.h"

extern uint32_t pressStartTime[3];  // Array to store press start times for debounce
extern uint32_t debounceTime;

void LED_Status(uint8_t num1, uint8_t num2, uint8_t num3);
void LEDOF(uint8_t son, uint8_t soff);
void BlinkLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t times, uint32_t delay_ms);
void CheckLP(const int output[3], const int input[3]);
void LPattern(int Linput[3]);

#endif /* INC_LED_H_ */
