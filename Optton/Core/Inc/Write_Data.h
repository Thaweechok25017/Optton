#ifndef __WRITE_DATA_H__
#define __WRITE_DATA_H__

#include "main.h"  // This includes HAL and GPIO definitions

// Define MCP_ADDR as per your application
#define MCP_ADDR 0x40  // Replace this with the correct address if needed

// Function prototype
void Write_data(uint8_t reg, uint8_t Value);

#endif /* __WRITE_DATA_H__ */
