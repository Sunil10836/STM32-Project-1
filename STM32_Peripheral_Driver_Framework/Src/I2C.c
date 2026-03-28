/*
 * I2C.c
 *
 *  Created on: Mar 24, 2026
 *      Author: Sunil Sutar
 */

#include "I2C.h"

I2C_Handle_t I2C1Handle;

void I2C1_Init(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}

void I2C1_GPIO_PinConfig()
{
	GPIO_Handle_t I2C1Pins;

	I2C1Pins.pGPIOx = I2C_PORT;
	I2C1Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2C1Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2C1Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2C1Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2C1Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2C1Pins.GPIO_PinConfig.GPIO_PinNumber = I2C_PIN_SCL;
	GPIO_Init(&I2C1Pins);

	//SDA
	I2C1Pins.GPIO_PinConfig.GPIO_PinNumber = I2C_PIN_SDA;
	GPIO_Init(&I2C1Pins);

}
