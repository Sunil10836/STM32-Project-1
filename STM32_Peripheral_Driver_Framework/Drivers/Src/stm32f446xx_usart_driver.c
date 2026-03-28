/*
 * stm32f446xx_usart_driver.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Sunil Sutar
 */


#include "stm32f446xx_usart_driver.h"

/*********************************************************************
 * @fn      		  - USART_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given USART
 *
 * @param[in]         - base address of the USART peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
void USART_PeriClockControl(USART_TypeDef_t *pUSARTx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		if(pUSARTx == USART1)
		{
			USART1_PCLK_EN();
		}
		else if(pUSARTx == USART2)
		{
			USART2_PCLK_EN();
		}
		else if(pUSARTx == USART3)
		{
			USART3_PCLK_EN();
		}
		else if(pUSARTx == UART4)
		{
			UART4_PCLK_EN();
		}
		else if(pUSARTx == UART5)
		{
			UART5_PCLK_EN();
		}
		else if(pUSARTx == USART6)
		{
			USART6_PCLK_EN();
		}
	}
	else
	{
		if(pUSARTx == USART1)
		{
			USART1_PCLK_DI();
		}
		else if(pUSARTx == USART2)
		{
			USART2_PCLK_DI();
		}
		else if(pUSARTx == USART3)
		{
			USART3_PCLK_DI();
		}
		else if(pUSARTx == UART4)
		{
			UART4_PCLK_DI();
		}
		else if(pUSARTx == UART5)
		{
			UART5_PCLK_DI();
		}
		else if(pUSARTx == USART6)
		{
			USART6_PCLK_DI();
		}
	}
}

void USART_SetBaudRate(USART_TypeDef_t *pUSARTx, uint32_t BaudRate)
{

	  //Variable to hold the APB clock
	  uint32_t PCLKx;

      uint32_t usartdiv;

	  //variables to hold Mantissa and Fraction values
	  uint32_t M_part,F_part;

	  uint32_t tempreg=0;

	  //Get the value of APB bus clock in to the variable PCLKx
	  if(pUSARTx == USART1 || pUSARTx == USART6)
	  {
		   //USART1 and USART6 are hanging on APB2 bus
		   PCLKx = RCC_GetPCLK2Value();
	  }
	  else
	  {
		   PCLKx = RCC_GetPCLK1Value();
	  }

	  //Check for OVER8 configuration bit
	  if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
	  {
		   //OVER8 = 1 , over sampling by 8
		   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	  }
	  else
	  {
		   //over sampling by 16
		   usartdiv = ((25 * PCLKx) / (4 *BaudRate));
	  }

	  //Calculate the Mantissa part
	  M_part = usartdiv/100;

	  //Place the Mantissa part in appropriate bit position . refer USART_BRR
	  tempreg |= M_part << 4;

	  //Extract the fraction part
	  F_part = (usartdiv - (M_part * 100));

	  //Calculate the final fractional
	  if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
	  {
		  //OVER8 = 1 , over sampling by 8
		  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);
	  }
	  else
	  {
		   //over sampling by 16
		   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);
	  }

	  //Place the fractional part in appropriate bit position . refer USART_BRR
	  tempreg |= F_part;

	  //copy the value of tempreg in to BRR register
	  pUSARTx->BRR = tempreg;
}


/*********************************************************************
 * @fn      		  - USART_Init
 *
 * @brief             - USART Initialization Function
 *
 * @param[in]         - pointer to USART Handle
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              -	none

 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
	uint32_t tempreg = 0;

	//Enable USART Clock
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	/**************** Configuration of USART_CR1 ***************************/
	//1. Configure USART_Mode
	if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		tempreg |= (1 << USART_CR1_TE);
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		tempreg |= (1 << USART_CR1_RE);
	}
	else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		tempreg |= ((1 << USART_CR1_TE) | (1 << USART_CR1_RE));
	}

	//2. Configure USART Word length
	tempreg |= (pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);

	//3. Configuration of parity control bit fields
	if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		//Enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//Enable EVEN parity
		 tempreg &= ~( 1 << USART_CR1_PS);
	}
	else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
	{
		//Enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//Enable Odd parity
		tempreg |= ( 1 << USART_CR1_PS);
	}

	 //Program the CR1 register
	pUSARTHandle->pUSARTx->CR1 |= tempreg;

	/**************** Configuration of USART_CR2 ***************************/

	tempreg=0;

	//Configuration of the number of stop bits
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

	//Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 |= tempreg;

	/**************** Configuration of USART_CR3 ***************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Enable RTS flow control
		tempreg |= ( 1 << USART_CR3_RTSE);
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Enable both CTS and RTS Flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}

	//Program the CR3 register
	pUSARTHandle->pUSARTx->CR3 |= tempreg;

	/**************** Configuration of USART_BRR ***************************/
	//pUSARTHandle->pUSARTx->BRR |= 0x8B;
	USART_SetBaudRate(pUSARTHandle->pUSARTx,pUSARTHandle->USART_Config.USART_Baudrate);

}

/*********************************************************************
 * @fn      		  - USART_DeInit
 *
 * @brief             - USART De-Initialization Function
 *
 * @param[in]         - base address of the USART peripheral
 * @param[in]         -
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */

void USART_DeInit(USART_TypeDef_t *pUSARTx)
{

}

/*********************************************************************
 * @fn      		  - USART_GetFlagStatus
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

uint8_t USART_GetFlagStatus(USART_TypeDef_t *pUSARTx, uint8_t FlagName)
{
	if(pUSARTx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - USART_SendData
 *
 * @brief             -
 *
 * @param[in]         - Base address of the USART peripheral
 * @param[in]         - Pointer to TxBuffer
 * @param[in]         - Length of data to be Send
 *
 * @return            - none
 *
 * @Note              - none

 */


void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t *pData;

	//Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

		//Check the USART_WordLength for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//Load the DR with 2-bytes masking  the bits other than first 9 bits
			pData = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR |= (*pData & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//Increment pTxBuffer
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer. so 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				//pUSARTHandle->pUSARTx->DR |= (*pData & (uint16_t)0xFF);

				//Increment pTxBuffer
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}

void USART_SendString(USART_Handle_t *pUSARTHandle, char *str)
{
	USART_SendData(pUSARTHandle, (uint8_t *)str, strlen(str));
}

uint8_t USART_ReceiveChar(USART_Handle_t *pUSARTHandle)
{
    // Wait until RXNE flag is set
    while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

    return (uint8_t)(pUSARTHandle->pUSARTx->DR & 0xFF);
}


/*********************************************************************
 * @fn      		  - USART_ReceiveData
 *
 * @brief             -
 *
 * @param[in]         - base address of the USART peripheral
 * @param[in]         - Pointer to RxBuffer
 * @param[in]         - Length of data to be Received
 *
 * @return            - none
 *
 * @Note              - none

 */

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Wait until RXNE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

		//Check the USART_WordLength for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//9-Bits Data

			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//9-bits Data
				//read only first 9 bits so mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

				//Increment pRxBuffer
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer. so 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

				//Increment pRxBuffer
				pRxBuffer++;
			}
		}
		else
		{
			//8-Bits Data

			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data
				//read 8 bits from DR
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
			}
			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity
				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);

			}
		}
	}
}

/*********************************************************************
 * @fn      		  - USART_PerpheralControl
 *
 * @brief             -
 *
 * @param[in]         - base address of the USART peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            - none
 *
 * @Note              - none

 */
void USART_PerpheralControl(USART_TypeDef_t *pUSARTx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	}
	else
	{
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
	}
}
