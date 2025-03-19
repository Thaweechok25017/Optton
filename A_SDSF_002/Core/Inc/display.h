/*
 * display.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Chanchai.Th
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "fonts.h"
#include "main.h"

#define SERVO_CLOSE_VAL 	1000
#define SERVO_OPEN_VAL 		2000
#define SERVO_STEP_INV  	10
#define SERVO_STEP_DELAY  	20

void Oled_Init(void);

void Oled_ShowMessage(char *m, uint16_t x, uint16_t y);

void Oled_ShowMessage_sm(char *m, uint16_t x, uint16_t y);

void Oled_Clear(void);

void Led_Show(bool led1, bool led2, bool led3);

void Led_Custom(uint32_t index, bool status);

void Led_Clear(void);

void Buzzer_Beep(uint32_t delay);

void Buzzer_OK(void);

void Buzzer_OK_L(void);

void Buzzer_NG(void);

void Servo_Init(TIM_HandleTypeDef *htim);

void Servo_Idle(TIM_HandleTypeDef *htim);

void Servo_Open(TIM_HandleTypeDef *htim);

void Servo_Close(TIM_HandleTypeDef *htim);

void Servo_Close_From_Last_Pos(TIM_HandleTypeDef *htim);

#endif /* INC_DISPLAY_H_ */
