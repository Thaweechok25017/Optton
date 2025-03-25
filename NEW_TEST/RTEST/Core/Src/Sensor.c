/*
 * Sensor.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#include "Sensor.h"

GPIO_PinState switchState[3];

GPIO_PinState readSwitchState(uint16_t GPIO_Pin) {
    return HAL_GPIO_ReadPin(GPIOC, GPIO_Pin);
}

void checkPatternMatch(int output[3]) {
    switchState[0] = readSwitchState(OP1_Pin);
    switchState[1] = readSwitchState(OP2_Pin);
    switchState[2] = readSwitchState(OP3_Pin);

    for (int i = 0; i < 3; i++) {
        if ((switchState[i] == GPIO_PIN_SET && output[i] == 0) || (switchState[i] == GPIO_PIN_RESET && output[i] == 1)) {
            return 0;  // ไม่ตรงกัน
        }
    }

    return 1;  // ถ้าตรงกันทั้งหมด
}

