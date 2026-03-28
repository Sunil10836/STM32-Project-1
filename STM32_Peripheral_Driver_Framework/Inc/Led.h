/*
 * Led.h
 *
 *  Created on: Mar 16, 2026
 *      Author: Sunil Sutar
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f446xx.h"

#define LED_PORT		GPIOA
#define LED_PIN			GPIO_PIN_5

void LED_Init();
void LED_On();
void LED_Off();
void LED_Toggle();
void LED_Blink();
void delay();

#endif /* LED_H_ */
