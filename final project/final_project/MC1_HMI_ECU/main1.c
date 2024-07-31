/******************************************************************************
 *
 * Module: MAIN
 *
 * File Name: main.c
 *
 * Description: main file for microcontroller1
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include <avr/io.h>
#include <util/delay.h> /* For the delay functions */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

#define PASS_SUCCESS 1
#define PASS_FAILED 0

/* global variable contain the ticks count of the timer */
static volatile uint8 g_timer1_ticks=0;

/* global variable contain the received byte by UART from CONTROL_ECU */
uint8 receive_byte = 0;


/*******************************************************************************
 *                     Functions Prototypes                                   *
 *******************************************************************************/

/* Description:
 * it is used when the password does not match the user's password for three consecutive trials
 */
void errorInPass(void);

/* Description:
 * take the password from the user
 * send it to CONTROL_ECU to check on it
 */
void EnterPass(void);

/* Description:
 * Responsible for changing the password first time and for any time the user wants to change it
 */
void ChangePass(void);

/* Description:
 * Responsible for choosing the option from the two options
 */
uint8 choose_option(void);

/* Description:
 * counts the ticks of the timer
 * It is the callback function
 */
void TIMER1TicksCounting(void);

int main(void)
{
	/* a variable to get the choose option from the keypad */
	uint8 mode_num;

	/*Enable global interrupts by setting the I-bit*/
		SREG |= (1 << 7);

	/* Initialize the UART driver with :
		 * 8-bit data
		 * parity check is disabled
		 * stop bit is one bit
		 * baud rate is 9600
		 */
		UART_ConfigType UARTconfig1;
		UARTconfig1.baud_rate=std_baudrate;
		UARTconfig1.bit_data=bit_data_8;
		UARTconfig1.mode_selected=Asynch;
		UARTconfig1.parity=disabled;
		UARTconfig1.stop_bit=one_bit;

		/* Initialize UART with the required configurations */
		UART_init(&UARTconfig1);


		/* Initialize the TIMER1 Driver with:
		 * initial value is 0
		 * compare value is 23437 as F_CPU=8MHZ and to get 3sec
		 * prescaler is 1024
		 * the mode is compare mode
		 */
		Timer1_ConfigType timer1config1;
		timer1config1.compare_value=23437;
		timer1config1.initial_value=0;
		timer1config1.mode=CTC1;
		timer1config1.prescaler=clk_1024;

		/* Initialize TIMER1 with the required configurations */
		Timer1_init(&timer1config1);

		 /* set the TIMER1TicksCounting to be the callback function */
		Timer1_setCallBack(TIMER1TicksCounting);

		 /* Initialize the LCD */
		LCD_init();

		LCD_clearScreen();
		LCD_displayString("Plz enter pass:");
		LCD_moveCursor(1,0);

		/* calling the function ChangePass to give the password to the system for the first time and save it */
		ChangePass();

		/* if the two passwords are matched receive 1 */
		if(receive_byte == PASS_SUCCESS)
		{
			/* Go to step 2 to choose from the options */
		}
		else{
			/* if the two passwords don't match enter the password again  */
			LCD_clearScreen();
			LCD_displayString("error enter pass:");
			LCD_moveCursor(1,0);
			ChangePass();
			if(receive_byte == PASS_SUCCESS)
			{

			}
			else
			{
				LCD_clearScreen();
				LCD_displayString("error enter pass:");
				LCD_moveCursor(1,0);
				ChangePass();
				if(receive_byte == PASS_SUCCESS)
				{

				}
				else
				{
					/* if the two passwords don't match for 3 consecutive times display an ERROR and close the system */
					LCD_clearScreen();
					LCD_displayString("SYSTEM ERROR!!");
					return 0;
				}
			}
		}



	while(1)
	{

		/* Call the function to display the main options on the LCD and choose from it */
		mode_num = choose_option();

		LCD_clearScreen();
		LCD_displayString("Plz enter pass:");
		LCD_moveCursor(1,0);

		/* Call the function to enter the pass*/
		EnterPass();

		/* Receive 1 for matched passwords */
		if(receive_byte == PASS_SUCCESS)
		{
			 /* go to the switch cases to execute the chosen option */
		}
		else{
			LCD_clearScreen();
			LCD_displayString("error enter pass:");
			LCD_moveCursor(1,0);

			/* Call the function to enter the pass again*/
			EnterPass();

			/* Receive 1 for matched passwords */
			if(receive_byte == PASS_SUCCESS)
			{

			}
			else{
				LCD_clearScreen();
				LCD_displayString("error enter pass:");
				LCD_moveCursor(1,0);

				/* Call the function to enter the pass again*/
				EnterPass();

				/* Receive 1 for matched passwords */
				if(receive_byte == PASS_SUCCESS)
				{

				}
				else
				{
					/* if the two passwords don't match for 3 consecutive times call the error function */
					errorInPass();
					continue;

				}
			}
		}

		switch(mode_num)
		{
		case'+': /* opening the door option */
		{
			LCD_clearScreen();
			LCD_displayString("Door's unlocking");

			/* a command to make the CONTROL_ECU turns the motor on and its timer1 */
			UART_sendByte('%');

			/*resetting the number of ticks*/
			g_timer1_ticks=0;

			/* waiting for the timer1 to pass 15sec*/
			while(g_timer1_ticks<=5){}

			/*resetting the number of ticks*/
			g_timer1_ticks=0;
			LCD_clearScreen();

			/* waiting for the timer1 to pass 3sec*/
			while(g_timer1_ticks<=1){}

			/*resetting the number of ticks*/
			g_timer1_ticks=0;
			LCD_displayString("Door's locking");

			/* waiting for the timer1 to pass 15sec*/
			while(g_timer1_ticks<=5){}
			g_timer1_ticks=0;


			LCD_clearScreen();
			break;
		}

		case'-': /* changing the password option */
			{

				LCD_clearScreen();
				LCD_displayString("Plz enter the pass:");
				LCD_moveCursor(1,0);

				/* calling the function ChangePass to change the password and save it */
				ChangePass();

				 /* Receive 1 for matched passwords */
				if(receive_byte == PASS_SUCCESS)
				{

				}else
				{
					/* If the two passwords don't match then enter the password again */
					LCD_clearScreen();
					LCD_displayString("error enter pass:");
					LCD_moveCursor(1,0);

					/* calling the function ChangePass to change the password and save it */
					ChangePass();

					/* Receive 1 for matched passwords */
				if(receive_byte == PASS_SUCCESS)
				{

				}
				else
				{
					/* If the two passwords don't match then enter the password again */
					LCD_clearScreen();
					LCD_displayString("error enter pass:");
					LCD_moveCursor(1,0);

					/* calling the function ChangePass to change the password and save it */
					ChangePass();

					/* Receive 1 for matched passwords */
					if(receive_byte == PASS_SUCCESS)
					{

					}
					else
					{
						/* if the two passwords don't match for 3 consecutive times call the error function */
						errorInPass();
					}
				}
				}
				break;
			}

		}

	}

}


