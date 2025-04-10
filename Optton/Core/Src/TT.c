/*
 * TT.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */
/* Firmware Use for OPTTON SENSOR AND DEMO JIG TO SEND SPI DATA
 * This STM32-based program interfaces with both on board GPIOs and an MCP23S17 SPI GPIO expander to control external LEDs.
 *
 * Main Objectives:
 *
 * 1. Enter setup mode by pressing buttons OP1 and OP3 together for 5 seconds.
 *
 * 2. Randomly generate a 3-button pattern using OP1, OP2, and OP3.
 *
 * 3. Indicate that pattern using LEDs and wait for the user to input OP1, OP2, and OP3 it correctly.
 *
 * 4. Wait for button input using OP1, OP2, and OP3, with guidance provided via LED indicators.
 * The system should detect both button press and release events using sensors.
 *
 * 	4.1 Indicate with LEDs (open state) and Send SPI To Show LED OPEN JIG
 * 	4.2 Wait for the same pattern again
 * 	4.3 Indicate with LEDs (closed state)and Send SPI To Show LED CLOSE JIG
 *
 * 5. Repeat the above process.
 *
 * */

#include "TT.h"
#include "main.h"

uint8_t data[4];  // Buffer for sending 4 bytes via SPI to MCP23S17
int pattern[3];   // Holds the 3-step button pattern
int step, started, round_flag, pattern_count = 0; // Set Keep data for function

#define MCP23S17_ADDR 0x48   // MCP23S17 SPI address
#define IODIRB  0x01         // MCP register for direction B
#define OLATB 0x15           // MCP register for output latch B

extern SPI_HandleTypeDef hspi1;  // SPI handle used for MCP23S17

/* "WaitForStart()"
 * 																		# Control LED OPTTON
 * Is a First Setup "IDEL"Turn On LED1 and LED3 Waits for the user to press and hold OP1 and OP3 for 5 seconds
 * and When held long enough, Turn off LED sets started = 1 and calls GeneratePattern()and if not held long enough started = 0 .*/

void WaitForStart() {
    uint32_t start_time = 0;

    // Set LED status to OFF
    HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_SET);

    // Turn on indicator LEDs 1 and 3
    LED_Status(1, 1);
    LED_Status(3, 1);

    while (!started) {
        if (HAL_GPIO_ReadPin(GPIOA, OP1_Pin) && HAL_GPIO_ReadPin(GPIOA, OP3_Pin)) {
            if (start_time == 0) {
                start_time = HAL_GetTick(); // Start timing
            }
            if (HAL_GetTick() - start_time >= 5000) { // Wait Hold 5 Second
                started = 1;
                LED_Status(1, 0);
                LED_Status(3, 0);
                GeneratePattern(); // Randomly generate a pattern
            }
        } else {
            start_time = 0; // Reset if buttons are released
        }
    }
}

/* "GeneratePattern()"
 *
 * Randomly selects one of 12 predefined button sequences.
 * The pattern is stored in the pattern[3] array.*/

void GeneratePattern() {
    int patterns[12][3] = {
        {1, 2, 1}, {1, 2, 3}, {1, 3, 1}, {1, 3, 2},
        {2, 1, 2}, {2, 1, 3}, {2, 3, 1}, {2, 3, 2},
        {3, 1, 2}, {3, 1, 3}, {3, 2, 1}, {3, 2, 3}
    };												//set data 12 pattern
    srand(HAL_GetTick());
    int index = rand() % 12;						// Choose one pattern randomly
    for (int i = 0; i < 3; i++) {
        pattern[i] = patterns[index][i];		 	// Save selected pattern for OPTTON 3
    }
    step = 0;
}

/* " LED_Status (int index, int state) "
 *  													# Control LED OPTTON
 * Use Turn ON or OFF a Specific LED by index (1-3)
 * and State (on = 1 , Off = 0) to Control LED Generate
 */

void LED_Status(int index, int state) {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint16_t pin1;

    switch (index) {
        case 1: port = GPIOB; pin = LED_1B1_Pin; pin1 = LED_1_Pin; break;
        case 2: port = GPIOB; pin = LED_2B6_Pin; pin1 = LED_2_Pin;break;
        case 3: port = GPIOB; pin = LED_3B4_Pin; pin1 = LED_3_Pin;break;
        default: return;
    }
    // Turn the LEDs ON or OFF
    HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port, pin1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* "TurnOffAllLEDs(int delay)"
 * 															# Control LED OPTTON
 * Use function LED_Status() To Turns off all LEDs and waits for the specified delay.  */

void TurnOffAllLEDs(int delay) {
    LED_Status(1, 0);
    LED_Status(2, 0);
    LED_Status(3, 0);
    HAL_Delay(delay); 			// Wait after turning LEDs off
}

/* "ProcessPattern()"
 * 																		# Control LED OPTTON
 * When started == 1 Use Function TurnOffAllLEDs() to turn off led and delay 1 Sec
 * after that Set LED On From GeneratePattern() and data Keep data in pattern[i] and Show LED From Pattern
 * wait for the user to press correct buttons one by one to Step Control "IDEL" To "OPEN" and Change data pattern_count = 1
 * and Turn LED Show Open or Close Status do the same in "CLOSE" but will Change data pattern_count = 0
 * and set started = 0 Return to WaitForStart() to Start New Loop.## Pattern Will New Generate When pattern "OPEN" and "CLOSE" Done */

void ProcessPattern() {

    if (started == 1) {
        TurnOffAllLEDs(1000);			// Clear before showing pattern

        // Show and check pattern input

        for (int i = 0; i < 3; i++) {
            LED_Status(pattern[i], 1);
            while (!ReadButton(GPIOA, (pattern[i] == 1 ? OP1_Pin : pattern[i] == 2 ? OP2_Pin : OP3_Pin)));
            LED_Status(pattern[i], 0);
        }
        pattern_count = 1;	// Mark "OPEN" pattern

        // Show OPEN status LED

        HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_RESET);

        TurnOffAllLEDs(1500);  // Delay 1.5 Second Pause before "CLOSE" phase

        // Re-enter pattern for CLOSE

        for (int i = 0; i < 3; i++) {
            LED_Status(pattern[i], 1);
            while (!ReadButton(GPIOA, (pattern[i] == 1 ? OP1_Pin : pattern[i] == 2 ? OP2_Pin : OP3_Pin)));
            LED_Status(pattern[i], 0);
        }
        pattern_count = 0;		// Mark "CLOSE" pattern

        // Show CLOSED status LED

        HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_SET);

        TurnOffAllLEDs(1500);

        started = 0;
    } else {
        WaitForStart();			// if started = 0 Not started Reset process
    }
}

/* "ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)"
 *
 * Use For Debounce to Protect press wrong and
 * Checks for a button press that is held for at least 500ms and then released.*/

uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    uint32_t start_time = HAL_GetTick();
    while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) {
        if (HAL_GetTick() - start_time >= 1000) {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET);
            return 1;			// Button held for 500 ms
        }
    }
    return 0;
}

/* " MCP23S17_Write(uint8_t reg, uint8_t value)"
 *
 * Writes a value to a register on MCP23S17 via SPI.
 * by Variable "data" will Keep Array 4 from "data[3]"
 * 0 = ADDRESS MCP
 * 1 = 0x00 R/W #this   Read 0x01 / Write 0x00
 * 2 = Register Data what Send or Set data To MCP
 * 3 = Value data want to trigger to MCP
 * after that Reset pin CS to Start Send Data and Set on CS Pin When Data Finish Transmit SPI */

void MCP23S17_Write(uint8_t reg, uint8_t value) {

    data[0] = MCP23S17_ADDR;		// ADRESS MCP23S17 JIG
    data[1] = 0x00;					// Write or Read command
    data[2] = reg;					// Register address
    data[3] = value;				// Value to write

    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, data, 4, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

/* "MCP23S17_Init()"
 *
 * Use function MCP23S17_Write(uint8_t reg, uint8_t value) for Start Trigger Pin
 * and Set Input = 1 or Output = 0 to MCP and Port GPIO Set HIGH or LOW*/

void MCP23S17_Init() {
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    MCP23S17_Write(IODIRB, 0b11110000);  // Set lower 4 pins of port B to output
	MCP23S17_Write(OLATB, 0b00001111);	// Set All HIGH # LED JIG OFF #
}

/* "Send_data()"
 *
 * Use for Link from ProcessPattern()
 * When "OPEN" pattern press Done Send_dataB or "OPEN" pattern press Done Send_dataA this will
 * Use FreeRTOS To 2 Task Send_data() and ProcessPattern() */

void Send_data(){
if(pattern_count == 1 ){
		Send_dataB();		// If OPEN SEND SPI AND ON JIG LED
}
else if (pattern_count == 0 ){
		Send_dataA();		// If CLOSE SEND SPI AND OFF JIG LED
	}
}

/* "Send_dataA()"
 *
 * Set OFF LED in JIG Port MCP23s17 port A and B 			# Control LED JIG
 * value 0 = Trigger Port MCP23s17 active LOW
 * value 1 = Trigger Port MCP23s17 active HIGH
 * 1111 1100
 */

void Send_dataA() {
		MCP23S17_Write(OLATB, 0b11111100);		// Turn off bits GPIOB 0–1 MCP
    }

/* "Send_dataB()"
 *
 * Set ON LED in JIG Port MCP23s17 port A and B				# Control LED JIG
 * value 0 = Trigger Port MCP23s17 active LOW
 * value 1 = Trigger Port MCP23s17 active HIGH
 * 1111 0011
 */

void Send_dataB(){

		MCP23S17_Write(OLATB, 0b11110011);		// Turn on bits GPIOB 2–3 MCP

}
