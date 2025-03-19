/*
 * Flash.c
 *
 *  Created on: Mar 19, 2025
 *      Author: Thaweechock.M
 */
#include "flash.h"
#include <stdio.h>

void Flash_ErasePage(uint32_t address) {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError = 0;

    uint32_t page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks = FLASH_BANK_1;
    EraseInitStruct.Page = page;
    EraseInitStruct.NbPages = 1;

    HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    HAL_FLASH_Lock();
}

void Flash_WriteArray(uint32_t address, uint32_t *data, uint32_t length) {
    Flash_ErasePage(address);
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < length; i++) {
        uint64_t data64 = (uint64_t) data[i];
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + (i * 8), data64);
    }

    HAL_FLASH_Lock();
}

void Flash_ReadArray(uint32_t address, uint32_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        data[i] = *(uint32_t*) (address + (i * 8));
    }
}


