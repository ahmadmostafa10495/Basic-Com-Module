
/*
 * BCM
 * BCM.c
 *
 * Created: 1/13/2020 2:04:37 PM
 *  Author: ahmad
 */ 


/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "BCM.h"
#include "UART.h"
#include "DIO.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define BCM_INIT	(0U)
#define BCM_UNINIT	(1U)

#define BCM_BUFFER_LOCKED		(0U)
#define BCM_BUFFER_UNLOCKED		(1U)

#define BCM_STATE_IDLE			(0U)
#define BCM_STATE_TX			(1U)
#define BCM_STATE_RX			(2U)
#define BCM_STATE_TX_COMPLETE	(3U)
#define BCM_STATE_RX_COMPLETE	(4U)

#define BCM_SEND_ID				(0U)
#define BCM_SEND_SIZE_HIGH		(1U)
#define BCM_SEND_SIZE_LOW		(2U)
#define BCM_SEND_DATA			(3U)
#define BCM_SEND_CHECKSUM		(4U)
#define BCM_RECEIVE_ID			(5U)
#define BCM_RECEIVE_SIZE_HIGH	(6U)
#define BCM_RECEIVE_SIZE_LOW	(7U)
#define BCM_RECEIVE_DATA		(8U)
#define BCM_RECEIVE_CHECKSUM	(9U)
#define BCM_SEND_END			(10U)
#define BCM_RECEIVE_END			(11U)
/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*																						  *
*																						  *
*******************************************************************************************/

typedef struct  
{
	uint8 u8_BCM_ID;
	uint16 u16_BCM_Size;
	uint8 *u8ptr_BCM_Data;
	uint8 u8_BCM_CheckSum;
}str_BCM_Buffer_t;

/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/

static uint8 gu8_BCM_Status = BCM_UNINIT;
static str_BCM_Buffer_t gastr_BCM_Buffer[BCM_MAX_CH_NUM];
static uint8 gau8_BCM_BufferStatus[BCM_MAX_CH_NUM];
static uint8 gau8_BCM_StateMachineStatus[BCM_MAX_CH_NUM];
static uint8 gau8_BCM_TranceivingStatus[BCM_MAX_CH_NUM];
static uint16 gau16_BCM_SizeCounter[BCM_MAX_CH_NUM];
static uint8 gu8_TempData[BCM_MAX_CH_NUM];
static uint8 gu16_ReceivedSize[BCM_MAX_CH_NUM];

/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/


void(BCM_ReceiveUART_Cbk)(void);
void(BCM_SendUART_Cbk)(void);

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the BCM module.
 * 							
 */
ERROR_STATUS 
BCM_Init(void)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if (BCM_UNINIT == gu8_BCM_Status)
	{
		uint8 u8_index;
		for (u8_index = 0; u8_index < BCM_MAX_CH_NUM; u8_index++)
		{
			/* initial values for module elements */
			gastr_BCM_Buffer[u8_index].u8_BCM_ID = 0;
			gastr_BCM_Buffer[u8_index].u16_BCM_Size = 0;
			gastr_BCM_Buffer[u8_index].u8ptr_BCM_Data = NULL;
			gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum = 0;
			gau8_BCM_BufferStatus[u8_index] = BCM_BUFFER_UNLOCKED;
			gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_IDLE;
			gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_ID;
			gau16_BCM_SizeCounter[u8_index] = 0;
			gu8_TempData[u8_index] = 0;
			gu16_ReceivedSize[u8_index] = 0;
		}
		gu8_BCM_Status = BCM_INIT;
	} 
	else
	{
		u8_retval = BCM_BASE_ERR + ALREADY_INITIALIZED_ERR;
	}
	return u8_retval;
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function deinitializes the BCM module.
 * 							
 */
ERROR_STATUS 
BCM_Deinit(void)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if (BCM_INIT == gu8_BCM_Status)
	{
		uint8 u8_index;
		for (u8_index = 0; u8_index < BCM_MAX_CH_NUM; u8_index++)
		{
			/* initial values for module elements */
			gastr_BCM_Buffer[u8_index].u8_BCM_ID = 0;
			gastr_BCM_Buffer[u8_index].u16_BCM_Size = 0;
			gastr_BCM_Buffer[u8_index].u8ptr_BCM_Data = NULL;
			gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum = 0;
			gau8_BCM_BufferStatus[u8_index] = BCM_BUFFER_UNLOCKED;
			gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_IDLE;
			gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_ID;
			gau16_BCM_SizeCounter[u8_index] = 0;
			gu8_TempData[u8_index] = 0;
			gu16_ReceivedSize[u8_index] = 0;
		}
		gu8_BCM_Status = BCM_UNINIT;
	}
	else
	{
		u8_retval = BCM_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;	
}

/**
 * Input: 
 *		 u8_BCM_ID: the channel id.
 *		 u8_BCM_Size: the buffer size.
 *		 u8ptr_BCM_Data: pointer to desired data buffer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts sending data through desired channel.
 * 							
 */
ERROR_STATUS 
BCM_Send(uint8 u8_BCM_ID, uint16 u16_BCM_Size, uint8 *u8ptr_BCM_Data)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if (BCM_INIT == gu8_BCM_Status)
	{
		if (u8_BCM_ID < BCM_MAX_CH_NUM)
		{
			if (BCM_BUFFER_UNLOCKED == gau8_BCM_BufferStatus[u8_BCM_ID])
			{
				switch(u8_BCM_ID)
				{
					case BCM_UART:
						/* uart init
						 * module state init
						 * send state init
						 * buffer state init */
						gastr_BCM_Buffer[u8_BCM_ID].u8_BCM_ID = u8_BCM_ID;
						gastr_BCM_Buffer[u8_BCM_ID].u16_BCM_Size = u16_BCM_Size;
						gastr_BCM_Buffer[u8_BCM_ID].u8ptr_BCM_Data = u8ptr_BCM_Data;
						gastr_BCM_Buffer[u8_BCM_ID].u8_BCM_CheckSum = 0;
						gau8_BCM_TranceivingStatus[u8_BCM_ID] = BCM_SEND_ID;
						gau8_BCM_StateMachineStatus[u8_BCM_ID] = BCM_STATE_TX_COMPLETE;
						gau8_BCM_BufferStatus[u8_BCM_ID] = BCM_BUFFER_LOCKED;
						UART_cfg_s UART_Info = {UART_INTERRUPT, TRANSMITTER, UART_NO_DOUBLE_SPEED, UART_ONE_STOP_BIT,\
												UART_NO_PARITY, UART_8_BIT, 9600, BCM_SendUART_Cbk, NULL, NULL};
						UART_Init(&UART_Info);
						break;
					case BCM_SPI:
						u8_retval = BCM_BASE_ERR + NOT_SUPPORTED_ERR;
						break;
					default:
						break;
				}
			} 
			else
			{
				u8_retval = BCM_BASE_ERR + RESOURCE_NOT_AVAILABLE_ERR;
			}
		} 
		else
		{
			u8_retval = BCM_BASE_ERR + RESOURCE_NOT_FOUND_ERR;
		}
	}
	else
	{
		u8_retval = BCM_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}

/**
 * Input: 
 *		 u8_BCM_ID: the channel id.
 *		 u8_BCM_Size: the buffer size.
 *		 u8ptr_BCM_Data: pointer to desired data buffer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts receiving data through desired channel.
 * 							
 */
ERROR_STATUS 
BCM_Setup_Receive(uint8 u8_BCM_ID, uint16 u16_BCM_Size, uint8 *u8ptr_BCM_Data)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if (BCM_INIT == gu8_BCM_Status)
	{
		if (u8_BCM_ID < BCM_MAX_CH_NUM)
		{
			if (BCM_BUFFER_UNLOCKED == gau8_BCM_BufferStatus[u8_BCM_ID])
			{
				switch(u8_BCM_ID)
				{
					case BCM_UART:
						/* uart init
						 * module state init
						 * receive state init
						 * buffer state init */
						gastr_BCM_Buffer[u8_BCM_ID].u8_BCM_ID = u8_BCM_ID;
						gastr_BCM_Buffer[u8_BCM_ID].u16_BCM_Size = u16_BCM_Size;
						gastr_BCM_Buffer[u8_BCM_ID].u8ptr_BCM_Data = u8ptr_BCM_Data;
						gastr_BCM_Buffer[u8_BCM_ID].u8_BCM_CheckSum = 0;
						gau8_BCM_TranceivingStatus[u8_BCM_ID] = BCM_RECEIVE_ID;
						gau8_BCM_StateMachineStatus[u8_BCM_ID] = BCM_STATE_RX;
						gau8_BCM_BufferStatus[u8_BCM_ID] = BCM_BUFFER_LOCKED;
						UART_cfg_s UART_Info = {UART_INTERRUPT, RECEIVER, UART_NO_DOUBLE_SPEED, UART_ONE_STOP_BIT,\
						UART_NO_PARITY, UART_8_BIT, 9600, NULL, BCM_ReceiveUART_Cbk, NULL};
						UART_Init(&UART_Info);
						break;
					case BCM_SPI:
						u8_retval = BCM_BASE_ERR + NOT_SUPPORTED_ERR;
						break;
					default:
						break;
				}
			}
			else
			{
				u8_retval = BCM_BASE_ERR + RESOURCE_NOT_AVAILABLE_ERR;
			}
		}
		else
		{
			u8_retval = BCM_BASE_ERR + RESOURCE_NOT_FOUND_ERR;
		}
	}
	else
	{
		u8_retval = BCM_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}

