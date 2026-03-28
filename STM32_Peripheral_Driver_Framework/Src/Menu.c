/*
 * UART_Menu.c
 *
 *  Created on: Mar 21, 2026
 *      Author: Sunil Sutar
 */

#include "Menu.h"

USART_Handle_t usart2_handle;

char msg[] = "Hello From STM32.\n\r";

char rx_buffer[50];

uint8_t tx_data[] = "I2C Mater Tx Testing\n";


void Main_Menu(USART_Handle_t *pUARTHanadle)
{
	uint8_t choice;

	while(1)
	{
		USART_SendString(&usart2_handle, "===== STM32 DRIVER TEST MENU =====\n\r");
		USART_SendString(&usart2_handle, "1. LED Test\n\r");
		USART_SendString(&usart2_handle, "2. Button Test\n\r");
		USART_SendString(&usart2_handle, "3. UART Test\n\r");
		USART_SendString(&usart2_handle, "4. I2C Test\n\r");

		USART_SendString(&usart2_handle, "Enter Choice: ");

		choice = USART_ReceiveChar(pUARTHanadle);

	    // Echo back
		USART_SendData(pUARTHanadle, &choice, 1);
		USART_SendString(pUARTHanadle, "\r\n");

		switch(choice)
		{
			case '1':
				Led_Menu(pUARTHanadle);
				break;

			case '2':
				Button_Menu(pUARTHanadle);
				break;

			case '3':
				UART_Menu(pUARTHanadle);
				break;

			case '4':
				I2C_Menu(pUARTHanadle);
				break;

			default:
				USART_SendString(&usart2_handle, "\n\rInvalid Choice\n\r");
		}
	}
}

void Led_Menu(USART_Handle_t *pUARTHanadle)
{
	uint8_t choice = 0;

	while(1)
	{
		USART_SendString(&usart2_handle, "--- LED MENU ---\n\r");

		USART_SendString(&usart2_handle, "1. LED ON\n\r");
		USART_SendString(&usart2_handle, "2. LED OFF\n\r");
		USART_SendString(&usart2_handle, "3. LED TOGGLE\n\r");
		USART_SendString(&usart2_handle, "4. Back\n\r");

		USART_SendString(&usart2_handle, "Enter Choice: ");

		choice = USART_ReceiveChar(pUARTHanadle);

	    // Echo back
		USART_SendData(pUARTHanadle, &choice, 1);
		USART_SendString(pUARTHanadle, "\r\n");

		switch(choice)
		{
			case '1':
				LED_On();
				USART_SendString(pUARTHanadle, "LED ON\n\r");
				break;

			case '2':
				LED_Off();
				USART_SendString(pUARTHanadle, "LED OFF\n\r");
				break;

			case '3':
				LED_Toggle();
				USART_SendString(pUARTHanadle, "LED Toggle\n\r");
				break;

			case '4':
				return;

			default:
				USART_SendString(&usart2_handle, "\n\rInvalid Choice\n\r");
		}
	}
}

void Button_Menu(USART_Handle_t *pUARTHanadle)
{
	USART_SendString(&usart2_handle, "Button Test..\n\r");

	if(GPIO_ReadFromInputPin(BTN_PORT, BTN_PIN) == BTN_PRESSED)
	{
		//Toggle LED
		GPIO_ToggleOutputPin(LED_PORT, LED_PIN);

		USART_SendString(&usart2_handle, "Button Pressed..\n\r");
	}
	else
	{
		USART_SendString(&usart2_handle, "Button Released..\n\r");
	}
}

void UART_Menu(USART_Handle_t *pUARTHanadle)
{

	uint8_t choice = 0;

	while(1)
	{
		USART_SendString(&usart2_handle, "--- UART MENU ---\n\r");

		USART_SendString(&usart2_handle, "1. UART Tx\n\r");
		USART_SendString(&usart2_handle, "2. UART TxRx\n\r");
		USART_SendString(&usart2_handle, "3. Back\n\r");

		USART_SendString(&usart2_handle, "Enter Choice: ");

		choice = USART_ReceiveChar(pUARTHanadle);

		// Echo back
		USART_SendData(pUARTHanadle, &choice, 1);
		USART_SendString(pUARTHanadle, "\r\n");

		switch(choice)
		{
			case '1':
				USART_SendString(pUARTHanadle, "UART Tx\n\r");
				USART_SendData(&usart2_handle, (uint8_t *)msg, strlen(msg));
				break;

			case '2':
				USART_SendString(pUARTHanadle, "UART TxRx\n\r");
				USART_SendData(&usart2_handle, (uint8_t *)msg, strlen(msg));
				USART_ReceiveData(&usart2_handle, (uint8_t *)rx_buffer, strlen(rx_buffer));
				USART_SendString(&usart2_handle, rx_buffer);
//				USART_SendString(&usart2_handle, "Enter data:\r\n");
//				USART_ReceiveLine(&usart2_handle, rx_buffer);
//				USART_SendString(&usart2_handle, "You typed: ");
//				USART_SendString(&usart2_handle, rx_buffer);
//				USART_SendString(&usart2_handle, "\r\n");
				break;

			case '3':
				return;

			default:
				USART_SendString(&usart2_handle, "\n\rInvalid Choice\n\r");
		}
	}
}

void I2C_Menu(USART_Handle_t *pUARTHanadle)
{
	uint8_t choice = 0;

	while(1)
	{
		USART_SendString(&usart2_handle, "--- I2C MENU ---\n\r");

		USART_SendString(&usart2_handle, "1. I2C Master Tx\n\r");
		USART_SendString(&usart2_handle, "2. I2C Master Rx\n\r");
		USART_SendString(&usart2_handle, "3. Back\n\r");

		USART_SendString(&usart2_handle, "Enter Choice: ");

		choice = USART_ReceiveChar(pUARTHanadle);

		// Echo back
		USART_SendData(pUARTHanadle, &choice, 1);
		USART_SendString(pUARTHanadle, "\r\n");

		switch(choice)
		{
			case '1':
				USART_SendString(pUARTHanadle, "I2C Master Tx\n\r");
				I2C_MasterSendData(&I2C1Handle, tx_data, strlen((char *)tx_data), SLAVE_ADDR); //Send Data
				break;

			case '2':
				USART_SendString(pUARTHanadle, "I2C Master Rx\n\r");

				break;

			case '3':
				return;

			default:
				USART_SendString(&usart2_handle, "\n\rInvalid Choice\n\r");
		}
	}


}
