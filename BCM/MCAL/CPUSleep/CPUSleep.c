
/*
 * CPUSleep.c
 *
 * Created: 1/12/2020 1:08:30 PM
 *  Author: ahmad
 */ 


/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "CPUSleep.h"
#include "registers.h"



/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/


#define CPUSLEEP_UNINIT 0U
#define CPUSLEEP_INIT 1U


#define SM0 (0x10)
#define SM1 (0x20)
#define SM2 (0x40)
#define SE (0x80)
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

static uint8 CPUSleep_status = CPUSLEEP_UNINIT;
static uint8 gu8_CPUSleep_mode = CPUSLEEP_IDLE_MODE;

/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the CPUSleep module.
 * 							
 */
ERROR_STATUS 
CPUSleep_Init(void)
{
	CPUSleep_status = CPUSLEEP_INIT;
	return E_OK;
}


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function deinitializes the CPUSleep module.
 * 							
 */
ERROR_STATUS 
CPUSleep_Deinit(void)
{
	MCUCR &= ~SE;
	CPUSleep_status = CPUSLEEP_UNINIT;
	return E_OK;
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function disables the Sleep Enable bit.
 * 							
 */
ERROR_STATUS 
CPUSleep_disable(void)
{
	MCUCR &= ~SE;
	return E_OK;
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function enables the Sleep Enable bit.
 * 							
 */
ERROR_STATUS 
CPUSleep_enable(void)
{
	MCUCR |= SE;
	asm("SLEEP");
	return E_OK;
}

/**
 * Input:
 *		 mode: chooses the sleep mode. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the CPUSleep module.
 * 							
 */
ERROR_STATUS 
CPUSleep_setMode(uint8 mode)
{
	gu8_CPUSleep_mode = mode;
	MCUCR &= 0x0F;
	MCUCR |= gu8_CPUSleep_mode;
	return E_OK;
}