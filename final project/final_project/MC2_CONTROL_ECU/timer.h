/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: header file for the Timer AVR driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

enum Timer1_Prescaler{
	no_clk,clk_1,clk_8,clk_64,clk_256,clk_1024,external_clk_falling,external_clk_rising
};

enum Timer1_Mode{
	normal_mode, PWM_PCorrect_8 ,PWM_PCorrect_9,PWM_PCorrect_10,CTC1,fast_PWM_8,fast_PWM_9,fast_PWM_10,PWM_Phase_freq_Correct1,\
	PWM_Phase_freq_Correct2,PWM_PCorrect1,PWM_PCorrect2,CTC2,fast_PWM1=14,fast_PWM2
};

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 enum Timer1_Prescaler prescaler;
 enum Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description:
 * Function to initialize the Timer driver
 **/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/* Description:
 * Function to disable the Timer1
 **/
void Timer1_deInit(void);

/* Description:
 * Function to set the Call Back function address
 **/
void Timer1_setCallBack(void(*a_ptr)(void));



#endif /* TIMER_H_ */


