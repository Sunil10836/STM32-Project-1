/*
 * I2C.h
 *
 *  Created on: Mar 24, 2026
 *      Author: Sunil Sutar
 */

#ifndef I2C_H_
#define I2C_H_

#include <string.h>
#include "stm32f446xx.h"

/*
 * I2C GPIO Pins
 * PB6 --> SCL
 * PB7 --> SDA
 *
 * Alt. Fun Mode => AF4
 *
 */

#define I2C_PIN_SCL		GPIO_PIN_6
#define I2C_PIN_SDA		GPIO_PIN_7
#define I2C_PORT		GPIOB

#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68	//Arduino Slave Address

extern I2C_Handle_t I2C1Handle;


void I2C1_Init(void);
void I2C1_GPIO_PinConfig();

#endif /* I2C_H_ */
