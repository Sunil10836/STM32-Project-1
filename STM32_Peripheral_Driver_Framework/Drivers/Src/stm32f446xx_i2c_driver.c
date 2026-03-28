/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: Mar 23, 2026
 *      Author: Sunil Sutar
 */

#include "stm32f446xx_i2c_driver.h"

static void I2C_GenerateStartCondition(I2C_TypeDef_t *pI2Cx);
static void I2C_ExecuteAddressPhase(I2C_TypeDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_TypeDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_TypeDef_t *pI2Cx);

/*********************************************************************
 * @fn      		  - I2C_GenerateStartCondition
 *
 * @brief             -
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */

static void I2C_GenerateStartCondition(I2C_TypeDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

/*********************************************************************
 * @fn      		  - I2C_ExecuteAddressPhase
 *
 * @brief             -
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
static void I2C_ExecuteAddressPhase(I2C_TypeDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1 << 0);			//R/W = 0, Write
	pI2Cx->DR = SlaveAddr;
}

/*********************************************************************
 * @fn      		  - I2C_ClearADDRFlag
 *
 * @brief             -
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */

static void I2C_ClearADDRFlag(I2C_TypeDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}

/*********************************************************************
 * @fn      		  - I2C_GenerateStopCondition
 *
 * @brief             -
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */

static void I2C_GenerateStopCondition(I2C_TypeDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

/*********************************************************************
 * @fn      		  - I2C_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given I2C
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */

void I2C_PeriClockControl(I2C_TypeDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}
	}
}

/*********************************************************************
 * @fn      		  - I2C_Init
 *
 * @brief             - I2C Initialization Function
 *
 * @param[in]         - pointer to I2C Handle
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              -	none

 */

void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;

	//Enable I2C Peripheral Clock
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	/******** Configure I2C_CR1 ********************/
	//Enable ACK Control Bit
	tempreg |= (pI2CHandle->I2C_Config.I2C_AckControl << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 |= tempreg;

	/******** Configure I2C_CR2 ********************/
	//Configure FREQ Field of CR2
	tempreg = 0;
	tempreg = RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 |= (tempreg & 0x3F);

	/******** Configure I2C_OAR ********************/
	//program the device own address
	tempreg = 0;
	tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);	//Given in RM
	pI2CHandle->pI2Cx->OAR1 |= tempreg;

	/******** Configure I2C_CCR ********************/
	tempreg = 0;
	uint16_t ccr_value = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Mode is Standard Mode
		ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF);
	}
	else
	{
		//Mode is Fast Mode
		tempreg |= (1 << I2C_CCR_FS);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY);

		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	/******** Configure I2C_TRISE ********************/
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Mode is Standard Mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;				//trise = 1000nSec in SM Mode
	}
	else
	{
		//Mode is Fast Mode
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000U) + 1;		//trise = 300nSec in FM Mode
	}

	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}

/*********************************************************************
 * @fn      		  - I2C_DeInit
 *
 * @brief             - I2C De-Initialization Function
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */

void I2C_DeInit(I2C_TypeDef_t *pI2Cx)
{

}

/*********************************************************************
 * @fn      		  - I2C_SendData
 *
 * @brief             - I2C Send Data
 *
 * @param[in]         - pointer to I2C Handle
 * @param[in]         - pointer to Tx Buffer
 * @param[in]         - Length
 * @param[in]         - Slave Address
 *
 * @return            - none
 *
 * @Note              -	none

 */

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	//1. Generate START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that START Generation is Completed by checking SB flag in SR
	//Until SB is Cleared SCL will be stretched(pulled to LOW)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the Slave Address with R/W bit set to 0
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm the Address Phase is Completed by Checking ADDR Flag in SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Clear ADDR Flag
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. Send Data until len becomes 0
	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));	//wait until TXE flag is set
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	//7. When  Len becomes 0 wait for TXE=1 and BTF=1 before STOP Condition
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate STOP Condition
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}



/*********************************************************************
 * @fn      		  - I2C_PeripheralControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */

void I2C_PeripheralControl(I2C_TypeDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}
	else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}

/*********************************************************************
 * @fn      		  - I2C_GetFlagStatus
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */

uint8_t I2C_GetFlagStatus(I2C_TypeDef_t *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
