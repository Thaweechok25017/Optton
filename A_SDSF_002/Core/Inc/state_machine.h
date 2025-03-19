/*
 * state_machine.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Chanchai.Th
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "stm32l4xx_hal.h"
#include "stdio.h"
#include "stdbool.h"
#include "display.h"
#include "sensors.h"

#define USER_TIMEOUT_1S 1000
#define USER_TIMEOUT_3S 3000
#define USER_TIMEOUT_10S 10000
#define USER_TIMEOUT_30S 30000

typedef enum {
	PATTERN_OPEN, PATTERN_CLOSE
} PatternType;

typedef enum {
	STATE_IDLE, STATE_CLOSE, STATE_OPEN, STATE_CONFIG
} State;

typedef enum {
	INIT, OPEN_BEGIN, OPEN_SELECT, OPEN_DEFAULT, OPEN_CUSTOM, CLOSE_BEGIN, CLOSE_SELECT, CLOSE_DEFAULT, CLOSE_CUSTOM, DONE, SETTING, OPEN_TIME, OPEN_TIME_SELECT, OPEN_TIME_SELECT_CUSTOM, OPEN_TIME_SELECT_INIT, OPEN_TIME_SELECT_NOTIME, EXIT
} CustomState;

void StateMc_Init(TIM_HandleTypeDef *h);

void stateRunning(void);

void goIdleState(void);

void goReadyState(void);

void goCloseState(void);

void goOpenState(void);

void goCustomState(void);

void stateCustomTask(void);

void buttonStateHandler(CustomState state_yes, CustomState state_no);

void buttonStateHandler2(CustomState state_1, CustomState state_2, CustomState state_3);

void showLedSensorIndex(uint32_t index, PatternType patrn);

bool readSensorIndex(uint32_t index, PatternType pt);

void setDefaultPattern(PatternType patrn);

void setDefaultPatternExitCustom(PatternType patrn);

void setCustomPattern(PatternType patrn);


#endif /* INC_STATE_MACHINE_H_ */
