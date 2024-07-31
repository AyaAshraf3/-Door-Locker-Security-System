/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the timer AVR driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/

#include "timer.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/* global variable contain the ticks count of the timer */
unsigned char g_tick = 0;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/* Interrupt Service Routine for timer1 compare mode */
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/* Interrupt Service Routine for timer1 overflow mode */
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}

}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/



void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCNT1 = Config_Ptr->initial_value;		/* Set timer1 initial count to zero */

	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);

	if(Config_Ptr->mode == CTC1)
	{

		OCR1A = Config_Ptr->compare_value;    /* Set the Compare value */
		TCCR1B = (TCCR1B & 0xE7) | ((Config_Ptr->mode)<<1); //check this shift is 1 or 3
		TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */
	}
	else if (Config_Ptr->mode == normal_mode)
	{
		TCCR1A = (TCCR1A & 0xFC) | (Config_Ptr->mode);
		TIMSK |= (1<<TOIE1); /* Enable Timer1 Overflow Interrupt */
	}


}


void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

void Timer1_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0 ;
	OCR1B = 0 ;
	/* Disable the Input Capture interrupt */
	TIMSK &= ~(1 << TOIE1);
	TIMSK &= ~(1 << OCIE1A);

	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;

}


