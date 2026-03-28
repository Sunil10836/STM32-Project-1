/*
 * Button.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Sunil Sutar
 */

#include <string.h>
#include "Button.h"

volatile uint8_t button_flag = 0;

void Button_Init()
{
	GPIO_Handle_t GpioBtn;

	GpioBtn.pGPIOx = BTN_PORT;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = BTN_PIN;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_PeriClockControl(BTN_PORT, ENABLE);

	GPIO_Init(&GpioBtn);
}

void Button_Interrupt_Init()
{
	GPIO_Handle_t GpioBtn;

	memset(&GpioBtn, 0, sizeof(GpioBtn));	//optional

	GpioBtn.pGPIOx = BTN_PORT;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = BTN_PIN;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_PeriClockControl(BTN_PORT, ENABLE);

	GPIO_Init(&GpioBtn);

	//IRQ Configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, 15);			//optional
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

}

void EXTI15_10_IRQHandler()
{
	delay();	//button de-bouce delay

	//Handle Interrupt
	GPIO_IRQHandling(GPIO_PIN_13);

	//button_flag = 1;

	//Toggle LED
	GPIO_ToggleOutputPin(LED_PORT, LED_PIN);
}

