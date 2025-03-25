/*
 * Pattern.c
 *
 *  Created on: Mar 19, 2025
 *      Author: Thaweechock.M
 */
#include "main.h"

uint32_t pattern_open[MAX_PATTERN] = {1, 2, 3};
uint32_t pattern_close[MAX_PATTERN] = {3, 2, 1};
uint8_t pattern_index = 0;
uint8_t setting_mode = 0;

void LoadPatternsFromFlash() {
    Flash_ReadArray(FLASH_STORAGE_ADDRESS, pattern_open, MAX_PATTERN);
    Flash_ReadArray(FLASH_STORAGE_ADDRESS + 64, pattern_close, MAX_PATTERN);

    if (pattern_open[0] == 0xFFFFFFFF || pattern_close[0] == 0xFFFFFFFF) {
        pattern_open[0] = 1; pattern_open[1] = 2; pattern_open[2] = 3;
        pattern_close[0] = 3; pattern_close[1] = 2; pattern_close[2] = 1;
    }
}

void SavePatternsToFlash() {
    Flash_WriteArray(FLASH_STORAGE_ADDRESS, pattern_open, MAX_PATTERN);
    Flash_WriteArray(FLASH_STORAGE_ADDRESS + 64, pattern_close, MAX_PATTERN);
}

void ProcessButtonPress(uint8_t button) {
    if (setting_mode == 1) {
        pattern_open[pattern_index++] = button;
        if (pattern_index >= 3) {
            SavePatternsToFlash();
            HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_SET);
            HAL_Delay(300);
            HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_RESET);
            setting_mode = 2;
            pattern_index = 0;
        }
    } else if (setting_mode == 2) {
        pattern_close[pattern_index++] = button;
        if (pattern_index >= 3) {
            SavePatternsToFlash();
            HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_SET);
            HAL_Delay(300);
            HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_RESET);
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
    if (current_time - last_press_time > 1000) {
        if (HAL_GPIO_ReadPin(GPIOA, OP1_PIN) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA, OP3_PIN) == GPIO_PIN_SET) {
            hold_count++;
            if (hold_count >= 5000) {
                setting_mode = 1;
                HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_SET);
                HAL_Delay(300);
                HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_RESET);
                hold_count = 0;
            }
        } else {
            hold_count = 0;
        }
        if (HAL_GPIO_ReadPin(GPIOA, OP1_PIN) == GPIO_PIN_SET) {
            ProcessButtonPress(1);
        }
        if (HAL_GPIO_ReadPin(GPIOA, OP2_PIN) == GPIO_PIN_SET) {
            ProcessButtonPress(2);
        }
        if (HAL_GPIO_ReadPin(GPIOA, OP3_PIN) == GPIO_PIN_SET) {
            ProcessButtonPress(3);
        }

        last_press_time = current_time;
    }
}



