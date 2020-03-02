/*
 * Timer.c
 *
 * Created: 12/22/2019 2:01:37 PM
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


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/

#define SHIFT_ZERO						(0x00)
#define SHIFT_ONE						(0x01)
#define SHIFT_TWO						(0x02)
#define SHIFT_THREE						(0x03)
#define SHIFT_FOUR						(0x04)
#define SHIFT_FIVE						(0x05)
#define SHIFT_SIX						(0x06)
#define SHIFT_SEVEN						(0x07)
#define SHIFT_TIMER_INTERRUPT_MODE		(0x40)

#define TCNT0_MAX						(255)
#define TCNT1_MAX						(65535)
#define TCNT2_MAX						(255)

#define TIMER_PRESCALER_BITS			(0x07)

#define NO_OF_TIMERS					(3)


/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/


static uint8 gu8_TimerPrescaler[NO_OF_TIMERS] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};
static uint8 gu8_started_state[NO_OF_TIMERS] = {ZERO_INITIALIZATION, ZERO_INITIALIZATION, ZERO_INITIALIZATION};
void(*TIMER_OVF_CBK_PTR[NO_OF_TIMERS])(void) = {NULL,NULL,NULL};





/******************************************************************************************
*                                                                                         *
*                                 IMPLEMENTATION                                          *
*																						  *
*																						  *
*******************************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
ERROR_STATUS 
Timer_Init(Timer_cfg_s* Timer_cfg)
{
	switch(Timer_cfg->Timer_CH_NO)
	{  
		case (TIMER_CH0) :
		if (NULL != Timer_cfg->Timer_Cbk_ptr)
		{
			TIMER_OVF_CBK_PTR[TIMER_CH0] = Timer_cfg->Timer_Cbk_ptr;
		}
		else
		{
			
		}
		switch(Timer_cfg->Timer_Mode)
		{
			case(TIMER_MODE):
			TCCR0 = ZERO_INITIALIZATION;
			switch(Timer_cfg->Timer_Prescaler)
			{
				case(TIMER_PRESCALER_NO) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_ONE;
				break;
				case(TIMER_PRESCALER_8) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_TWO;
				break;
				case(TIMER_PRESCALER_64) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_THREE;
				break;
				case(TIMER_PRESCALER_256) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_FOUR;
				break;
				case(TIMER_PRESCALER_1024) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_FIVE;
				break;
				default :
				return E_NOK;
			}
			break;
			case (COUNTER_RISING_MODE):
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_SEVEN;
			break;
			
			case (COUNTER_FALLING_MODE):
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_SIX;
			break;
			
			default :
			return E_NOK;
		}
		switch(Timer_cfg->Timer_Polling_Or_Interrupt)
		{
			case(TIMER_INTERRUPT_MODE):
			TIMSK |= SHIFT_ONE;
			break;
			case(TIMER_POLLING_MODE):
			TIMSK |= ZERO_INITIALIZATION;
			break;
			default :
			return E_NOK;
		}
		break;
		case (TIMER_CH1) :
		if (NULL != Timer_cfg->Timer_Cbk_ptr)
		{
			TIMER_OVF_CBK_PTR[TIMER_CH1] = Timer_cfg->Timer_Cbk_ptr;
		}
		else
		{
			
		}
		switch(Timer_cfg->Timer_Mode)
		{
			case(TIMER_MODE):
			TCCR1 = ZERO_INITIALIZATION;
			switch(Timer_cfg->Timer_Prescaler)
			{
				case(TIMER_PRESCALER_NO) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_ONE;
				break;
				case(TIMER_PRESCALER_8) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_TWO;
				break;
				case(TIMER_PRESCALER_64) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_THREE;
				break;
				case(TIMER_PRESCALER_256) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_FOUR;
				break;
				case(TIMER_PRESCALER_1024) :
				gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_FIVE;
				break;
				default :
				return E_NOK;
			}
			break;
			
			case (COUNTER_RISING_MODE):
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_SEVEN;
			break;
			
			case (COUNTER_FALLING_MODE):
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_SIX;
			break;
			
			default :
			return E_NOK;
		}
		switch(Timer_cfg->Timer_Polling_Or_Interrupt)
		{
			case(TIMER_INTERRUPT_MODE ) :
			TIMSK |= SHIFT_FOUR;
			break;
			case(TIMER_POLLING_MODE):
			TIMSK |= ZERO_INITIALIZATION;
			break;
			default :
			return E_NOK;
		}
		break;
		case (TIMER_CH2) :
		if (NULL != Timer_cfg->Timer_Cbk_ptr)
		{
			TIMER_OVF_CBK_PTR[TIMER_CH2] = Timer_cfg->Timer_Cbk_ptr;
		}
		else
		{
			
		}
		switch(Timer_cfg->Timer_Mode)
		{
			case(TIMER_MODE):
			TCCR2 = ZERO_INITIALIZATION;
		switch(Timer_cfg->Timer_Prescaler)
		{
			case(TIMER_PRESCALER_NO) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_ONE;
			break;
			case(TIMER_PRESCALER_8) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_TWO;
			break;
			case(TIMER_PRESCALER_32) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_THREE;
			break;
			case(TIMER_PRESCALER_64) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_FOUR;
			break;
			case(TIMER_PRESCALER_128) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_FIVE;
			break;
			case(TIMER_PRESCALER_256) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_SIX;
			break;
			case(TIMER_PRESCALER_1024) :
			gu8_TimerPrescaler[Timer_cfg->Timer_CH_NO] = SHIFT_SEVEN;
			break;
			default :
			return E_NOK;
		}
			break;
			
			case (COUNTER_RISING_MODE):
			break;
			
			case (COUNTER_FALLING_MODE):
			break;
			
			default :
			return E_NOK;
		}
		switch(Timer_cfg->Timer_Polling_Or_Interrupt)
		{
			case(TIMER_INTERRUPT_MODE ) :
			TIMSK |= SHIFT_TIMER_INTERRUPT_MODE;
			break;
			case(TIMER_POLLING_MODE):
			TIMSK |= SHIFT_ZERO;
			break;
			default :
			return E_NOK;
		}
		break;
		default :
		return E_NOK;
		
	}
	return E_NOK;
}


/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts the needed timer.
 * 							
 */
