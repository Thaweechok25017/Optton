/*
 * TT.c
 *
 *  Created on: Mar 24, 2025
 *      Author: Thaweechock.M
 */

#include "TT.h"
#include "main.h"

uint8_t data[3];
int pattern[3];
int step,started,round_flag,pattern_count = 0;

#define MCP23S17_ADDR 0x40

#define IODIRB 0x00
#define IODIRA 0x01

#define GPIOMCPB 0x13
#define GPIOMCPA 0x12

#define OLATB 0x15
#define OLATA 0x14

#define IOCON 0x0A

int TEST;
extern SPI_HandleTypeDef hspi1;

void WaitForStart() {
    uint32_t start_time = 0;
    HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_SET);

    LED_Status(1, 1);
    LED_Status(3, 1);

    while (!started) {
        if (HAL_GPIO_ReadPin(GPIOA, OP1_Pin) && HAL_GPIO_ReadPin(GPIOA, OP3_Pin)) {
            if (start_time == 0) {
                start_time = HAL_GetTick();
            }
            if (HAL_GetTick() - start_time >= 5000) {
                started = 1;
                LED_Status(1, 0);
                LED_Status(3, 0);
                GeneratePattern();
            }
        } else {
            start_time = 0;
        }
    }
}

void GeneratePattern() {
    int patterns[12][3] = {
        {1, 2, 1}, {1, 2, 3}, {1, 3, 1}, {1, 3, 2},
        {2, 1, 2}, {2, 1, 3}, {2, 3, 1}, {2, 3, 2},
        {3, 1, 2}, {3, 1, 3}, {3, 2, 1}, {3, 2, 3}
    };
    srand(HAL_GetTick());
    int index = rand() % 12;
    for (int i = 0; i < 3; i++) {
        pattern[i] = patterns[index][i];
    }
    step = 0;
}

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
    HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port, pin1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void TurnOffAllLEDs(int delay) {
    LED_Status(1, 0);
    LED_Status(2, 0);
    LED_Status(3, 0);
    HAL_Delay(delay);
}

void ProcessPattern() {

    if (started == 1) {
        TurnOffAllLEDs(1000);

        for (int i = 0; i < 3; i++) {
            LED_Status(pattern[i], 1);
            while (!ReadButton(GPIOA, (pattern[i] == 1 ? OP1_Pin : pattern[i] == 2 ? OP2_Pin : OP3_Pin)));
            LED_Status(pattern[i], 0);
        }
        pattern_count = 1;

        HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_RESET);

        for(int m = 0 ; m < 1000 ; m++){
        Send_dataA();
        }

        TurnOffAllLEDs(1500);

        for (int i = 0; i < 3; i++) {
            LED_Status(pattern[i], 1);
            while (!ReadButton(GPIOA, (pattern[i] == 1 ? OP1_Pin : pattern[i] == 2 ? OP2_Pin : OP3_Pin)));
            LED_Status(pattern[i], 0);
        }
        pattern_count = 0;
        HAL_GPIO_WritePin(GPIOB, LED_on_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, LED_onB8_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, LED_off_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, LED_offC15_Pin, GPIO_PIN_SET);

        for(int n = 0 ; n < 1000 ; n++){
        Send_dataB();
        }

        TurnOffAllLEDs(1500);

        started = 0;
        WaitForStart();

        if (pattern_count == 0) {
            WaitForStart();
        }
    } else {
        WaitForStart();
    }
}

uint8_t ReadButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    uint32_t start_time = HAL_GetTick();
    while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) {
        if (HAL_GetTick() - start_time >= 500) {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET);  // รอจนปล่อยปุ่ม
            return 1;
        }
    }
    return 0;
}

void MCP23S17_Write(uint8_t reg, uint8_t value) {

    data[0] = MCP23S17_ADDR;
    data[1] = 0x00;
    data[2] = reg;
    data[3] = value;

    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, data, 4, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

}

void MCP23S17_Init() {
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    MCP23S17_Write(IODIRA, 0b11110000);
    MCP23S17_Write(GPIOMCPA, 0b11111111);
    MCP23S17_Write(GPIOMCPB, 0b11111111);
}

void Send_data(){
if(pattern_count == 1){
	MCP23S17_Write(GPIOMCPA, 0b11110000);
    MCP23S17_Write(GPIOMCPB, 0b11110000);
}
else{
    MCP23S17_Write(GPIOMCPA, 0b00001111);
    MCP23S17_Write(GPIOMCPB, 0b00001111);
	}
}

void Send_dataA() {
		MCP23S17_Write(GPIOMCPA, 0b11110000); // ปิด
        MCP23S17_Write(GPIOMCPB, 0b11110000);
    }

void Send_dataB(){
        MCP23S17_Write(GPIOMCPA, 0b00001111); // เปิด
        MCP23S17_Write(GPIOMCPB, 0b00001111);
}
