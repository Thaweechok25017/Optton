#include "stm32l4xx_hal.h"

#ifndef INC_LCD_CONFIG_H_
#define INC_LCD_CONFIG_H_

/* CONFIG FOR LIBRARY USER */
#define D4_GPIO_PORT GPIOB
#define D5_GPIO_PORT GPIOA
#define D6_GPIO_PORT GPIOB
#define D7_GPIO_PORT GPIOC

#define RS_GPIO_PORT GPIOB
#define EN_GPIO_PORT GPIOA

//4 pin mode -> pins
#define D4_Pin GPIO_PIN_5
#define D5_Pin GPIO_PIN_15
#define D6_Pin GPIO_PIN_10
#define D7_Pin GPIO_PIN_7

#define RS_Pin GPIO_PIN_6
#define EN_Pin  GPIO_PIN_8

#endif
/* INC_LCD_CONFIG_H_ */
