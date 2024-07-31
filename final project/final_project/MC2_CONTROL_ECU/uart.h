 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Aya Ashraf
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


/*******************************************************************************
 *                             Definitions                                    *
 *******************************************************************************/

enum UART_BitData{
    bit_data_5,bit_data_6,bit_data_7,bit_data_8,bit_data_9=7
};
enum UART_Parity{
	disabled,even=2,odd
};
enum UART_StopBit{
	one_bit,two_bits
};
enum UART_BaudRate{
	std_baudrate=9600,std_baudrate_2=14400
};

enum UART_Mode{
	Asynch,Synch
};


typedef struct{
	enum UART_BitData bit_data;
	enum UART_Parity parity;
	enum UART_StopBit stop_bit;
	enum UART_BaudRate baud_rate;
	enum UART_Mode mode_selected;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */





