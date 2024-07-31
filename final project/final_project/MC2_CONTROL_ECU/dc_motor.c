/******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the ATmega16 DC motor driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/


#include"dc_motor.h"
#include"pwm.h"
#include"gpio.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void DcMotor_Init(void)
{
	/* Configure 2 pins in the data port as output pins */
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2,PIN_OUTPUT);

	/* Writing zero in the DC motor two pins*/
	GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1,DcMotor_stop);
	GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2,DcMotor_stop);

}



void DcMotor_Rotate(enum DcMotor_State state,uint8 speed)
{
	/* get the duty cycle used for PWM signal  */
	uint8 duty_cycle= (speed * 255) / 100;

	/* choosing the direction of rotation of the motor based on the DC motor  */
	switch(state)
	{
	case DcMotor_stop:
		/* Writing zero in the DC motor two pins*/
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1,DcMotor_stop);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2,DcMotor_stop);
		/*call the start function of timer0 to create the PWM signal */
		PWM_Timer0_Start(duty_cycle);
		break;
	case DcMotor_CW:
		/* Writing 1 in the DC motor pin2 so it rotates clockwise*/
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2,DcMotor_CW);
		/*call the start function of timer0 to create the PWM signal */
		PWM_Timer0_Start(duty_cycle);
		break;
	case DcMotor_CCW:
		/* Writing 1 in the DC motor pin1 so it rotates anti-clockwise*/
		PWM_Timer0_Start(duty_cycle);
		/*call the start function of timer0 to create the PWM signal */
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1,DcMotor_CW);
		break;
	}


}
