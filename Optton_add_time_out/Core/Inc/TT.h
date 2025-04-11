/*
 * TT.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#ifndef TT_H
#define TT_H

#include "stm32g0xx_hal.h"
#include <stdlib.h>
#include <time.h>

void WaitForStart();
void GeneratePattern();
void LED_Status(int index, int state);
void BlinkLED(int led_status);
void ProcessPattern();
void CheckInput();
uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void MCP23S17_Write(uint8_t reg, uint8_t data);
uint8_t MCP23S17_Read(uint8_t reg);
void MCP23S17_Init();
void TurnOffLightB();
void TurnOnLightB();
void MCP23S17_Init();
void Send_data();
void Send_dataA();
void Send_dataB();
void MCP23S17_Init();
void Off_led_status();
void On_led_status();

#endif /* TT_H */
