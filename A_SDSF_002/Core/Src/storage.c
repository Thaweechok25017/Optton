/*
 * storage.c
 *
 *  Created on: Nov 13, 2024
 *      Author: Chanchai.Th
 */
#include "storage.h"

void Flash_ErasePage(uint32_t address) {
	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError = 0;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page = (address - 0x08000000) / FLASH_PAGE_SIZE;  // Calculate the page number
	EraseInitStruct.Banks = FLASH_BANK_1;
	EraseInitStruct.NbPages = 1;

	// Perform the erase operation
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

	HAL_FLASH_Lock();
}

void Flash_WriteArray(uint32_t address, uint32_t *data, uint32_t length) {

	// Always erase before flash
	Flash_ErasePage(address);

	HAL_FLASH_Unlock();

	for (uint32_t i = 0; i < length; i++) {
		// Prepare 64-bit data by padding 32-bit values to 64-bit format
		uint64_t data64 = (uint64_t) data[i];

		// Write the double word (64-bit) to Flash
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + (i * 8), data64) != HAL_OK) {
			// Handle error
			break;
		}
	}

	HAL_FLASH_Lock();
}

void Flash_ReadArray(uint32_t address, uint32_t *data, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		// Read each 32-bit value from the 8-byte aligned addresses
		data[i] = *(uint32_t*) (address + (i * 8));
	}
}

void ReadOpenPattern(uint32_t *data) {
	Flash_ReadArray(FLASH_OPEN_PATTERN_ADDR, data, 3);
}

void ReadClosePattern(uint32_t *data) {
	Flash_ReadArray(FLASH_CLOSE_PATTERN_ADDR, data, 3);
}

void ReadServoPosition(uint32_t *data) {
	Flash_ReadArray(FLASH_SERVO_POS_ADDR, data, 1);
}
void ReadTimeOutOpen(uint32_t *data) {
	Flash_ReadArray(FLASH_TIMEOUT_OPEN_ADDR, data, 1);
}
void WriteTimeOutOpen(uint32_t *data) {
	Flash_WriteArray(FLASH_TIMEOUT_OPEN_ADDR, data, 1);
}
void WriteOpenPattern(uint32_t *data) {
	Flash_WriteArray(FLASH_OPEN_PATTERN_ADDR, data, 3);
}

void WriteClosePattern(uint32_t *data) {
	Flash_WriteArray(FLASH_CLOSE_PATTERN_ADDR, data, 3);
}

void WriteServoPosition(uint32_t *data) {
	Flash_WriteArray(FLASH_SERVO_POS_ADDR, data, 1);
}

void EraseStorage() {
	Flash_ErasePage(FLASH_OPEN_PATTERN_ADDR);
	Flash_ErasePage(FLASH_CLOSE_PATTERN_ADDR);
}
