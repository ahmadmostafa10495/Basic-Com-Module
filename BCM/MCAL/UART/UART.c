/*
 * UART.c
 *
 * Created: 12/28/2019 1:18:28 PM
 *  Author: ahmad
 */ 

/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "UART.h"
#include "common_macros.h"
#include "std_types.h"
#include "interrupt.h"
#include "registers.h"
#include "DIO.h"
#if DEBUGTEST
#include "Char_lcd.h"
#include "softwareDelay.h"
#endif


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define SIXTEEN					(16U)
#define ONE						(1U)
#define EIGHT					(8U)
#define EIGHTY_INITIALIATION    (0x80)

/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/

void (*Transmitter_Cbk_ptr)(void) = NULL;
void (*Receiver_Cbk_ptr)(void) = NULL;
void (*DataRegisterEmpty_Cbk_ptr)(void) = NULL;
static uint8 gu8_interrupt_mode = UART_POLLING;

/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/



/** 
 * \brief UART_Init : 
 *
 * \param *pUART_cfg_s : A pointer to a struct that holds the desired UART configuration
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/     
extern ERROR_STATUS UART_Init(UART_cfg_s *pUART_cfg_s)
{
	if (NULL == pUART_cfg_s)
	{
		return E_NOK;
	} 
	else
	{
		DIO_Cfg_s DIO_info_TX = {UART_TX_GPIO, UART_TX_BIT, OUTPUT};
		DIO_init(&DIO_info_TX);
		DIO_Cfg_s DIO_info_RX = {UART_RX_GPIO, UART_RX_BIT, INPUT};
		DIO_init(&DIO_info_RX);
		if (NULL != pUART_cfg_s->Transmit_Cbk_ptr)
		{
			Transmitter_Cbk_ptr = pUART_cfg_s->Transmit_Cbk_ptr;
		} 
		else
		{
			
		}
		if (NULL != pUART_cfg_s->Resceive_Cbk_ptr)
		{
			Receiver_Cbk_ptr = pUART_cfg_s->Resceive_Cbk_ptr;
		}
		else
		{
			
		}
		if (NULL != pUART_cfg_s->DataRegisterEmpty_Cbk_ptr)
		{
			DataRegisterEmpty_Cbk_ptr = pUART_cfg_s->DataRegisterEmpty_Cbk_ptr;
		}
		else
		{
			
		}
		switch(pUART_cfg_s->u8_DoubleSpeed)
		{
			case UART_NO_DOUBLE_SPEED:
				UART_CSRA &= ~UART_U2X; 
				UART_BRRL = (uint8) ((F_CPU / (SIXTEEN * pUART_cfg_s->u32_BaudRate)) - ONE);
				UART_BRRH = (uint8) (((F_CPU / (SIXTEEN * pUART_cfg_s->u32_BaudRate)) - ONE)>>EIGHT);
				break;
			case UART_DOUBLE_SPEED:
				UART_CSRA |= UART_U2X;
				UART_BRRL = (uint8) ((F_CPU / (EIGHT * pUART_cfg_s->u32_BaudRate)) - ONE);
				UART_BRRH = (uint8) (((F_CPU / (EIGHT * pUART_cfg_s->u32_BaudRate)) - ONE)>>EIGHT);
				break;
			default:
				return E_NOK;
		}		
		switch(pUART_cfg_s->u8_DesiredOperation)
		{
			case TRANSMITTER:
				switch(pUART_cfg_s->u8_InterruptMode)
				{
					case UART_INTERRUPT:
					UART_CSRB |= UART_TXCIE;
					gu8_interrupt_mode = UART_INTERRUPT;
					break;
					case UART_POLLING:
					UART_CSRB &= ~(UART_DRIE|UART_TXCIE|UART_RXCIE);
					gu8_interrupt_mode = UART_POLLING;
					break;
					default:
					return E_NOK;
				}
				UART_CSRB |= UART_TXEN;
				break;
			case RECEIVER:
				switch(pUART_cfg_s->u8_InterruptMode)
				{
					case UART_INTERRUPT:
					UART_CSRB |= UART_RXCIE;
					gu8_interrupt_mode = UART_INTERRUPT;
					break;
					case UART_POLLING:
					UART_CSRB &= ~(UART_DRIE|UART_TXCIE|UART_RXCIE);
					gu8_interrupt_mode = UART_POLLING;
					break;
					default:
					return E_NOK;
				}
				UART_CSRB |= UART_RXEN;
				break;			
			case TRANSCEIVER:
				switch(pUART_cfg_s->u8_InterruptMode)
				{
					case UART_INTERRUPT:
					UART_CSRB |= UART_TXCIE|UART_RXCIE;
					gu8_interrupt_mode = UART_INTERRUPT;
					break;
					case UART_POLLING:
					UART_CSRB &= ~(UART_DRIE|UART_TXCIE|UART_RXCIE);
					gu8_interrupt_mode = UART_POLLING;
					break;
					default:
					return E_NOK;
				}
				UART_CSRB |= UART_TXEN|UART_RXEN;
				break;
			default:
				return E_NOK;
		}
		switch(pUART_cfg_s->u8_ParityBit)
		{
			case UART_NO_PARITY:
				/*   DEFAULT BY ZERO                          */
				break;
			case UART_EVEN_PARITY:
				UART_CSRC |= UART_RSEL|UART_PM1;
				break;
			case UART_ODD_PARITY:
				UART_CSRC |= UART_RSEL|UART_PM1|UART_PM0;
				break;
			default:
				return E_NOK;		
		}
		switch(pUART_cfg_s->u8_StopBit)
		{
			case UART_ONE_STOP_BIT:
				/*   DEFAULT BY ZERO                          */
				break;
			case UART_TWO_STOP_BIT:
				UART_CSRC |= UART_RSEL|UART_SBS;
				break;
			default:
				return E_NOK;	
		}
		switch(pUART_cfg_s->u8_DataSize)
		{
			case UART_5_BIT:
				/*   DEFAULT BY ZERO                          */
				break;
			case UART_6_BIT:
				UART_CSRC |= UART_RSEL|UART_CSZ0;
				break;
			case UART_7_BIT:
				UART_CSRC |= UART_RSEL|UART_CSZ1;
				break;
			case UART_8_BIT:
				UART_CSRC |= UART_RSEL|UART_CSZ0|UART_CSZ1;
				break;
			case UART_9_BIT:
				UART_CSRC |= UART_RSEL|UART_CSZ0|UART_CSZ1;
				UART_CSRB |= UART_CSZ2;
				break;
			default:
				return E_NOK;
		}
		
	}
	return E_OK;
}

