 /******************************************************************************
 *
 * Module: I2C
 *
 * File Name: i2c.c
 *
 * Description: Header file for the AVR GPIO driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/

#include "i2c.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_TypeConfig* ConfigPtr){
	TWCR = (1<<TWEN); /* enable TWI */
	TWSR = 0x00;/*The only value for TWBS to work generic for 2modes is TWBS=00*/
	/*Only two modes for I2C supported by AVR one for normal mode and one for Fast mode*/
	switch (ConfigPtr->bitRate) {
	default:

	case (Normal):
		/*calculated for SCL eqn*/
		TWBR = 32;
		break;

	case (Fast):
		/*Calculated from SCL equation*/
		TWBR = 2;
		break;
	}
	/* Two Wire Bus address my address if any master device want to call me:(used in case this MC is a slave device)
	 General Call Recognition: Off */
	TWAR = ((ConfigPtr->masterAddress) << 1);
}
void TWI_start(void){
	/* Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	/*wait until start bit is sent successfully */
	while(BIT_IS_CLEAR(TWCR,TWINT)){}
}
void TWI_stop(void){
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
void TWI_writeByte(uint8 data){
	/* Put data On TWI data Register */
	TWDR = data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*wait for the data to be sent successfully*/
	while(BIT_IS_CLEAR(TWCR,TWINT)){}

}

uint8 TWI_readByteWithACK(void){
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT)) {
	}
	return TWDR;

}
uint8 TWI_readByteWithNACK(void){
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT)) {
	}
	return TWDR;
}
uint8 TWI_getStatus(void){
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	return (TWSR & 0xF8);
}
