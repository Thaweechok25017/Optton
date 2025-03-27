/*
 * Pattern.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */
#include "TT.h"
#include "main.h"

int pattern[3];
int step = 0;
int round_flag = 0;
int started = 0; // ใช้ตรวจสอบว่ากดปุ่ม OP1 & OP3 ค้าง 5 วินาทีแล้วหรือยัง

void WaitForStart() {
    uint32_t start_time = 0;

    LED_Status(1, 1);
    LED_Status(3, 1);

    while (!started) {
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) {
            if (start_time == 0) {
                start_time = HAL_GetTick();
            }
            if (HAL_GetTick() - start_time >= 5000) {
                started = 1;
                LED_Status(1, 0);
                LED_Status(3, 0);
                GeneratePattern();
            }
        } else {
            start_time = 0;
        }
    }
}


void GeneratePattern() {
    int patterns[12][3] = {
        {1, 2, 1}, {1, 2, 3}, {1, 3, 1}, {1, 3, 2},
        {2, 1, 2}, {2, 1, 3}, {2, 3, 1}, {2, 3, 2},
        {3, 1, 2}, {3, 1, 3}, {3, 2, 1}, {3, 2, 3}
    };
    srand(HAL_GetTick());
    int index = rand() % 12;
    for (int i = 0; i < 3; i++) {
        pattern[i] = patterns[index][i];
    }
    step = 0;
}

void LED_Status(int index, int state) {
    GPIO_TypeDef *port;
    uint16_t pin;

    switch (index) {
        case 1: port = GPIOC; pin = GPIO_PIN_10; break;
        case 2: port = GPIOC; pin = GPIO_PIN_11; break;
        case 3: port = GPIOC; pin = GPIO_PIN_12; break;
        default: return;
    }
    HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BlinkLED(int led_status) {
    GPIO_TypeDef *port = GPIOC;
    uint16_t pin = (led_status == 0) ? GPIO_PIN_8 : GPIO_PIN_7; // 0 = LED_on, 1 = LED_off

    for (int i = 0; i < 2; i++) {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        HAL_Delay(500);
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        HAL_Delay(500);
    }
}

void ProcessPattern() {
    for (int i = 0; i < 3; i++) {
        LED_Status(pattern[i], 1);
        while (!ReadButton(GPIOC, (pattern[i] == 1 ? GPIO_PIN_0 : pattern[i] == 2 ? GPIO_PIN_1 : GPIO_PIN_2)));
        LED_Status(pattern[i], 0);
    }

    if (round_flag == 0) {
        HAL_GPIO_WritePin(GPIOC, LED_on_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(GPIOC, LED_on_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
    }
    round_flag = !round_flag;
    GeneratePattern();
}

void CheckInput() {
    int pressed = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) ||
                  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) ||
                  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);

    if (pressed) {
        int correct_op = (pattern[step] == 1) ? GPIO_PIN_0 :
                         (pattern[step] == 2) ? GPIO_PIN_1 : GPIO_PIN_2;

        if (HAL_GPIO_ReadPin(GPIOC, correct_op)) {
            ProcessPattern();
        } else {
            BlinkLED(round_flag);
        }
    }
}

uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    uint32_t start_time = HAL_GetTick();

    while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) {
        if (HAL_GetTick() - start_time >= 500) {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET);  // รอจนปล่อยปุ่ม
            return 1;
        }
    }
    return 0;
}
