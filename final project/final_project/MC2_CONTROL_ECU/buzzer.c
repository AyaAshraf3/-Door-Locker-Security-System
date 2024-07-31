 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Aya AShraf
 *
 *******************************************************************************/
#include "gpio.h"
#include"buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Buzzer_init()
{

	/* Setup the direction for the buzzer pin as output pin through the GPIO driver */

	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

	/* Turn off the buzzer through the GPIO */

	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);

}


void Buzzer_on()
{
	/* Turn on the buzzer through the GPIO */
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

void Buzzer_off()
{
	/* Turn off the buzzer through the GPIO */
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
