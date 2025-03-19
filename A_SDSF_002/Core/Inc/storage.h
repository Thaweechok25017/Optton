/*
 * storage.h
 *
 *  Created on: Nov 13, 2024
 *      Author: Chanchai.Th
 */

#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#include "main.h"

#define FLASH_OPEN_PATTERN_ADDR   	0x08060000
#define FLASH_CLOSE_PATTERN_ADDR   	0x08060800
#define FLASH_SERVO_POS_ADDR   		0x08061000
#define FLASH_TIMEOUT_OPEN_ADDR   	0x08061800

void Flash_WriteArray(uint32_t address, uint32_t *data, uint32_t length);

void Flash_ReadArray(uint32_t address, uint32_t *data, uint32_t length);

void Flash_ErasePage(uint32_t address);

void ReadOpenPattern(uint32_t *data);

void ReadClosePattern(uint32_t *data);

void ReadServoPosition(uint32_t *data);

void WriteOpenPattern(uint32_t *data);

void WriteClosePattern(uint32_t *data);

void WriteServoPosition(uint32_t *data);

void ReadTimeOutOpen(uint32_t *data);

void WriteTimeOutOpen(uint32_t *data);

void EraseStorage(void);

#endif /* INC_STORAGE_H_ */
