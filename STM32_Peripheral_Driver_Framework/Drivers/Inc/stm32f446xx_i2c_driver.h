/*
 * stm32f446xx_i2c_driver.h
 *
 *  Created on: Mar 23, 2026
 *      Author: Sunil Sutar
 */

#ifndef INC_STM32F446XX_I2C_DRIVER_H_
#define INC_STM32F446XX_I2C_DRIVER_H_

#include "stm32f446xx.h"

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;

}I2C_Config_t;


/*
 *Handle structure for I2Cx peripheral
 */

typedef struct
{
	I2C_TypeDef_t *pI2Cx;			/*!< This holds the base address of I2Cx(x:1,2,3) peripheral >*/
	I2C_Config_t  I2C_Config;

}I2C_Handle_t;

/********************************* User Configurable Parameters *******************/

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 		100000
#define I2C_SCL_SPEED_FM4K 		400000
#define I2C_SCL_SPEED_FM2K  	200000

/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE          1
#define I2C_ACK_DISABLE         0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        	0
#define I2C_FM_DUTY_16_9     	1


/*
 * I2C related status flags definitions
 */
#define I2C_FLAG_SB			( 1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR 		( 1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF  		( 1 << I2C_SR1_BTF)
#define I2C_FLAG_TXE   		( 1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE   	( 1 << I2C_SR1_RXNE)

/********************************* I2C API Prototypes *******************/

/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_TypeDef_t *pI2Cx, uint8_t EnOrDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_TypeDef_t *pI2Cx);


/*
 * I2C Send and Receive Data
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr);


/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_TypeDef_t *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_TypeDef_t *pI2Cx , uint32_t FlagName);

#endif /* INC_STM32F446XX_I2C_DRIVER_H_ */
