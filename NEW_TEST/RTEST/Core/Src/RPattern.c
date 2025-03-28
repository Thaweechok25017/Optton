/*
 * RPattern.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */
#include "RPattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint8_t hold_detected = 0;
uint8_t pattern_index = 0;  // ใช้ในการเลือก pattern จาก 12 patterns
int output[3];  // เก็บ patterns ที่สุ่ม

void Modrun(int hold_detected){
    switch(hold_detected){
        case 0:
        		checkPatternMatch();
            break;
        case 1:
    			GenerateAndPrintPattern();
    			checkPatternMatch();
    			LEDOF(1,0);
            break;
    }
}

void Starttrig(){
	LED_Status(0,0,0);
	LED_Status(1,0,1);
    uint32_t current_time = HAL_GetTick();
    static uint32_t hold_start_time = 0;

    if (HAL_GPIO_ReadPin(GPIOC, OP1_Pin) && HAL_GPIO_ReadPin(GPIOC, OP3_Pin)) {
        if (hold_start_time == 0) {
            hold_start_time = current_time;
        }

        if (current_time - hold_start_time >= 5000 && !hold_detected) {
            LED_Status(0,0,0);

            if(checkPatternMatch(output)){
                hold_detected = 1;
            }
            else{
            	BlinkLED(GPIOC,LED_on_Pin,2,500);
                hold_detected = 0;
            }

        }
    } else {
        hold_start_time = 0;
        hold_detected = 0;
    }
}

void GenerateAndPrintPattern() {
    int patterns[12][3] = {
        {1, 2, 1}, {1, 2, 3}, {1, 3, 1}, {1, 3, 2},
        {2, 1, 2}, {2, 1, 3}, {2, 3, 1}, {2, 3, 2},
        {3, 1, 2}, {3, 1, 3}, {3, 2, 1}, {3, 2, 3}
    };

    srand(time(NULL));
    int index = rand() % 12;
    for (int j = 0; j < 3; j++) {
        output[j] = patterns[index][j];
        printf("Pattern: %d%d%d\n", output[0], output[1], output[2]);
    }

}