ERROR_STATUS 
Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
		if (Timer_Count > TCNT0_MAX)
		{
			return E_NOK;
		}
		if (gu8_TimerPrescaler[Timer_CH_NO] == TIMER_NO_CLOCK)
		{
			return E_NOK;
		}
		else if ((gu8_TimerPrescaler[Timer_CH_NO] == SHIFT_SEVEN)||(gu8_TimerPrescaler[Timer_CH_NO] == SHIFT_SIX))
		{
			TCNT0 = ZERO_INITIALIZATION;
			TCCR0 |= gu8_TimerPrescaler[Timer_CH_NO];
		}
		else 
		{
			TCNT0 = TCNT0_MAX - Timer_Count;
			TCCR0 |= gu8_TimerPrescaler[Timer_CH_NO];
		}
		break;
		
		case TIMER_CH1:
		if (gu8_TimerPrescaler[Timer_CH_NO] == TIMER_NO_CLOCK)
		{
			return E_NOK;
		}
		else if ((gu8_TimerPrescaler[Timer_CH_NO] == SHIFT_SEVEN)||(gu8_TimerPrescaler[Timer_CH_NO] == SHIFT_SIX))
		{
			TCNT1 = ZERO_INITIALIZATION;
			TCCR1 |= gu8_TimerPrescaler[Timer_CH_NO];
		}
		else 
		{
			TCNT1 = TCNT1_MAX - Timer_Count;
			TCCR1 |= gu8_TimerPrescaler[Timer_CH_NO];
		}
		break;
		
		case TIMER_CH2:
		if (Timer_Count > TCNT2_MAX)
		{
			return E_NOK;
		}
		if (gu8_TimerPrescaler[Timer_CH_NO] == TIMER_NO_CLOCK)
		{
			return E_NOK;
		}
		else 
		{
			TCNT2 = TCNT2_MAX - Timer_Count;
			TCCR2 |= gu8_TimerPrescaler[Timer_CH_NO];
		}
		break;
		default:
		return E_NOK;
	}
	gu8_started_state[Timer_CH_NO] = ONE_INITIALIZATION;
	return E_OK;
}


/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */
ERROR_STATUS 
Timer_Stop(uint8 Timer_CH_NO){
	if (Timer_CH_NO>NO_OF_TIMERS - 1)
	{
		return E_NOK;
	}
	else
	{
		if (gu8_started_state[Timer_CH_NO] == ONE_INITIALIZATION)
		{
			switch (Timer_CH_NO)
			{
				case TIMER_CH0:
				TCCR0 &= ~(TIMER_PRESCALER_BITS);
				break;
				case TIMER_CH1:
				TCCR1 &= ~(TIMER_PRESCALER_BITS);
				break;
				case TIMER_CH2:
				TCCR2 &= ~(TIMER_PRESCALER_BITS);
				break;
				default:
				return E_NOK;
			}
			gu8_started_state[Timer_CH_NO] = ZERO_INITIALIZATION;
		}
		else
		{
			return E_NOK;
		}
		return E_OK;	
	}
}


/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 * 							
 */
ERROR_STATUS 
Timer_GetStatus(uint8 Timer_CH_NO, bool* Data)
{
	if (Timer_CH_NO > NO_OF_TIMERS - 1)
	{
		return E_NOK;
	}
	else
	{
		switch(Timer_CH_NO)
		{
			case TIMER_CH0:
				*Data = TIFR & (ONE_INITIALIZATION << SHIFT_ZERO);
				break;
			case TIMER_CH1:
				*Data = TIFR & (ONE_INITIALIZATION << SHIFT_TWO);
				break;
			case TIMER_CH2:
				*Data = TIFR & (ONE_INITIALIZATION << SHIFT_SIX);
				break;
			default:
				return E_NOK;			
		}
	}
	return E_OK;
}


/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */
ERROR_STATUS 
Timer_GetValue(uint8 Timer_CH_NO, uint16* Data)
{
	*Data = ZERO_INITIALIZATION;
	if (Timer_CH_NO > NO_OF_TIMERS - 1)
	{
		return E_NOK;
	}
	else
	{
		switch (Timer_CH_NO)
		{
			case TIMER_CH0:
				*Data = TCNT0;
				break;
			case TIMER_CH1:
				*Data = TCNT1;
				break;
			case TIMER_CH2:
				*Data = TCNT2;
				break;
			default:
				return E_NOK;
		}
	}
	return E_OK;
}



ISR(TIMER0_OVF_vect)
{
	if (NULL != TIMER_OVF_CBK_PTR[TIMER_CH0])
	{
		TIMER_OVF_CBK_PTR[TIMER_CH0]();
	} 
	else
	{
		
	}
}

ISR(TIMER1_OVF_vect)
{
	if (NULL != TIMER_OVF_CBK_PTR[TIMER_CH1])
	{
		TIMER_OVF_CBK_PTR[TIMER_CH1]();
	}
	else
	{
		
	}
}

ISR(TIMER2_OVF_vect)
{
	if (NULL != TIMER_OVF_CBK_PTR[TIMER_CH2])
	{
		TIMER_OVF_CBK_PTR[TIMER_CH2]();
	}
	else
	{
		
	}
}
