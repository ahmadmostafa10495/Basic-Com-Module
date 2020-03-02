
/*
 * TMU
 * TMU.c
 *
 * Created: 1/11/2020 6:22:08 PM
 *  Author: ahmad
 */ 



/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/
#include "Timer.h"
#include "interrupt.h"
#include "TMU.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define TMU_INIT 0U
#define TMU_UNINIT 1U

#define STARTED 0U
#define NOT_STARTED 1U



/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*																						  *
*																						  *
*******************************************************************************************/


typedef struct {
	uint16 u16_counter;
	uint16 u16_period;
	void(*Func_PTR)(void);
	uint8 u8_repeat;
	}str_TMU_Request_t;

/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/


static uint8 gu8_TMU_Status = TMU_UNINIT;
static uint8 gu8_TMU_TIMER_CH = 0;
static uint16 gu16_TMU_TimerCount = 0;
static uint8 gu8_TMU_DispatcherFlag = 0;
static str_TMU_Request_t gastr_TMU_Buffer[MAX_BUFFER_SIZE];
static uint8 gu8_bufferCounter = 0;
static uint8 gu8_timerStarted = NOT_STARTED;
/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/

void TIMER_CBK(void);




/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the TMU module.
 * 							
 */
ERROR_STATUS 
TMU_Init(void)
{
	uint8 u8_retval = TMU_BASE_ERR + SUCCESS;
	if (gu8_TMU_Status == TMU_UNINIT)
	{
		/* TIMER INIT*/
		Timer_cfg_s Timer_info;
		switch(TMU_Info.u8_TMU_TimerID)
		{
			case TMU_CH0:
				Timer_info.Timer_CH_NO = TIMER_CH0;
				gu8_TMU_TIMER_CH = TIMER_CH0;
				break;
			case TMU_CH1:
				Timer_info.Timer_CH_NO = TIMER_CH1;
				gu8_TMU_TIMER_CH = TIMER_CH1;
				break;	
			case TMU_CH2:
				Timer_info.Timer_CH_NO = TIMER_CH2;
				gu8_TMU_TIMER_CH = TIMER_CH2;
				break;			 
		} 
		Timer_info.Timer_Mode = TIMER_MODE;
		Timer_info.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		Timer_info.Timer_Prescaler = TIMER_PRESCALER_1024;
		Timer_info.Timer_Cbk_ptr = TIMER_CBK;
		Timer_Init(&Timer_info);
		gu16_TMU_TimerCount = (TMU_Info.u8_TMU_Resolution * 16000U) / 1024U;
		gu8_TMU_Status = TMU_INIT;
	}
	else
	{
		u8_retval = TMU_BASE_ERR + ALREADY_INITIALIZED_ERR;
	}
	return u8_retval;
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function deinitializes the TMU module.
 * 							
 */
ERROR_STATUS 
TMU_Deinit(void)
{
	uint8 u8_retval = TMU_BASE_ERR + SUCCESS;
	if (gu8_TMU_Status == TMU_INIT)
	{
		/* TIMER DEINIT and module variables returning to unused values*/
		Timer_Stop(gu8_TMU_TIMER_CH);
		gu8_timerStarted = NOT_STARTED;
		gu8_bufferCounter = 0;
		gu8_TMU_DispatcherFlag = 0;
		gu8_TMU_Status = TMU_UNINIT;
	} 
	else
	{
		u8_retval = TMU_BASE_ERR + NOT_INITIALIZED_ERR;
	}

	return u8_retval;
}

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
ERROR_STATUS 
TMU_Start(void(*FUNC_PTR)(void), uint16 u16_period, uint8 u8_repeat)
{
	uint8 u8_retval = TMU_BASE_ERR + SUCCESS;
	if (gu8_TMU_Status == TMU_INIT)
	{
		if (NULL != FUNC_PTR)
		{
			/* adding func to buffer*/
			str_TMU_Request_t TMU_Request = {0, u16_period, FUNC_PTR, u8_repeat};
			gastr_TMU_Buffer[gu8_bufferCounter] = TMU_Request;
			gu8_bufferCounter++;
		}
		else
		{
			return E_NOK;
		}
		if (gu8_timerStarted == NOT_STARTED)
		{
			/*getting the module into starting state "happens first time only"*/
			Timer_Start(gu8_TMU_TIMER_CH, gu16_TMU_TimerCount);
			gu8_timerStarted = STARTED;
		} 
		else
		{
			
		}
	} 
	else
	{
		u8_retval = TMU_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}


/**
 * Input: 
 *		 FUNC_PTR: pointer to function.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function removes the function from the existing queue.
 * 							
 */
ERROR_STATUS 
TMU_Stop(void(*FUNC_PTR)(void))
{
	uint8 u8_retval = TMU_BASE_ERR + SUCCESS;
	if (gu8_TMU_Status == TMU_INIT)
	{
		if (NULL != FUNC_PTR)
		{
			uint8 i;
			for (i = 0; i < gu8_bufferCounter; i++)
			{
				if(gastr_TMU_Buffer[i].Func_PTR == FUNC_PTR)
				{
					/*removing func from buffer*/
					gastr_TMU_Buffer[i] = gastr_TMU_Buffer[gu8_bufferCounter];
					gu8_bufferCounter--;
					break;
				}
				else
				{
					
				}
			}
		}
		else
		{
			u8_retval = TMU_BASE_ERR + NULL_PTR_ERR;
		}
		if (gu8_bufferCounter == 0 && u8_retval == TMU_BASE_ERR + SUCCESS)
		{
			/* getting module into stop state "happens when buffer is empty only"*/
			Timer_Stop(gu8_TMU_TIMER_CH);
			gu8_timerStarted = NOT_STARTED;
		}
		else
		{
			
		}
	}
	else
	{
		u8_retval = TMU_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function checks if any function is due and calls it.
 * 							
 */
ERROR_STATUS 
TMU_Dispatcher(void)
{
	uint8 u8_retval = TMU_BASE_ERR + SUCCESS;
	if (TMU_INIT == gu8_TMU_Status)
	{
		if (1 == gu8_TMU_DispatcherFlag)
		{
			uint8 i;
			for (i = 0; i < gu8_bufferCounter; i++)
			{
				/* increamenting func counter */
				gastr_TMU_Buffer[i].u16_counter++;
				if (gastr_TMU_Buffer[i].u16_counter == gastr_TMU_Buffer[i].u16_period)
				{
					/* excuting func when the delay period ends*/
					gastr_TMU_Buffer[i].Func_PTR();
					gastr_TMU_Buffer[i].u16_counter = 0;
					if (ONE_TIME == gastr_TMU_Buffer[i].u8_repeat)
					{
						/* removing the one shot func after excution */
						gastr_TMU_Buffer[i] = gastr_TMU_Buffer[gu8_bufferCounter];
						gu8_bufferCounter--;
						i--;
					} 
					else
					{
						
					}
				} 
				else
				{
					
				}
			}
			if (gu8_bufferCounter == 0)
			{
				/* getting module into stop state "happens when buffer is empty only"
				 in this case it may happen due to having only ONESHOT funcs in the buffer*/
				Timer_Stop(gu8_TMU_TIMER_CH);
				gu8_timerStarted = NOT_STARTED;
			}
			else
			{
						
			}
			gu8_TMU_DispatcherFlag = 0;
		} 
		else
		{
			
		}
	} 
	else
	{
		u8_retval = TMU_BASE_ERR + NOT_INITIALIZED_ERR;
	}
	return u8_retval;
}

void 
TIMER_CBK(void)
{
	/* resetting the timer */
	Timer_Start(gu8_TMU_TIMER_CH, gu16_TMU_TimerCount);
	gu8_TMU_DispatcherFlag = 1;
}