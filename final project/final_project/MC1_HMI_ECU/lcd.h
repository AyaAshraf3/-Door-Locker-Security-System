/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: aya ashraf
 *
 *******************************************************************************/


#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "stdlib.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define LCD_BITS_MODE 8
#if ((LCD_BITS_MODE!=8) && (LCD_BITS_MODE!=4))
#error "Number of Data bits should be equal to 4 or 8"
#endif

#define LCD_NUM_LINES 2
#if ((LCD_NUM_LINES!=2) && (LCD_NUM_LINES!=4))
#error "Number of lines bits should be equal to 2 or 4"
#endif

#define LCD_RS_PORT PORTA_ID
#define LCD_RS_PIN  PIN1_ID
#define LCD_ENABLE_PORT PORTA_ID
#define LCD_ENABLE_PIN  PIN2_ID
#define LCD_DB_PORT PORTC_ID

#if (LCD_BITS_MODE==4)
#define LCD_DB4_PIN  PIN3_ID
#define LCD_DB5_PIN  PIN4_ID
#define LCD_DB6_PIN  PIN5_ID
#define LCD_DB7_PIN  PIN6_ID
#endif

/* LCD Commands */
#define LCD_CLEAR_COMMAND                    0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command);

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 character);

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *str);
/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *str);

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col);

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data);

#endif /* LCD_H_ */
