
/*
 * CPUSleep.h
 *
 * Created: 1/12/2020 1:10:06 PM
 *  Author: ahmad
 */ 
#ifndef CPU_SLEEP_H_
#define CPU_SLEEP_H_




/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "retval.h"

/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define CPUSLEEP_IDLE_MODE (0x00)
#define CPUSLEEP_ADC_NOISE_REDUCTION_MODE (0x10)
#define CPUSLEEP_POWER_DOWN_MODE (0x20)
#define CPUSLEEP_POWER_SAVE_MODE (0x30)
#define CPUSLEEP_STANDBY_MODE (0x60)
#define CPUSLEEP_EXTENDED_STANDBY_MODE (0x70)


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
 * Description: This function initializes the CPUSleep module.
 * 							
 */
ERROR_STATUS CPUSleep_Init(void);


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function deinitializes the CPUSleep module.
 * 							
 */
ERROR_STATUS CPUSleep_Deinit(void);

/**
 * Input:
 *		 mode: chooses the sleep mode. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function sets the sleep mode.
 * 							
 */
ERROR_STATUS CPUSleep_setMode(uint8 mode);

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function enables the Sleep Enable bit.
 * 							
 */
ERROR_STATUS CPUSleep_enable(void);

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function disables the Sleep Enable bit.
 * 							
 */
ERROR_STATUS CPUSleep_disable(void);

#endif