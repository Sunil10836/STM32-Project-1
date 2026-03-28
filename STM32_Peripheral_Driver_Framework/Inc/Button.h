/*
 * Button.h
 *
 *  Created on: Mar 20, 2026
 *      Author: Sunil Sutar
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f446xx.h"

#include "Led.h"

extern volatile uint8_t button_flag;

#define BTN_PORT		GPIOC
#define BTN_PIN			GPIO_PIN_13

#define GPIO_PIN_LOW	0
#define BTN_PRESSED		GPIO_PIN_LOW

void Button_Init();
void Button_Interrupt_Init();

#endif /* BUTTON_H_ */