/*******************************************************************************
 *                     Functions Definitions                                   *
 *******************************************************************************/

uint8 choose_option(void)
{
	uint8 l_mode_num=0; /* variable to get the chosen option */
	do{
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		LCD_displayString("+ : Open door");
		LCD_moveCursor(1,0);
		LCD_displayString("- : change pass");

		/* get the chosen option from the keypad */
		l_mode_num = KEYPAD_getPressedKey();
		_delay_ms(500);

	}while(l_mode_num != 43 && l_mode_num != 45 ); /* make sure it's a '+' or '-' */

	return l_mode_num;
}

void errorInPass(void)
{
	/* a command to make the CONTROL_ECU ready for using the buzzer */
	    UART_sendByte('&');
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"ERROR!BUZZER ON");

		/*resetting the number of ticks*/
		g_timer1_ticks=0;

		/* waiting for the timer1 to pass 60sec*/
		while(g_timer1_ticks<=20){}
		g_timer1_ticks=0;

}



void ChangePass(void)
{
	uint8 key_num=0;
	uint8 i=0;

	/* a command to make the CONTROL_ECU ready for reading the two entered passwords and save it */
	UART_sendByte('#');

	for(i=0;i<5;i++)
	{
		/* Get the pressed button from keypad */
		key_num = KEYPAD_getPressedKey();

		/* make sure it's a number from 0 to 9 */
		if(key_num <=9 && key_num >=0)
		{
			 /* display the pressed keypad switch */
			LCD_displayString("*");

			/* Send the pressed key to MC2 through UART */
			UART_sendByte(key_num);
		}
		else
		{
			i--; /* to make the UART send five numbers only not including any characters */
		}

		/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
		_delay_ms(500);
	}

	do{
	key_num = KEYPAD_getPressedKey();

	/* make '=' as an Enter button */
	}	while(key_num != '=');

	LCD_displayStringRowColumn(0,0,"plz re-enter the");
	LCD_displayStringRowColumn(1,0,"same pass:");

	key_num=0;

		for(i=0;i<5;i++)
		{
			/* Get the pressed button from keypad */
			key_num = KEYPAD_getPressedKey();

			/* make sure it's a number from 0 to 9 */
			if(key_num <=9 && key_num >=0)
			{
				/* display the pressed keypad switch */
				LCD_displayString("*");
				/* Send the pressed key to MC2 through UART */
				UART_sendByte(key_num);
			}
			else
			{
				i--; /* to make the UART send five numbers only not including any characters */
			}

			/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
			_delay_ms(500);

		}

		do{
			key_num = KEYPAD_getPressedKey();
			}	while(key_num != '='); /* make '=' as an Enter button */

		/* Receive a byte 1 (if the passwords are matched)or 0 (if the passwords are not matched) */
		receive_byte = UART_recieveByte();

		/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
				_delay_ms(500);

}

void EnterPass(void)
{
	uint8 key_num=0;
		uint8 i=0;

		/* a command to make the CONTROL_ECU ready for reading the password and check it with the one saved before */
		UART_sendByte('$');

		for(i=0;i<5;i++)
		{
			/* Get the pressed button from keypad */
			key_num = KEYPAD_getPressedKey();

			/* make sure it's a number from 0 to 9 */
			if(key_num <=9 || key_num >=0)
			{
				/* display the pressed keypad switch */
				LCD_displayString("*");
				/* Send the pressed key to MC2 through UART */
				UART_sendByte(key_num);
			}
			else
			{
				i--; /* to make the UART send five numbers only not including any characters */
			}

			/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
			_delay_ms(500);
		}

		do{
		key_num = KEYPAD_getPressedKey();
		}	while(key_num != '='); /* make '=' as an Enter button */

		/* Receive a byte 1 (if the passwords are matched)or 0 (if the passwords are not matched) */
		receive_byte = UART_recieveByte();

		/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
		_delay_ms(500);

}

void TIMER1TicksCounting(void)
{
	g_timer1_ticks++; /* increment the number of ticks */
}
