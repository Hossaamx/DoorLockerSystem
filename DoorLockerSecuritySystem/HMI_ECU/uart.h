/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum{
	DISABLED,RESERVED,EVEN,ODD
}uart_Parity;
typedef enum{
	ONE,TWO
}uart_stopBit;
typedef enum{
	FIVE,SIX,SEVEN,EIGHT,NINE=7
}uart_charSize;
typedef struct{
	uint32 baud_rate;
	uart_Parity parity;
	uart_stopBit stopBit;
	uart_charSize size;
}uart_ConigType;

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

void UART_init(const uart_ConigType* ConfigPtr);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint16 UART_recieveByte(void);
void UART_sendByte(const uint16 data);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str);
#endif
