/*
 * Pattern.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#ifndef TT_H
#define TT_H

#include "stm32l4xx_hal.h"
#include <stdlib.h>
#include <time.h>

void WaitForStart();
void GeneratePattern();
void LED_Status(int index, int state);
void BlinkLED(int led_status);
void ProcessPattern();
void CheckInput();
uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#endif /* TT_H */

