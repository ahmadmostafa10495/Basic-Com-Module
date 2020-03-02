/*
 * DIO.c
 *
 * Created: 12/22/2019 7:43:13 PM
 *  Author: ahmad
 */ 


#include "DIO.h"



/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS 
DIO_init (DIO_Cfg_s *DIO_info)
{
	if (!(DIO_info == NULL))
	{
		switch(DIO_info->dir)
		{
			case INPUT:
			switch (DIO_info->GPIO)
			{
				case GPIOA:
				PORTA_DIR &= ~DIO_info->pins;
				break;
				case GPIOB:
				PORTB_DIR &= ~DIO_info->pins;
				break;
				case GPIOC:
				PORTC_DIR &= ~DIO_info->pins;
				break;
				case GPIOD:
				PORTD_DIR &= ~DIO_info->pins;
				break;
				default:
				return E_NOK;
			}
			break;
			case OUTPUT:
			switch (DIO_info->GPIO)
			{
				case GPIOA:
				PORTA_DIR |= DIO_info->pins;
				break;
				case GPIOB:
				PORTB_DIR |= DIO_info->pins;
				break;
				case GPIOC:
				PORTC_DIR |= DIO_info->pins;
				break;
				case GPIOD:
				PORTD_DIR |= DIO_info->pins;
				break;
				default:
				return E_NOK;
			}
			break;
			default:
			return E_NOK;
		}
	}
	else
	{
		return E_NOK;
	}
	return E_OK;
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS 
DIO_Write (uint8 GPIO, uint8 pins, uint8 value)
{
	switch(value)
	{
		case LOW:
		switch (GPIO)
		{
			case GPIOA:
			PORTA_DATA &= ~pins;
			break;
			case GPIOB:
			PORTB_DATA &= ~pins;
			break;
			case GPIOC:
			PORTC_DATA &= ~pins;
			break;
			case GPIOD:
			PORTD_DATA &= ~pins;
			break;
			default:
			return E_NOK;
		}
		break;
		case HIGH:
		switch (GPIO)
		{
			case GPIOA:
			PORTA_DATA |= pins;
			break;
			case GPIOB:
			PORTB_DATA |= pins;
			break;
			case GPIOC:
			PORTC_DATA |= pins;
			break;
			case GPIOD:
			PORTD_DATA |= pins;
			break;
			default:
			return E_NOK;
		}
		break;
		default:
		return E_NOK;
	}
	return E_OK;
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS 
DIO_Read (uint8 GPIO, uint8 pins, uint8 *data)
{
	if (NULL == data)
	{
		return E_NOK;
	}
	else
	{
		switch (GPIO)
		{
			case GPIOA:
			*data = PORTA_PIN & pins;       //GET_BIT(PORTA_PIN,pin);
			break;
			case GPIOB:
			*data = PORTB_PIN & pins;                // return GET_BIT(PORTB_PIN,pin);
			break;
			case GPIOC:
			*data = PORTC_PIN & pins;                               //return GET_BIT(PORTC_PIN,pin);	
			break;
			case GPIOD:
			*data = PORTD_PIN & pins;                // return GET_BIT(PORTD_PIN,pin);
			break;
			default:
			return E_OK;
		}
	}
	return E_OK;
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS 
DIO_Toggle (uint8 GPIO, uint8 pins)
{
	switch (GPIO)
	{
		case GPIOA:
		PORTA_DATA ^= pins;
		break;
		case GPIOB:
		PORTB_DATA ^= pins;
		break;
		case GPIOC:
		PORTC_DATA ^= pins;
		break;
		case GPIOD:
		PORTD_DATA ^= pins;
		break;
		default:
		return E_NOK;
	}
	return E_OK;
}