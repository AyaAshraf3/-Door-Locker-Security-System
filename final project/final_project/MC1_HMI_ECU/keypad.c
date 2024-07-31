 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/

#include"keypad.h"
#include"gpio.h"
#include<util/delay.h>

#ifndef STANDARD_KEYPAD
#if(KEYPAD_COLS_NUM==3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_num);
#elif(KEYPAD_COLS_NUM==4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_num);
#endif
#endif



uint8 KEYPAD_getPressedKey(void)
{
	uint8 row,col;
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+2,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+3,PIN_INPUT);

	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+1,PIN_INPUT);
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+2,PIN_INPUT);

#if(KEYPAD_COLS_NUM==4)
	GPIO_setupPinDirection(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+3,PIN_INPUT);
#endif

	while(1)
	{
		for(row=0;row<KEYPAD_ROWS_NUM;row++)
		{
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+row,PIN_OUTPUT);
			GPIO_writePin(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+row,KEYPAD_BUTTON_PRESSED);

			for(col=0;col<KEYPAD_COLS_NUM;col++)
			{
				if(GPIO_readPin(KEYPAD_COL_PORT_ID,KEYPAD_COL_FIRST_PIN_ID+col) == KEYPAD_BUTTON_PRESSED)
				{
                   #if(KEYPAD_COLS_NUM==3)

                     #ifdef STANDARD_KEYPAD
					   return ((row*KEYPAD_COLS_NUM)+col+1);
                     #else
					   return KEYPAD_4x3_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
                     #endif

                   #elif(KEYPAD_COLS_NUM==4)

                     #ifdef STANDARD_KEYPAD
					   return ((row*KEYPAD_COLS_NUM)+col+1);
                     #else
					   return KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_COLS_NUM)+col+1);
                     #endif

                   #endif

				}
			}

			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+row,PIN_INPUT);
			GPIO_writePin(KEYPAD_ROW_PORT_ID,KEYPAD_ROW_FIRST_PIN_ID+row,KEYPAD_BUTTON_RELEASED);
			_delay_ms(5);
		}

	}

}

#ifndef STANDARD_KEYPAD
#if(KEYPAD_COLS_NUM==4)
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_num)
{
	uint8 keypad_button = 0;
	switch(button_num)
	{
	        case 1: keypad_button = 7;
					break;
			case 2: keypad_button = 8;
					break;
			case 3: keypad_button = 9;
					break;
			case 4: keypad_button = '%'; // ASCII Code of %
					break;
			case 5: keypad_button = 4;
					break;
			case 6: keypad_button = 5;
					break;
			case 7: keypad_button = 6;
					break;
			case 8: keypad_button = '*'; /* ASCII Code of '*' */
					break;
			case 9: keypad_button = 1;
					break;
			case 10: keypad_button = 2;
					break;
			case 11: keypad_button = 3;
					break;
			case 12: keypad_button = '-'; /* ASCII Code of '-' */
					break;
			case 13: keypad_button = 13;  /* ASCII of Enter */
					break;
			case 14: keypad_button = 0;
					break;
			case 15: keypad_button = '='; /* ASCII Code of '=' */
					break;
			case 16: keypad_button = '+'; /* ASCII Code of '+' */
					break;
			default: keypad_button = button_num;
					break;
	}
	return keypad_button;
}
#elif(KEYPAD_COLS_NUM==3)
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_num)
{
	uint8 keypad_button = 0;
		switch(button_number)
		{
			case 10: keypad_button = '*'; // ASCII Code of *
					 break;
			case 11: keypad_button = 0;
					 break;
			case 12: keypad_button = '#'; // ASCII Code of #
					 break;
			default: keypad_button = button_number;
					break;
		}
		return keypad_button;
}
#endif
#endif
