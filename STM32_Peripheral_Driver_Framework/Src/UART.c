/*
 * UART.c
 *
 *  Created on: Mar 21, 2026
 *      Author: Sunil Sutar
 */

#include <string.h>
#include "UART.h"

void USART2_GPIO_PinConfig(void)
{
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = USART2_GPIO_PORT;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

	//USART2_TX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = USART2_TX;
	GPIO_Init(&usart_gpios);

	//USART2_RX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = USART2_RX;
	GPIO_Init(&usart_gpios);
}

void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baudrate = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

	USART_Init(&usart2_handle);
}

/*
void USART_ReceiveLine(USART_TypeDef_t *USARTx, uint8_t *buffer)
{
    uint32_t i = 0;
    uint8_t ch;

    while (1)
    {
        while (!(USARTx->SR & (1 << 5))); // Wait RXNE
        ch = (uint8_t)(USARTx->DR & 0xFF);

        if (ch == '\r' || ch == '\n')
        {
            buffer[i] = '\0'; // Null terminate
            break;
        }

        buffer[i++] = ch;
    }
}
*/

/*

void UART_SendChar(USART_TypeDef_t *pUSARTx, char ch)
{
    while(!(pUSARTx->SR & (1 << USART_SR_TXE)));

    pUSARTx->DR = ch;
}

void UART_SendString(USART_TypeDef_t *pUSARTx, const char* str)
{
    while(*str)
    {
    	UART_SendChar(pUSARTx, *str++);
    }
}
*/

/*
void UART_SendString(USART_TypeDef_t *pUSARTx,char *str)
{
	while(*str)
	{
		UART_SendChar(USART2, *str++);
	}

	//while(! (pUSARTx->SR & (1 << USART_SR_TC)));
}
*/

/*
char UART_ReceiveChar(USART_TypeDef_t *pUSARTx)
{
    while(!(pUSARTx->SR & (1 << USART_SR_RXNE)));

    return pUSARTx->DR;
}

int UART_ReceiveNumber(USART_TypeDef_t *pUSARTx)
{
	 char ch = UART_ReceiveChar(pUSARTx);

	 UART_SendChar(pUSARTx, ch); // echo

	 return (ch - '0');
}
*/