/** 
 * \brief UART_Deinit : Deinitialize the UART module
 *
 * \param void
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/     
extern ERROR_STATUS UART_Deinit(void)
{
	Transmitter_Cbk_ptr = NULL;
	Receiver_Cbk_ptr = NULL;
	DataRegisterEmpty_Cbk_ptr = NULL;
	UART_CSRA = ZERO_INITIALIZATION;
	UART_CSRB = ZERO_INITIALIZATION;
	UART_CSRC = EIGHTY_INITIALIATION;
	UART_BRRH = ZERO_INITIALIZATION;
	UART_BRRL = ZERO_INITIALIZATION;
	DIO_Cfg_s DIO_info_TX = {UART_TX_GPIO, UART_TX_BIT, INPUT};
	DIO_init(&DIO_info_TX);
	DIO_Cfg_s DIO_info_RX = {UART_RX_GPIO, UART_RX_BIT, INPUT};
	DIO_init(&DIO_info_RX);
	gu8_interrupt_mode = UART_POLLING;
	return E_OK;
	
}

/** 
 * \brief  UART_SendByte : Sends a certain byte
 *
 * \param u8_Data : The byte the user wants to send
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/  
extern ERROR_STATUS UART_SendByte(uint8 u8_Data)
{
	switch(gu8_interrupt_mode)
	{
		case UART_POLLING:
			while (!( UART_CSRA & UART_DRE));
			UART_DR = u8_Data;
			break;
		case UART_INTERRUPT:
			UART_DR = u8_Data;
			break;	
	}

	return E_OK;
}

/** 
 * \brief  UART_ReceiveByte : Receives a certain byte
 *
 * \param pu8_ReceivedData : The sent byte will be stored in the passed pointer 
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/  
extern ERROR_STATUS UART_ReceiveByte(uint8 *pu8_ReceivedData)
{
	switch(gu8_interrupt_mode)
	{
		case UART_POLLING:
			while (!(UART_CSRA & UART_RXC));
			*pu8_ReceivedData = UART_DR;
			break;
		case UART_INTERRUPT:
			*pu8_ReceivedData = UART_DR;
			break;
	}
	return E_OK;
}

ISR(USART_TXC_vect)
{
	if (NULL != Transmitter_Cbk_ptr)
	{
		Transmitter_Cbk_ptr();
	} 
	else
	{
		
	}
}

ISR(USART_UDRE_vect)
{
	if (NULL != DataRegisterEmpty_Cbk_ptr)
	{
		DataRegisterEmpty_Cbk_ptr();
	}
	else
	{
		
	}
}

ISR(USART_RXC_vect)
{
	if (NULL != Receiver_Cbk_ptr)
	{
		Receiver_Cbk_ptr();
	}
	else
	{
		
	}
}



