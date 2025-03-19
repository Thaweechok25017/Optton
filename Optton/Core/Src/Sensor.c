/*
 * Sensor.c
 *
 *  Created on: Mar 18, 2025
 *      Author: Thaweechock.M
 */
#include "Sensor.h"

void ReadValue(uint8_t OP) {
	if(OP == 1){return HAL_GPIO_ReadPin(GPIOA, OP1_Pin);}
	else if(OP == 2){return HAL_GPIO_ReadPin(GPIOA, OP2_Pin);}
	else if (OP == 3){return HAL_GPIO_ReadPin(GPIOA, OP3_Pin);}
}
