/*
 * BCM.c
 *
 * Created: 1/13/2020 1:11:06 PM
 *  Author: ahmad
 */ 

#include "interrupt.h"
#include "BCM.h"
#include "CPUSleep.h"
#include "TMU.h"
#include "DIO.h"

uint8 alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
/*sizeof(alphabet)/sizeof(uint8)*/
/*uint8 alphabet[27];*/
void LED0(void)
{
	DIO_Toggle(GPIOB, PIN0);
}
void LED1(void)
{
	DIO_Toggle(GPIOB, PIN1);
}
void LED2(void)
{
	DIO_Toggle(GPIOB, PIN2);
}
void LED3(void)
{
	DIO_Toggle(GPIOB, PIN3);
}
void LED4(void)
{
	DIO_Toggle(GPIOB, PIN4);
}
void LED5(void)
{
	DIO_Toggle(GPIOB, PIN5);
}
void LED6(void)
{
	DIO_Toggle(GPIOB, PIN6);
}
void LED7(void)
{
	DIO_Toggle(GPIOB, PIN7);
}

int main(void)
{
	
	BCM_Init();
	BCM_Send(BCM_UART, sizeof(alphabet)/sizeof(uint8), alphabet);
	BCM_Send(BCM_UART, 27, alphabet);
	DIO_Cfg_s DIO_Info = {GPIOB, FULL_PORT, OUTPUT};
	DIO_init(&DIO_Info);
	DIO_Write(GPIOB, FULL_PORT, LOW);
	TMU_Init();
	TMU_Start(LED0, 5, PERIODIC);
	TMU_Start(LED1, 10, PERIODIC);
	CPUSleep_Init();
	CPUSleep_setMode(CPUSLEEP_IDLE_MODE);
	sei();

    while(1)
    {
		
        //TODO:: Please write your application code 
		DIO_Toggle(GPIOB, PIN3);
		BCM_TxDispatcher();
		DIO_Toggle(GPIOB, PIN3);
		DIO_Toggle(GPIOB, PIN4);
		TMU_Dispatcher();
		DIO_Toggle(GPIOB, PIN4);
		DIO_Toggle(GPIOB, PIN5);
		CPUSleep_enable();
		DIO_Toggle(GPIOB, PIN5);
    }
}

