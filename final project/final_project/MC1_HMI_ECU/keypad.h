/******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Header file for the Keypad driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"

#define KEYPAD_ROWS_NUM 4
#define KEYPAD_COLS_NUM 4

#define KEYPAD_ROW_PORT_ID PORTB_ID
#define KEYPAD_ROW_FIRST_PIN_ID PIN4_ID

#define KEYPAD_COL_PORT_ID PORTD_ID
#define KEYPAD_COL_FIRST_PIN_ID PIN2_ID

#define KEYPAD_BUTTON_PRESSED LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED LOGIC_HIGH

uint8 KEYPAD_getPressedKey(void);



#endif /* KEYPAD_H_ */
