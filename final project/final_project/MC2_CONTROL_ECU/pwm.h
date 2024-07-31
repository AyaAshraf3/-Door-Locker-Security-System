/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: header file for the ATmega16 PWM driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/


#ifndef PWM_H_
#define PWM_H_
#include"gpio.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PWM_OCO_PORT PORTB_ID
#define PWM_OCO_PIN PIN3_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for starting the timer0 to create the PWM signal */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