/**
 * Input: 
 *		 u8_BCM_ID: the channel id.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function unlocks the receiving ability for the desired channel.
 * 							
 */
ERROR_STATUS 
BCM_RxUnlock(uint8 u8_BCM_ID)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if(BCM_INIT == gu8_BCM_Status)
	{
		if (u8_BCM_ID < BCM_MAX_CH_NUM)
		{
			/*unlocking desired resource*/
			gau8_BCM_BufferStatus[u8_BCM_ID] = BCM_BUFFER_UNLOCKED;
		} 
		else
		{
			u8_retval = BCM_BASE_ERR + RESOURCE_NOT_FOUND_ERR;
		}
	}
	else
	{
		u8_retval = BCM_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the state machine for the Rx.
 * 							
 */
ERROR_STATUS 
BCM_RxDispatcher(void)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if (BCM_INIT == gu8_BCM_Status)
	{
		uint8 u8_index;
		for (u8_index = 0; u8_index < BCM_MAX_CH_NUM; u8_index++)
		{
			if (BCM_BUFFER_LOCKED == gau8_BCM_BufferStatus[u8_index])
			{
				if (BCM_STATE_RX_COMPLETE == gau8_BCM_StateMachineStatus[u8_index])
				{
					switch(gau8_BCM_TranceivingStatus[u8_index])
					{
						case BCM_RECEIVE_ID:
							/* checking for BCM ID match */
							if (gastr_BCM_Buffer[u8_index].u8_BCM_ID == gu8_TempData[u8_index])
							{
								/* changing receive state */
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_SIZE_HIGH;
							}
							else
							{
									
							}
							/* changing module state */
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_RX;
							break;
						case BCM_RECEIVE_SIZE_HIGH:
							/* receiving first byte of size */
							gu16_ReceivedSize[u8_index] = gu8_TempData[u8_index];
							gu16_ReceivedSize[u8_index] = gu16_ReceivedSize[u8_index] << 8;
							/* changing receive state */
							gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_SIZE_LOW;
							/* changing module state */
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_RX;
							break;
						case BCM_RECEIVE_SIZE_LOW:
							/* receiving first byte of size */
							gu16_ReceivedSize[u8_index] += gu8_TempData[u8_index];
							/* changing receive state */
							gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_DATA;
							/* changing module state */
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_RX;
							break;
							if ((gastr_BCM_Buffer[u8_index].u16_BCM_Size) >= gu16_ReceivedSize[u8_index])
							{
								/* changing receive state */
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_DATA;
							}
							else
							{
								/* changing receive state */
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_END;
								u8_retval = BCM_BASE_ERR + BUFFER_NOT_ENOUGH_ERR;
							}
							/* changing module state */
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_RX;
							break;
						case BCM_RECEIVE_DATA:
							if (gau16_BCM_SizeCounter[u8_index] < gastr_BCM_Buffer[u8_index].u16_BCM_Size - 1)
							{
								/* receiving last byte of data */
								gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum += gu8_TempData[u8_index];
								/* changing receive state */
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_DATA;
								gastr_BCM_Buffer[u8_index].u8ptr_BCM_Data[gau16_BCM_SizeCounter[u8_index]] = gu8_TempData[u8_index];
								gau16_BCM_SizeCounter[u8_index]++;
								/* changing module state */
								gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_RX;
								break;
							}
							else
							{
								gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum += gu8_TempData[u8_index];
								/* changing receive state */
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_CHECKSUM;
								gastr_BCM_Buffer[u8_index].u8ptr_BCM_Data[gau16_BCM_SizeCounter[u8_index]] = gu8_TempData[u8_index];
								gau16_BCM_SizeCounter[u8_index]++;
								/* changing module state */
								gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_RX;
								break;
							}
						case BCM_RECEIVE_CHECKSUM:
							if (gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum == gu8_TempData[u8_index])
							{
/*
// 								DIO_Cfg_s DIO_Info = {GPIOB, PIN4, OUTPUT};
// 								DIO_init(&DIO_Info);
// 								DIO_Write(GPIOB, PIN4, HIGH);
*/
								/* changing receive state */
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_END;
							} 
							else
							{
								gau8_BCM_TranceivingStatus[u8_index] = BCM_RECEIVE_END;
								u8_retval = BCM_BASE_ERR + CHECKSUM_ERR;
							}
							/* changing module state */
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_IDLE;
							break;
						case BCM_RECEIVE_END:
							/*uart deinit*/
							UART_Deinit();
							break;
					}
				}
				else
				{
				
				}
			}
			else
			{
			
			}
		}
	}
	else
	{
		u8_retval = BCM_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the state machine for the Tx.
 * 							
 */
ERROR_STATUS 
BCM_TxDispatcher(void)
{
	uint8 u8_retval = BCM_BASE_ERR + SUCCESS;
	if (BCM_INIT == gu8_BCM_Status)
	{
		uint8 u8_index;
		for (u8_index = 0; u8_index < BCM_MAX_CH_NUM; u8_index++)
		{
			if (BCM_BUFFER_LOCKED == gau8_BCM_BufferStatus[u8_index])
			{
				if (BCM_STATE_TX_COMPLETE == gau8_BCM_StateMachineStatus[u8_index])
				{
					gu8_TempData[u8_index] = gastr_BCM_Buffer[u8_index].u8ptr_BCM_Data[gau16_BCM_SizeCounter[u8_index]];
					switch(gau8_BCM_TranceivingStatus[u8_index])
					{
						case BCM_SEND_ID:
							/* sending BCM ID*/
							gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_SIZE_HIGH;
							UART_SendByte(gastr_BCM_Buffer[u8_index].u8_BCM_ID);
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_TX;
							break;
						case BCM_SEND_SIZE_HIGH:
							/* sending first byte of size*/
							gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_SIZE_LOW;
							UART_SendByte((uint8)(gastr_BCM_Buffer[u8_index].u16_BCM_Size >> 8));
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_TX;
							break;
						case BCM_SEND_SIZE_LOW:
							/* sending last byte of size*/
							gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_DATA;
							UART_SendByte((uint8)(gastr_BCM_Buffer[u8_index].u16_BCM_Size));
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_TX;
							break;
						case BCM_SEND_DATA:
							if (gau16_BCM_SizeCounter[u8_index] < gastr_BCM_Buffer[u8_index].u16_BCM_Size - 1)
							{	
								/* sending data*/
								gau16_BCM_SizeCounter[u8_index]++;
								gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum += gu8_TempData[u8_index];
								gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_DATA;								
								UART_SendByte(gu8_TempData[u8_index]);
								gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_TX;
								break;
							} 
							else
							{
								/* sending last byte of data*/
								gau16_BCM_SizeCounter[u8_index]++;
								gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum += gu8_TempData[u8_index];
								gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_CHECKSUM;
								UART_SendByte(gu8_TempData[u8_index]);
								gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_TX;
								break;	
							}
						case BCM_SEND_CHECKSUM:
							/* sending checksum*/
							gau8_BCM_TranceivingStatus[u8_index] = BCM_SEND_END;
							UART_SendByte(gastr_BCM_Buffer[u8_index].u8_BCM_CheckSum);
							gau8_BCM_StateMachineStatus[u8_index] = BCM_STATE_IDLE;
							break;
						case BCM_SEND_END:
							/*unlocking the resource*/
							gau8_BCM_BufferStatus[u8_index] = BCM_BUFFER_UNLOCKED;
							/* uart deinit*/
							UART_Deinit();
							break;
					}
				}
				else
				{
					
				}
			}
			else
			{
				
			}
		}
	}
	else
	{
		u8_retval = BCM_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}



void(BCM_ReceiveUART_Cbk)(void)
{
	UART_ReceiveByte(&gu8_TempData[BCM_UART]);
	if (BCM_STATE_RX == gau8_BCM_StateMachineStatus[BCM_UART])
	{
		gau8_BCM_StateMachineStatus[BCM_UART] = BCM_STATE_RX_COMPLETE;
	}
	else
	{
		
	}
}
void(BCM_SendUART_Cbk)(void)
{
	if (BCM_STATE_TX == gau8_BCM_StateMachineStatus[BCM_UART])
	{
		gau8_BCM_StateMachineStatus[BCM_UART] = BCM_STATE_TX_COMPLETE;
	} 
	else
	{
		
	}
}