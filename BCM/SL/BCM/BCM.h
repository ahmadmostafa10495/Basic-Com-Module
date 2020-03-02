
/*
 * BCM
 * BCM.h
 *
 * Created: 1/13/2020 2:04:22 PM
 *  Author: ahmad
 */ 
#ifndef BCM_H_
#define BCM_H_


/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "BCM_Cfg.h"
#include "BCM_LCfg.h"
#include "retval.h"
#include "std_types.h"
#include "common_macros.h"

/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define BCM_UART		(0U)
#define BCM_SPI			(1U)

/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*																						  *
*																						  *
*******************************************************************************************/




/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/





/******************************************************************************************
*                                                                                         *
*                                 PROTOTYPES                                              *
*																						  *
*																						  *
*******************************************************************************************/

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the BCM module.
 * 							
 */
ERROR_STATUS BCM_Init(void);

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function deinitializes the BCM module.
 * 							
 */
ERROR_STATUS BCM_Deinit(void);

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
ERROR_STATUS BCM_Send(uint8 u8_BCM_ID, uint16 u16_BCM_Size, uint8 *u8ptr_BCM_Data);

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
ERROR_STATUS BCM_Setup_Receive(uint8 u8_BCM_ID, uint16 u16_BCM_Size, uint8 *u8ptr_BCM_Data);

/**
 * Input: 
 *		 u8_BCM_ID: the channel id.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function unlocks the receiving ability for the desired channel.
 * 							
 */
ERROR_STATUS BCM_RxUnlock(uint8 u8_BCM_ID);

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the state machine for the Rx.
 * 							
 */
ERROR_STATUS BCM_RxDispatcher(void);

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the state machine for the Tx.
 * 							
 */
ERROR_STATUS BCM_TxDispatcher(void);
#endif