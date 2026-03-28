/*
 * UART_Menu.h
 *
 *  Created on: Mar 21, 2026
 *      Author: Sunil Sutar
 */

#ifndef MENU_H_
#define MENU_H_

#include "stm32f446xx.h"
#include "Led.h"
#include "Button.h"
#include "UART.h"
#include "I2C.h"

void Main_Menu(USART_Handle_t *pUARTHanadle);
void Led_Menu(USART_Handle_t *pUARTHanadle);
void Button_Menu(USART_Handle_t *pUARTHanadle);
void UART_Menu(USART_Handle_t *pUARTHanadle);
void I2C_Menu(USART_Handle_t *pUARTHanadle);

#endif /* MENU_H_ */
