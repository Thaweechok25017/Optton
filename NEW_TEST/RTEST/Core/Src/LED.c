/*
 * LED.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#include "LED.h"

uint32_t pressStartTime = 0;
uint32_t debounceTime = 1000;

void LED_Status(uint8_t num1, uint8_t num2, uint8_t num3) {
    HAL_GPIO_WritePin(GPIOC, LED_1_Pin, num1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_2_Pin, num2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_3_Pin, num3 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void LEDOF(uint8_t son, uint8_t soff) {
    HAL_GPIO_WritePin(GPIOC, LED_on_Pin, son ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_off_Pin, soff ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BlinkLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t times, uint32_t delay_ms) {
	if (GPIOx == NULL) return;
		for (uint8_t i = 0; i < times; i++) {
			HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
			HAL_Delay(delay_ms);
			HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
			HAL_Delay(delay_ms);
    }
}

void CheckLP(const int output[3], const int input[3]){
	for (int i = 0; i < 12; i++) {
	        if (output[0] == input[0] && output[1] == input[1] && output[2] == input[2]) {
	            LED_Status(input[0], input[1], input[2]);
	            break;
	        }
	    }
}
void LPattern(int Linput[3]){
	LED_Status(Linput[0], Linput[1], Linput[2]);

	for (int t = 0; t < 3; t++) {
	        if ((Sensor[t] == 1) && (Linput[t] == 1)) {
	            if (pressStartTime[t] == 0) {
	                pressStartTime[t] = HAL_GetTick();
	            }
	            if (HAL_GetTick() - pressStartTime[t] >= debounceTime) {
	                switch (t) {
	                    case 0:
	                        HAL_GPIO_WritePin(GPIOC, LED_1_Pin, GPIO_PIN_RESET);  // LED_1 ดับ
	                        break;
	                    case 1:
	                        HAL_GPIO_WritePin(GPIOC, LED_2_Pin, GPIO_PIN_RESET);  // LED_2 ดับ
	                        break;
	                    case 2:
	                        HAL_GPIO_WritePin(GPIOC, LED_3_Pin, GPIO_PIN_RESET);  // LED_3 ดับ
	                        break;
	                }
	                pressStartTime[t] = 0;  // รีเซ็ตเวลา
	            }
	        } else {
	            pressStartTime[t] = 0;  // รีเซ็ตเวลาเมื่อไม่ได้กดปุ่ม
	        }
	    }
}


