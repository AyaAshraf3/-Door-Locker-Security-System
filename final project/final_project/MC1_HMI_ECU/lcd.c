 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: aya ashraf
 *
 *******************************************************************************/

#include"lcd.h"
#include"common_macros.h"  /* For GET_BIT Macro */
#include"gpio.h"
#include<util/delay.h>   /* For the delay functions */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void)
{
	/* Configure the direction for RS and E pins as output pins */
	GPIO_setupPinDirection(LCD_RS_PORT,LCD_RS_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_ENABLE_PORT,LCD_ENABLE_PIN,PIN_OUTPUT);
	_delay_ms(20); /* LCD Power ON delay always > 15ms */
#if(LCD_BITS_MODE==4)
	/* Configure 4 pins in the data port as output pins */
	GPIO_setupPinDirection(LCD_DB_PORT,LCD_DB4_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DB_PORT,LCD_DB5_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DB_PORT,LCD_DB6_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DB_PORT,LCD_DB7_PIN,PIN_OUTPUT);

	/* Send for 4 bit initialization of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);
#elif(LCD_BITS_MODE==8)
	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DB_PORT,PORT_OUTPUT);
	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);
#endif

	LCD_sendCommand( LCD_CURSOR_OFF);  //cursor off
	LCD_sendCommand(LCD_CLEAR_COMMAND);  //clear screen

}


/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command)
{
	/* Instruction Mode RS=0 */
	GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,LOGIC_LOW);
	_delay_ms(1); /* delay for processing Tas = 50ns */
	/* Enable LCD E=1 */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_HIGH);
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
#if(LCD_BITS_MODE==4)
	GPIO_writePin(LCD_DB_PORT,LCD_DB4_PIN,GET_BIT(command,4));
	GPIO_writePin(LCD_DB_PORT,LCD_DB5_PIN,GET_BIT(command,5));
	GPIO_writePin(LCD_DB_PORT,LCD_DB6_PIN,GET_BIT(command,6));
	GPIO_writePin(LCD_DB_PORT,LCD_DB7_PIN,GET_BIT(command,7));
	_delay_us(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	GPIO_writePin(LCD_DB_PORT,LCD_DB4_PIN,GET_BIT(command,0));
	GPIO_writePin(LCD_DB_PORT,LCD_DB5_PIN,GET_BIT(command,1));
	GPIO_writePin(LCD_DB_PORT,LCD_DB6_PIN,GET_BIT(command,2));
	GPIO_writePin(LCD_DB_PORT,LCD_DB7_PIN,GET_BIT(command,3));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif(LCD_BITS_MODE==8)
	/* out the required command to the data bus B0 --> B7 */
	GPIO_writePort(LCD_DB_PORT,command);
	_delay_ms(1); /* delay for processing Tdsw = 100ns */

	/* Disable LCD E=0 */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_LOW);
	_delay_ms(1); /* delay for processing Th = 13ns */

#endif

}


/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 character)
{
	/* Data Mode RS=1 */
	GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,LOGIC_HIGH);
	_delay_ms(1); /* delay for processing Tas = 50ns */

    /* Enable LCD E=1 */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_HIGH);
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

#if(LCD_BITS_MODE==4)
	GPIO_writePin(LCD_DB_PORT,LCD_DB4_PIN,GET_BIT(character,4));
	GPIO_writePin(LCD_DB_PORT,LCD_DB5_PIN,GET_BIT(character,5));
	GPIO_writePin(LCD_DB_PORT,LCD_DB6_PIN,GET_BIT(character,6));
	GPIO_writePin(LCD_DB_PORT,LCD_DB7_PIN,GET_BIT(character,7));
	_delay_us(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	GPIO_writePin(LCD_DB_PORT,LCD_DB4_PIN,GET_BIT(character,0));
	GPIO_writePin(LCD_DB_PORT,LCD_DB5_PIN,GET_BIT(character,1));
	GPIO_writePin(LCD_DB_PORT,LCD_DB6_PIN,GET_BIT(character,2));
	GPIO_writePin(LCD_DB_PORT,LCD_DB7_PIN,GET_BIT(character,3));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif(LCD_BITS_MODE==8)
	/* out the required command to the data bus B0 --> B7 */
	GPIO_writePort(LCD_DB_PORT,character);
	_delay_ms(1);/* delay for processing Tdsw = 100ns */

	/* Disable LCD E=0 */
	GPIO_writePin(LCD_ENABLE_PORT,LCD_ENABLE_PIN,LOGIC_LOW);
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif

}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *str)
{
	while(*str != '\0')
	{
		LCD_displayCharacter(*str);
		str++;

	}

}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_location_address=0;
  #if(LCD_NUM_LINES==4)
	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
	case 0:
		lcd_location_address = col;
		break;
	case 1:
		lcd_location_address = col + 0x40;
		break;
	case 2:
		lcd_location_address = col + 0x10;
		break;
	case 3:
		lcd_location_address = col + 0x50;
		break;
	}
#elif(LCD_NUM_LINES==2)
	switch(row)
	{
	case 0:
		lcd_location_address = col;
		break;
	case 1:
		lcd_location_address = col + 0x40;
		break;
	}
#endif

	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_location_address | LCD_SET_CURSOR_LOCATION);

}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *str)
{
	/* go to to the required LCD position */
	LCD_moveCursor(row,col);
	/* display the string */
	LCD_displayString(str);

}


/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
	/* String to hold the ascii result */
	char lcd_str[16];
	/* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	itoa(data,lcd_str,10);
    /* Display the string */
	LCD_displayString(lcd_str);

}


/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	/* Send clear display command */
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}

