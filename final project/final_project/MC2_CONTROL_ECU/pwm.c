/*
 * pwm.c
 *
 *  Created on: Oct 14, 2023
 *      Author: DELL
 */

#include"pwm.h"
#include <avr/io.h>

void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0; //Set Timer Initial value

	OCR0  = duty_cycle; // Set Compare Value

	//set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
	GPIO_setupPinDirection(PWM_OCO_PORT,PWM_OCO_PIN,PIN_OUTPUT);

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
