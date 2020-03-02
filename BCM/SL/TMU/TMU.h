
/*
 * TMU
 * TMU.h
 *
 * Created: 1/11/2020 6:22:22 PM
 *  Author: ahmad
 */ 

#ifndef TMU_H_
#define TMU_H_



/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "TMU_Cfg.h"
#include "TMU_LCfg.h"

/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define ONE_TIME 0U
#define PERIODIC 1U
#define REMOVE   2U

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

#define		TMU_CH0					0
#define		TMU_CH1					1
#define		TMU_CH2					2



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
 * Description: This function initializes the TMU module.
 * 							
 */
ERROR_STATUS TMU_Init(void);


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function deinitializes the TMU module.
 * 							
 */
ERROR_STATUS TMU_Deinit(void);


/**
 * Input: 
 *		 FUNC_PTR: pointer to function.
 *		 period: periodicity of the function.
 *		 repeat: ONE_TIME or PERIODIC.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function adds the function to the existing queue.
 * 							
 */
ERROR_STATUS TMU_Start(void(*FUNC_PTR)(void), uint16 u16_period, uint8 u8_repeat);


/**
 * Input: 
 *		 FUNC_PTR: pointer to function.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function removes the function from the existing queue.
 * 							
 */
ERROR_STATUS TMU_Stop(void(*FUNC_PTR)(void));


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function checks if any function is due and calls it.
 * 							
 */
ERROR_STATUS TMU_Dispatcher(void);
#endif