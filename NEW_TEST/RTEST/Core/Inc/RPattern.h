/*
 * RPattern.h
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#ifndef INC_RPATTERN_H_
#define INC_RPATTERN_H_

#include "stm32l4xx_hal.h"

void GenerateAndPrintPattern();
void Starttrig();
void Modrun(int hold_detected);

#endif /* INC_RPATTERN_H_ */
