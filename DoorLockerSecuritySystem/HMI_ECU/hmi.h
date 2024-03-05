
/* Module: HMI_ECU
 *
 * File Name: hmi.h
 *
 * Description: Header file HMI ECU
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/

#ifndef HMI_H_
#define HMI_H_
#include "timer.h"
#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include <avr/io.h>
#include "avr/interrupt.h"


#define PASSWORD_LENGTH 5
#define MAX_CHANCES 3
#define DISPLAY_PERIOD 500
#define FIFTEEN_SECOND 15
#define THREE_SECOND 3
#define ONE_MINUTE 60
#define MC_READY_TO_SEND	0x10
#define MC_READY_TO_RECEIVE	0x11
#define PASSWORD_MATCHED 	0x01
#define PASSWORD_MISMATCHED	0x00
#define CHANGE_PASSWORD_REQUEST	0x02
#define UNLOCK_DOOR_REQUEST	0x03
#define CHANGE_PASSWORD	0x04
#define UNLOCK_DOOR	0x05
#define WRONG_MATCH	0x06 /*For matching with e2prom*/
#define SYSTEM_LOCK	0x07

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*Function responsible for:
 * 1-Function used to prompt user for password on LCD
 * 2-Accepts Input from user on keypad and store it in Global array
 *
 * */
void getPassword();
/*Function Responsible For:
 * 1-Getting Password in The right Format For UART Send string function.
 * 2-Send Password as a String.
 * */
void sendPassword(uint8*Pass);
/*Function Responsible for
 * 1-Interfacing Program for setting up password and confirming it
 * 2-sends Passwords to Control ECU to make sure they are matching
 *	3-Wait For A reply From control ECU for Validity of Password Matching
 * */
void Entry();
/*function Responsible for displaying Main menu options*/
void mainMenuDisplay(void);
/*Function responsible For displaying Wrong password message when entering wrong one*/
void DisplayError();
/*Call back Function for timer1*/
void callBack();
/*Function Responsible For:
 * 1-Opening ,closing and holding door messages display on LCD
 * 2-Manipulating g_tick variable according to Number of interrupts in TImer0
 * */
void doorStateDisplay();

#endif /* HMI_H_ */
