/*
 * Pattern.h
 *
 *  Created on: Mar 19, 2025
 *      Author: Thaweechock.M
 */

#ifndef INC_PATTERN_H_
#define INC_PATTERN_H_

#include "main.h"

#define MAX_PATTERN 3
#define FLASH_STORAGE_ADDRESS 0x0801F800  // ปรับตามพื้นที่ Flash ที่ต้องการใช้

extern uint32_t pattern_open[MAX_PATTERN];
extern uint32_t pattern_close[MAX_PATTERN];
extern uint8_t pattern_index;
extern uint8_t setting_mode;

void LoadPatternsFromFlash();
void SavePatternsToFlash();
void ProcessButtonPress(uint8_t button);
void CheckButtonPress();

#endif /* INC_PATTERN_H_ */
