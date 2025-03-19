/*
 * display.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Chanchai.Th
 */

#include "display.h"
#include "storage.h"

extern TIM_HandleTypeDef htim3;

uint32_t servoPos[1];

/*
 * Oled_Init
 *
 * This function initializes the OLED display and shows a welcome message on the screen.
 * It first initializes the SSD1306 OLED display, then positions the cursor at the top-left corner (0, 0),
 * and displays the message "Welcome" using a specific font (`Font_11x18`).
 * After the message is displayed, it updates the screen to show the changes.
 *
 * No parameters or return values.
 */
void Oled_Init(void) {
	SSD1306_Init();
	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("Welcome", &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

/*
 * Oled_ShowMessage
 *
 * This function displays a custom message on the OLED screen at a specified location.
 * It moves the cursor to the coordinates (x, y), then displays the message `m` using a specific font (`Font_11x18`).
 * After displaying the message, it updates the screen to show the changes.
 *
 * Parameters:
 *   - m: The message to be displayed on the OLED screen.
 *   - x: The x-coordinate where the message will be displayed.
 *   - y: The y-coordinate where the message will be displayed.
 */
void Oled_ShowMessage(char *m, uint16_t x, uint16_t y) {
	SSD1306_GotoXY(x, y);
	SSD1306_Puts(m, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

/*
 * Oled_ShowMessage_sm
 *
 * This function displays a custom message on the OLED screen at a specified location using a smaller font.
 * It moves the cursor to the coordinates (x, y), then displays the message `m` using the font `Font_7x10`.
 * After displaying the message, it updates the screen to show the changes.
 *
 * Parameters:
 *   - m: The message to be displayed on the OLED screen.
 *   - x: The x-coordinate where the message will be displayed.
 *   - y: The y-coordinate where the message will be displayed.
 */
void Oled_ShowMessage_sm(char *m, uint16_t x, uint16_t y) {
	SSD1306_GotoXY(x, y);
	SSD1306_Puts(m, &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

/*
 * Oled_Clear
 *
 * This function clears the OLED screen by calling the SSD1306_Clear function,
 * which removes all previously displayed content on the screen.
 */
void Oled_Clear() {
	SSD1306_Clear();
}

/*
 * Led_Show
 *
 * This function controls the state of three LEDs. Each LED is controlled by passing
 * a boolean value for `led1`, `led2`, and `led3`. If the corresponding value is `true`,
 * the LED will be turned ON (GPIO_PIN_SET), otherwise, it will be turned OFF (GPIO_PIN_RESET).
 *
 * Parameters:
 *   - led1: State of the first LED (true for ON, false for OFF).
 *   - led2: State of the second LED (true for ON, false for OFF).
 *   - led3: State of the third LED (true for ON, false for OFF).
 */
void Led_Show(bool led1, bool led2, bool led3) {
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, led1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, led2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, led3 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/*
 * Led_Custom
 *
 * This function controls the state of a specific LED based on the provided `index` and
 * `status`. The `index` specifies which LED to control (1, 2, or 3), and the `status`
 * determines whether the LED should be ON (true) or OFF (false).
 *
 * Parameters:
 *   - index: The LED to control (1, 2, or 3).
 *   - status: The desired state of the LED (true for ON, false for OFF).
 */
void Led_Custom(uint32_t index, bool status) {
	switch (index) {
	case 1:
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, status ? GPIO_PIN_SET : GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, status ? GPIO_PIN_SET : GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, status ? GPIO_PIN_SET : GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}

/*
 * Led_Clear
 *
 * This function turns off all LEDs by calling `Led_Show(false, false, false)`.
 * It ensures that LED1, LED2, and LED3 are all set to the OFF state.
 */
void Led_Clear(void) {
	Led_Show(false, false, false);
}

/*
 * Buzzer_Beep
 *
 * This function activates the buzzer by starting PWM on timer 3, channel 1,
 * waits for the specified delay (in milliseconds), and then stops the PWM.
 *
 * Parameters:
 *   - delay: Duration (in milliseconds) for which the buzzer should beep.
 *
 * Dependencies:
 *   - Requires `htim3` to be properly initialized for PWM output.
 *   - Uses `HAL_TIM_PWM_Start` and `HAL_TIM_PWM_Stop` for controlling the buzzer.
 */
void Buzzer_Beep(uint32_t delay) {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_Delay(delay);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

/*
 * Buzzer_OK
 *
 * This function provides an "OK" beep sound pattern by:
 *   1. Activating the buzzer for 100 milliseconds.
 *   2. Waiting for 100 milliseconds.
 *   3. Activating the buzzer again for 50 milliseconds.
 *
 * Dependencies:
 *   - Calls `Buzzer_Beep` to generate the sound.
 *   - Requires `htim3` to be properly initialized for PWM output.
 */
void Buzzer_OK(void) {
	Buzzer_Beep(100);
	HAL_Delay(100);
	Buzzer_Beep(50);
}

/*
 * Buzzer_OK_L
 *
 * This function generates a long "OK" beep sound by activating
 * the buzzer for 500 milliseconds.
 *
 * Dependencies:
 *   - Calls `Buzzer_Beep` to generate the sound.
 *   - Requires `htim3` to be properly initialized for PWM output.
 */
void Buzzer_OK_L(void) {
	Buzzer_Beep(500);
}

/*
 * Buzzer_NG
 *
 * This function generates a "NG" (No Good) alert beep sound by
 * activating the buzzer for 300 milliseconds.
 *
 * Dependencies:
 *   - Calls `Buzzer_Beep` to generate the sound.
 *   - Requires `htim3` to be properly initialized for PWM output.
 */
void Buzzer_NG(void) {
	Buzzer_Beep(300);
}

/*
 * Servo_Init
 *
 * Initializes the servo motor by reading the last stored position
 * and ensuring it is within the valid range. If the position is
 * out of bounds, it resets to the default closed position.
 *
 * Steps:
 *  1. Reads the last saved servo position using `ReadServoPosition`.
 *  2. Checks if the position is within `SERVO_CLOSE_VAL` and `SERVO_OPEN_VAL`.
 *  3. If out of range, resets to `SERVO_CLOSE_VAL` and saves it.
 *  4. Sets the PWM compare value to the servo position.
 *  5. Starts the PWM signal on `TIM_CHANNEL_2`.
 *  6. Moves the servo to the last known closed position.
 *
 * Parameters:
 *  - `htim`: Pointer to the TIM handle used for controlling the servo.
 *
 * Dependencies:
 *  - `ReadServoPosition()`: Reads stored servo positions.
 *  - `WriteServoPosition()`: Writes servo positions to persistent storage.
 *  - `__HAL_TIM_SET_COMPARE()`: Sets the PWM duty cycle.
 *  - `HAL_TIM_PWM_Start()`: Starts the PWM output.
 *  - `Servo_Close_From_Last_Pos()`: Moves the servo to the closed position.
 */
void Servo_Init(TIM_HandleTypeDef *htim) {
	ReadServoPosition(servoPos);
	if (servoPos[0] < SERVO_CLOSE_VAL || servoPos[0] > SERVO_OPEN_VAL) {
		servoPos[0] = SERVO_CLOSE_VAL;
		WriteServoPosition(servoPos);
	}
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, servoPos[0]);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
	Servo_Close_From_Last_Pos(htim);
}

/*
 * Servo_Idle
 *
 * Moves the servo to its last known closed position.
 *
 * Steps:
 *  1. Reads the last saved servo position using `ReadServoPosition()`.
 *  2. Moves the servo to the last known closed position using `Servo_Close_From_Last_Pos()`.
 *
 * Parameters:
 *  - `htim`: Pointer to the TIM handle used for controlling the servo.
 *
 * Dependencies:
 *  - `ReadServoPosition()`: Reads stored servo positions.
 *  - `Servo_Close_From_Last_Pos()`: Moves the servo to the closed position.
 */
void Servo_Idle(TIM_HandleTypeDef *htim) {
	ReadServoPosition(servoPos);
	Servo_Close_From_Last_Pos(htim);
}

/*
 * Servo_Open
 *
 * Gradually opens the servo from the closed position to the fully open position.
 *
 * Steps:
 *  1. Sets the servo to the initial closed position (`SERVO_CLOSE_VAL`).
 *  2. Gradually increases the position in steps (`SERVO_STEP_INV`) until it reaches `SERVO_OPEN_VAL`.
 *  3. Writes the final servo position to memory using `WriteServoPosition()`.
 *  4. Ensures the servo is precisely set to `SERVO_OPEN_VAL` at the end.
 *
 * Parameters:
 *  - `htim`: Pointer to the TIM handle used for controlling the servo.
 *
 * Dependencies:
 *  - `WriteServoPosition()`: Stores the servo's last known position.
 *  - `SERVO_CLOSE_VAL`: Defines the closed position of the servo.
 *  - `SERVO_OPEN_VAL`: Defines the fully open position of the servo.
 *  - `SERVO_STEP_INV`: Defines the incremental step size for opening.
 *  - `SERVO_STEP_DELAY`: Defines the delay between steps.
 */
void Servo_Open(TIM_HandleTypeDef *htim) {
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, SERVO_CLOSE_VAL);
	uint32_t position = SERVO_CLOSE_VAL;
	while (position <= SERVO_OPEN_VAL) {
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, position);
		position += SERVO_STEP_INV;
		HAL_Delay(SERVO_STEP_DELAY);
	}
	WriteServoPosition(&position);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, SERVO_OPEN_VAL);
}

/*
 * Servo_Close
 *
 * Gradually moves the servo from the open position (`SERVO_OPEN_VAL`)
 * to the closed position (`SERVO_CLOSE_VAL`) in incremental steps.
 *
 * Steps:
 *  1. Set the servo to `SERVO_OPEN_VAL` (fully open).
 *  2. Gradually decrease the position in steps of `SERVO_STEP_INV` until near `SERVO_CLOSE_VAL`.
 *  3. Write the final position using `WriteServoPosition()`.
 *  4. Ensure exact positioning at `SERVO_CLOSE_VAL`.
 *
 * Parameters:
 *  - `htim`: Pointer to the TIM handle used for controlling the servo.
 *
 * Dependencies:
 *  - `WriteServoPosition()`: Stores the servo's last known position.
 *  - `SERVO_OPEN_VAL`: Defines the fully open position of the servo.
 *  - `SERVO_CLOSE_VAL`: Defines the closed position of the servo.
 *  - `SERVO_STEP_INV`: Defines the decremental step size for closing.
 *  - `SERVO_STEP_DELAY`: Defines the delay between steps.
 */
void Servo_Close(TIM_HandleTypeDef *htim) {
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, SERVO_OPEN_VAL);
	uint32_t position = SERVO_OPEN_VAL;
	while (position >= SERVO_CLOSE_VAL) {
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, position);
		position -= SERVO_STEP_INV;
		HAL_Delay(SERVO_STEP_DELAY);
	}
	WriteServoPosition(&position);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, SERVO_CLOSE_VAL);
}

/*
 * Servo_Close_From_Last_Pos
 *
 * Moves the servo from its last known position (`servoPos[0]`)
 * to the closed position (`SERVO_CLOSE_VAL`) in incremental steps.
 *
 * Steps:
 *  1. Read the last known position from `servoPos[0]`.
 *  2. Gradually decrease the position in steps of `SERVO_STEP_INV` until near `SERVO_CLOSE_VAL`.
 *  3. Write the final position using `WriteServoPosition()`.
 *  4. Ensure exact positioning at `SERVO_CLOSE_VAL`.
 *
 * Parameters:
 *  - `htim`: Pointer to the TIM handle used for controlling the servo.
 *
 * Dependencies:
 *  - `WriteServoPosition()`: Stores the servo's last known position.
 *  - `servoPos[0]`: Holds the last known servo position.
 *  - `SERVO_CLOSE_VAL`: Defines the closed position of the servo.
 *  - `SERVO_STEP_INV`: Defines the decremental step size for closing.
 *  - `SERVO_STEP_DELAY`: Defines the delay between steps.
 */
void Servo_Close_From_Last_Pos(TIM_HandleTypeDef *htim) {
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, servoPos[0]);
	uint32_t position = servoPos[0];
	while (position >= SERVO_CLOSE_VAL) {
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, position);
		position -= SERVO_STEP_INV;
		HAL_Delay(SERVO_STEP_DELAY);
	}
	WriteServoPosition(&position);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, SERVO_CLOSE_VAL);
}
