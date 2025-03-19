/*
 * sensors.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Chanchai.Th
 */

#include "sensors.h"

/*
 * ReadOpttonValue
 *
 * This function reads the state of a specific sensor (button) based on the provided sensor number.
 * It reads the GPIO pin corresponding to the requested sensor (1, 2, or 3) and returns the pin state.
 * The function assumes that the buttons are connected to specific GPIO pins defined by `OPTTON1_Pin`,
 * `OPTTON2_Pin`, and `OPTTON3_Pin`.
 *
 * Parameters:
 *   - sensor: The sensor number to read (1, 2, or 3).
 *     - sensor 1: Reads the state of the button connected to `OPTTON1_Pin`.
 *     - sensor 2: Reads the state of the button connected to `OPTTON2_Pin`.
 *     - sensor 3: Reads the state of the button connected to `OPTTON3_Pin`.
 *
 * Returns:
 *   - `true` if the button is pressed (GPIO pin is low).
 *   - `false` if the button is not pressed (GPIO pin is high or not connected).
 */
bool ReadOpttonValue(uint16_t sensor) {
	if (sensor == 1) {
		return HAL_GPIO_ReadPin(OPTTON1_GPIO_Port, OPTTON1_Pin);
	} else if (sensor == 2) {
		return HAL_GPIO_ReadPin(OPTTON2_GPIO_Port, OPTTON2_Pin);
	} else if (sensor == 3) {
		return HAL_GPIO_ReadPin(OPTTON3_GPIO_Port, OPTTON3_Pin);
	}

	return false; // Return false if the sensor number is invalid
}
