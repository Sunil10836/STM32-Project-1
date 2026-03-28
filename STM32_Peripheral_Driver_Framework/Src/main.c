/*
 * main.c
 *
 *  Created on: Mar 16, 2026
 *      Author: Sunil Sutar
 */

#include <Menu.h>
#include <string.h>
#include "stm32f446xx.h"
#include "Led.h"
#include "Button.h"
#include "UART.h"
#include "I2C.h"

int main(void)
{
	LED_Init();
	Button_Init();

	USART2_GPIO_PinConfig();
	USART2_Init();
	USART_PerpheralControl(USART2, ENABLE);

	I2C1_GPIO_PinConfig();
	I2C1_Init();
	I2C_PeripheralControl(I2C1, ENABLE);


	while(1)
	{
		Main_Menu(&usart2_handle);
	}
}


