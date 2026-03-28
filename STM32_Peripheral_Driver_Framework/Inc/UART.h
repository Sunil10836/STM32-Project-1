/*
 * UART.h
 *
 *  Created on: Mar 21, 2026
 *      Author: Sunil Sutar
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f446xx_usart_driver.h"

#define USART2_GPIO_PORT	GPIOA
#define USART2_TX			GPIO_PIN_2
#define USART2_RX			GPIO_PIN_3

extern USART_Handle_t usart2_handle;

void USART2_GPIO_PinConfig(void);
void USART2_Init(void);
//void USART_ReceiveLine(USART_TypeDef_t *USARTx, uint8_t *buffer);

//void UART_SendChar(USART_TypeDef_t *pUSARTx, char ch);
//void UART_SendString(USART_TypeDef_t *pUSARTx, const char* str);
//
//char UART_ReceiveChar(USART_TypeDef_t *pUSARTx);
//int UART_ReceiveNumber(USART_TypeDef_t *pUSARTx);

#endif /* UART_H_ */
