  /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*flag used to help with send and receive functions as we must write and read from 9th bit first in case of 9bit data in frame*/
uint8 g_extraBitFlag=0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const uart_ConigType* ConfigPtr){
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1 << U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1 << RXEN) | (1 << TXEN);
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = Configured by passed ConfigPtr
	 * USBS    = Configured by passed ConfigPtr
	 * UCSZ1:0 = Configured by passed ConfigPtr
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC= (1<<URSEL)|((ConfigPtr->parity)<<UPM1)|((ConfigPtr->stopBit)<<USBS)|(((ConfigPtr->size)&0x03)<<UCSZ1);
	if(ConfigPtr->size==NINE){
		SET_BIT(UCSRB,UCSZ2);
		g_extraBitFlag=1;
	}
	else{
		g_extraBitFlag=0;
	}
	/* Calculate the UBRR register value */
	ubrr_value = (uint16) (((F_CPU / ((ConfigPtr->baud_rate) * 8UL))) - 1);
	UBRRH= ubrr_value <<8;
	UBRRL=ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
/*changed input from uint8 to uint16 to support 9 bits mode*/
void UART_sendByte(const uint16 data){

	 /* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	if(g_extraBitFlag){
		UCSRB|=(data>>TXB8);/*Put the 9th bit in TXB8 before UDR*/
	}
	UDR=data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint16 UART_recieveByte(void){
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	uint16 data = 0;
		while (BIT_IS_CLEAR(UCSRA, RXC)) {
		}
		if (g_extraBitFlag) {
			data = (UDR | (UCSRB & (1 << RXB8)));
		}
		data = UDR;
		return data;
}
/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str){
	uint8 i = 0;

	/* Send the whole string */
	while (Str[i] != '\0') {
		UART_sendByte(Str[i]);
		i++;
	}
}
/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str){
	uint8 i=0;
	Str[i]= UART_recieveByte();
	while(Str[i]!='#'){
		i++;
		Str[i]= UART_recieveByte();
	}
	Str[i]='\0';
	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
}
