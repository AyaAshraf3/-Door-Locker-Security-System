/******************************************************************************
 *
 * Module: DC MOTOT
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the ATmega16 DC Motor driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_PORT_ID PORTB_ID
#define DC_MOTOR_PIN1   PIN0_ID
#define DC_MOTOR_PIN2   PIN1_ID
#define DUTY_CYCLE_MAX_VALUE 100

/*******************************************************************************
 *                      Structures/Enums definition                                  *
 *******************************************************************************/

enum DcMotor_State{
	DcMotor_stop,DcMotor_CW, DcMotor_CCW ,
};


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the DC Motor driver.
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for setting the rotating direction and the speed of the motor.
 */
void DcMotor_Rotate(enum DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
