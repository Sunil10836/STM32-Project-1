/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Mar 20, 2026
 *      Author:  Sunil Sutar
 */

#include "stm32f446xx_spi_driver.h"

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given SPI
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
void SPI_PeriClockControl(SPI_TypeDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
		else if(pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
		else if(pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
	}
}

/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             - SPI Initialization Function
 *
 * @param[in]         - pointer to SPI Handle
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              -	none

 */

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//Enable the Clock for SPI Peripheral
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	/*Configure SPI_CR1*/
	uint32_t tempreg = 0;

	//1. Configure SPI Device Mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2. Configure SPI Bus Config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDI_MODE should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI_MODE should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI_MODE should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);

		//RXONLY Bit must be Set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |=  pSPIHandle->SPIConfig.SPI_BusConfig <<SPI_CR1_BR;

	//4. Configure DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	//7. Configure the SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 |= tempreg;
}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             - SPI De-Initialization Function
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */

void SPI_DeInit(SPI_TypeDef_t *pSPIx)
{

}

uint8_t SPI_GetFlagStatus(SPI_TypeDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             -
 *
 * @param[in]         - Base address of the SPI peripheral
 * @param[in]         - Pointer to TxBuffer
 * @param[in]         - Length of data to be Send
 *
 * @return            - none
 *
 * @Note              - none

 */

void SPI_SendData(SPI_TypeDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait Until TXE Flag is Set
		while(SPI_GetFlagStatus(pSPIx, SPI_FLAG_TXE) == FLAG_RESET);

		//2 Check the DFF Bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//16-bit DFF
			//1. Load the Data into DR
			pSPIx->DR = *((uint16_t *)pTxBuffer);
			Len--;
			Len--;
			(uint16_t *)pTxBuffer++;
		}
		else
		{
			//8-bit DFF
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             -
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - Pointer to RxBuffer
 * @param[in]         - Length of data to be Received
 *
 * @return            - none
 *
 * @Note              - none

 */

void SPI_ReceiveData(SPI_TypeDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait Until RXNE Flag is Set
		while(SPI_GetFlagStatus(pSPIx, SPI_FLAG_RXNE) == FLAG_RESET);

		//2 Check the DFF Bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//16-bit DFF
			//1. Read Data from DR to RxBuffer address
			*((uint16_t *)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pRxBuffer++;
		}
		else
		{
			//8-bit DFF
			*(pRxBuffer) = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
	}
}

/*********************************************************************
 * @fn      		  - SPI_PerpheralControl
 *
 * @brief             -
 *
 * @param[in]         - base address of the SPI peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */

void SPI_PerpheralControl(SPI_TypeDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}

}
