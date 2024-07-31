/******************************************************************************
 *
 * Module: main
 *
 * File Name: Final_project.c
 *
 * Description: microcontroller2 (conrtol_ECU)
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "buzzer.h"
#include <avr/io.h>
#include "timer.h"
#include "uart.h"
#include "external_eeprom.h"
#include <util/delay.h> /* For the delay functions */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

#define PASS_MATCHED 1
#define PASS_UNMATCHED 0

/* global variable contain the ticks count of the timer */
static volatile uint8 g_timer1_ticks=0;

/*******************************************************************************
 *                     Functions Prototypes                                   *
 *******************************************************************************/

/* Description:
 * check whether the two entered passwords are matched or not  */
uint8 checkPassFirst(void);


/* Description:
 * take the password for the first time or when the user wants to change it from HMI_ECU
 * Save it in the EEPROM
 * Check if the two passwords entered are matched or not
 */
void savingInEEPROM(void);


/* Description:
 * enter the password
 * check whether the entered password matches the saved pass in the eeprom */
void checkPass(void);


/* Description:
 * counts the ticks of the timer
 * It is the callback function
 */
void TIMER1TicksCounting(void);

int main(void)
{

	uint8 check_byte;

	/* Initialize the UART driver with :
	 * 8-bit data
	 * parity check is disabled
	 * stop bit is one bit
	 * baud rate is 9600
	 */
	UART_ConfigType UARTconfig2;
	UARTconfig2.baud_rate=std_baudrate;
	UARTconfig2.bit_data=bit_data_8;
	UARTconfig2.mode_selected=Asynch;
	UARTconfig2.parity=disabled;
	UARTconfig2.stop_bit=one_bit;

	 /* Initialize UART with the required configurations */
	UART_init(&UARTconfig2);


	/* Initialize the TIMER1 Driver with:
		 * initial value is 0
		 * compare value is 23437
		 * prescaler is 1024
		 * the mode is compare mode
		 */
	Timer1_ConfigType timer1config2;
	timer1config2.compare_value=23437;
	timer1config2.initial_value=0;
	timer1config2.mode=CTC1;
	timer1config2.prescaler=clk_1024;

	/* Initialize TIMER1 with the required configurations */
	Timer1_init(&timer1config2);

	/* set the TIMER1TicksCounting to be the callback function */
	Timer1_setCallBack(TIMER1TicksCounting);

	/*Enable global interrupts by setting the I-bit*/
	SREG |= (1 << 7);

	/* initialize DC Motor driver */
	DcMotor_Init();

	/* Initialize the BUZZER */
	Buzzer_init();

	while(1)
	{
		/* Receive a command from HMI_ECU to take a specific action */
		check_byte = UART_recieveByte();

		switch(check_byte)
		{
		case'#': /* the user need to change the password or enter the password for the first time */
			savingInEEPROM();
			break;

		case'$': /* The user need to enter the password to either open the door or change password */
			checkPass();
			break;

		case'%':  /* The user need to open the door */
		{
			/* reset the number of ticks*/
			g_timer1_ticks=0;
		DcMotor_Rotate(DcMotor_CW,DUTY_CYCLE_MAX_VALUE);
		/* OPEN the door for 15 seconds */
		while(g_timer1_ticks<=5){}

		/* reset the number of ticks*/
		g_timer1_ticks=0;
		DcMotor_Rotate(DcMotor_stop,0);
		/* HOLD the door for 3 seconds */
		while(g_timer1_ticks<=1){}

		/* reset the number of ticks*/
		g_timer1_ticks=0;
		DcMotor_Rotate(DcMotor_CCW,DUTY_CYCLE_MAX_VALUE);
		/* CLOSE the door for 15 seconds */
		while(g_timer1_ticks<=5){}
		/* STOP the motor*/
		DcMotor_Rotate(DcMotor_stop,0);
		/* reset the number of ticks*/
		g_timer1_ticks=0;
		break;
		}

		case'&': /* The user entered the password wrong 3 consecutive time so that the buzzer must be ON */
		{
			/* reset the number of ticks*/
			g_timer1_ticks=0;
			Buzzer_on();
			/* Activate the BUZZER for 1 minute */
			while(g_timer1_ticks<=20){}
			/*CLOSE the buzzer*/
			Buzzer_off();
			/* reset the number of ticks*/
			g_timer1_ticks=0;
			break;
		}

		}
		_delay_ms(500);

	}

}


/*******************************************************************************
 *                     Functions Definitions                                   *
 *******************************************************************************/


uint8 checkPassFirst(void)
{
	uint8 saved_data=0,resaved_data=0,first_address=0,sec_address=0;

	/* loop for the first password saved in the eeprom*/
	for(first_address=0;first_address<5;first_address++)
	{
		/* the second saved password's address in the eeprom*/
		sec_address = first_address+6;
		 /* get the numbers of the first saved pass from eeprom*/
		EEPROM_readByte((0x01)+first_address,&saved_data);
		_delay_ms(10);
		/* get the numbers of the second saved pass from eeprom*/
		EEPROM_readByte((0x01)+sec_address,&resaved_data);
		if(saved_data != resaved_data)
		{
			return PASS_UNMATCHED; /* if the passwords are not matched return 0 to MC1*/
		}
	}
	return PASS_MATCHED;  /* if the passwords are matched return 1 to MC1*/

}


void checkPass(void)
{
	uint8 key,entered_address;
	for(entered_address=6;entered_address<11;entered_address++)
		{
			/* Receive the pressed key from MC1 through UART */
			key = UART_recieveByte();
			/* enter the password in the second place in the eeprom*/
			if (! (EEPROM_writeByte((0x01)+entered_address,key)) )
			{
				/* if it's not saved send an error to MC1*/
				UART_sendByte(ERROR);
				return;
			}
			/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
			_delay_ms(500);
		}
	if(checkPassFirst())
	{
		UART_sendByte(PASS_MATCHED); /* if the passwords are matched return 1 to MC1*/
	}
	else
	{
		UART_sendByte(PASS_UNMATCHED);/* if the passwords are not matched return 0 to MC1*/
	}

}


void savingInEEPROM(void)
{
	uint8 key,first_address,sec_address;
	for(first_address=0;first_address<5;first_address++)
	{
		/* Receive the pressed key from MC1 through UART */
		key = UART_recieveByte();

		/* Save the first password in the EEPROM */
		if (! (EEPROM_writeByte((0x01)+first_address,key)) )
		{
			/* if it's not saved send an error to MC1*/
			UART_sendByte(ERROR);
			return;
		}
		/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
		_delay_ms(500);
	}

	for(sec_address=6;sec_address<11;sec_address++)
	{
		/* Receive the pressed key from MC1 through UART */
		key = UART_recieveByte();

		/* Save the second password in the EEPROM */
		if (! (EEPROM_writeByte((0x01)+sec_address,key)) )
		{
			/* if it's not saved send an error to MC1*/
			UART_sendByte(ERROR);
			return;
		}
		/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
		_delay_ms(500);
	}
	/* call the checkPassFirst to check if the two passwords are matched or not */
	if (checkPassFirst() == 0)
	{
		/* Send 1 to HMI_ECU to let it know that the two passwords are matched */
		UART_sendByte(PASS_UNMATCHED);
	}
	else
	{
		/* Send 0 to HMI_ECU to let it know that the two passwords are not matched */
		UART_sendByte(PASS_MATCHED);
	}


}

void TIMER1TicksCounting(void)
{
	g_timer1_ticks++; /* increment the number of ticks */
}


