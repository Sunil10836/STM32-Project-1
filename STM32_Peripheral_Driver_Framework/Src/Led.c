/*
 * Led.c
 *
 *  Created on: Mar 16, 2026
 *      Author: Sunil Sutar
 */


#include "Led.h"

/*
 * LED = PA5
 */
void LED_Init()
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = LED_PORT;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = LED_PIN;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	//GPIO_OP_TYPE_OD
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_PeriClockControl(LED_PORT, ENABLE);

	GPIO_Init(&GpioLed);
}

void LED_On()
{
	GPIO_WriteToOutputPin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void LED_Off()
{
	GPIO_WriteToOutputPin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void LED_Toggle()
{
	GPIO_ToggleOutputPin(LED_PORT, LED_PIN);
}

void LED_Blink()
{
	GPIO_WriteToOutputPin(LED_PORT, LED_PIN, GPIO_PIN_SET);
	delay();
	GPIO_WriteToOutputPin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
	delay();
}

void delay()
{
	for(uint32_t i=0; i<500000; i++);
}
