/*
 * Pattern.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */
#include "main.h"

uint32_t pattern_open[MAX_PATTERN] = {1, 2, 3};
uint32_t pattern_close[MAX_PATTERN] = {3, 2, 1};
uint8_t pattern_index = 0;
uint8_t setting_mode = 0;

void ProcessButtonPress(uint8_t button) {
    if (setting_mode == 1) {
        pattern_open[pattern_index++] = button;
        if (pattern_index >= 3) {
            HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
            HAL_Delay(300);
            HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
            setting_mode = 2;
            pattern_index = 0;
        }
    } else if (setting_mode == 2) {
        pattern_close[pattern_index++] = button;
        if (pattern_index >= 3) {
            HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
            HAL_Delay(300);
            HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_RESET);
            setting_mode = 0;
        }
    } else {
        if (pattern_index < 3) {
            if (button == pattern_open[pattern_index]) {
                pattern_index++;
                if (pattern_index == 3) {
                    HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_RESET);
                }
            } else if (button == pattern_close[pattern_index]) {
                pattern_index++;
                if (pattern_index == 3) {
                    HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
                }
            } else {
                pattern_index = 0;
            }
        }
    }
}

void CheckButtonPress() {
    static uint32_t last_press_time = 0;
    static uint8_t hold_count = 0;

    uint32_t current_time = HAL_GetTick();

    if (current_time - last_press_time > 2000) {
        if (HAL_GPIO_ReadPin(GPIOA, OP1_Pin) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA, OP3_Pin) == GPIO_PIN_SET) {
            hold_count++;
            if (hold_count >= 5000) {
                setting_mode = 1;
                HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
                HAL_Delay(300);
                HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
                hold_count = 0;
            }
        } else {
            hold_count = 0;
        }
        if (HAL_GPIO_ReadPin(GPIOA, OP1_Pin) == GPIO_PIN_SET) {
            ProcessButtonPress(1);
        }
        if (HAL_GPIO_ReadPin(GPIOA, OP2_Pin) == GPIO_PIN_SET) {
            ProcessButtonPress(2);
        }
        if (HAL_GPIO_ReadPin(GPIOA, OP3_Pin) == GPIO_PIN_SET) {
            ProcessButtonPress(3);
        }

        last_press_time = current_time;
    }
}

void control_LED_BLINK(GPIO_TypeDef *PORT, uint16_t PIN, uint8_t state, uint32_t interval) {
    static uint32_t previousMillis = 0;
    uint32_t currentMillis = HAL_GetTick();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        HAL_GPIO_TogglePin(PORT, PIN);
    }
}





