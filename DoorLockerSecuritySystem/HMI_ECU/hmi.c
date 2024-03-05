/******************************************************************************
 *
 * Module: HMI_ECU
 *
 * File Name: hmi.c
 *
 * Description: Source file HMI ECU
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/
#include "hmi.h"
#include "util/delay.h"

uint8 g_password[PASSWORD_LENGTH];
uint16 g_seconds=0;
uint8 g_chances=0;
uint8 g_check=PASSWORD_MISMATCHED;



int main(void){
	uint8 key;
	uint8 stateReceived;
	SREG |= (1 << 7);
	uart_ConigType uartconfig_Ptr = { 9600, DISABLED, ONE, EIGHT };
	UART_init(&uartconfig_Ptr);
	/* Timer freq = 8MHz/1024, one clock-cycle time = 128 uSecond
			so to force the timer to produce an interrupt every 1 second:
			we set the compare value to be 1/(128u) = 7813 */
	Timer1_ConfigType timerconfig_ptr = { 0, 7813, COMPARE, F_CPU_1024 };
	Timer1_init(&timerconfig_ptr);
	Timer1_setCallBack(callBack);
	LCD_init();
	LCD_displayString("WELCOME");
	_delay_ms(DISPLAY_PERIOD);
	Entry();
	mainMenuDisplay();
	while(1){
		key=KEYPAD_getPressedKey();
		if(key=='+'){
			LCD_clearScreen();
			LCD_displayString("Enter Password");
			getPassword();
			UART_sendByte(MC_READY_TO_SEND);

			sendPassword(g_password);
			UART_sendByte(UNLOCK_DOOR_REQUEST);
			stateReceived = UART_recieveByte();
			if (stateReceived == UNLOCK_DOOR) {
				doorStateDisplay();
			}
			else if (stateReceived == WRONG_MATCH) { //wrong password
				LCD_clearScreen();
				g_chances++;
				LCD_displayString("Wrong Password,Retry");
				if (g_chances == MAX_CHANCES) {
					g_seconds = 0;
					LCD_clearScreen();
					LCD_displayString("Sus Break In!!");
					while (g_seconds < ONE_MINUTE);
					g_chances = 0;
					g_seconds = 0;
				} else {
					_delay_ms(DISPLAY_PERIOD);
				}
			}
			mainMenuDisplay();
		}
		else if(key=='-'){
			LCD_clearScreen();
			LCD_displayString("Enter Your Password");
			getPassword();
			UART_sendByte(MC_READY_TO_SEND);
			sendPassword(g_password);
			UART_sendByte(CHANGE_PASSWORD_REQUEST);
			stateReceived = UART_recieveByte();
			if (stateReceived == CHANGE_PASSWORD) {
				Entry();
			}
			else if (stateReceived == WRONG_MATCH) {
				LCD_clearScreen();
				g_chances++;
				LCD_displayString("Wrong Password,Retry");
				if (g_chances == MAX_CHANCES) {
					g_seconds = 0;
					LCD_clearScreen();
					LCD_displayString("WRONG!!!");
					while (g_seconds < ONE_MINUTE);
					g_chances = 0;
					g_seconds = 0;
				}
				else {
					_delay_ms(DISPLAY_PERIOD);
				}

		}
			mainMenuDisplay();
		}

	}
}


/*Function responsible for:
 * 1-Function used to prompt user for password on LCD
 * 2-Accepts Input from user on keypad and store it in Global array
 *
 * */
void getPassword() {
	LCD_moveCursor(1, 0);
	uint8 count = 0, key = 0;
	for (count = 0; count < PASSWORD_LENGTH; count++) {
		key = KEYPAD_getPressedKey();
		if (key >= 1 && key <= 9) {
			g_password[count] = key;
			LCD_moveCursor(1, count);
			LCD_displayCharacter('*');
			/*LCD_intgerToString(key);*/

		} else {
			count--;
			continue;
		}
		_delay_ms(500);
	}
	/*Waiting for user to press Enter*/
	while (KEYPAD_getPressedKey() != 13){}

}
void Entry() {
	while (g_check == PASSWORD_MISMATCHED) {
		LCD_clearScreen();
		LCD_displayString("Enter Password:");
		LCD_moveCursor(0, 1);
		getPassword();
		UART_sendByte(MC_READY_TO_SEND);
		while (UART_recieveByte() != MC_READY_TO_RECEIVE){}
		sendPassword(g_password);
		LCD_clearScreen();
		LCD_displayString("Re-Enter Password:");
		LCD_moveCursor(1, 0);
		getPassword();
		UART_sendByte(MC_READY_TO_SEND);
		while (UART_recieveByte() != MC_READY_TO_RECEIVE);
		sendPassword(g_password);
		while(UART_recieveByte()!=MC_READY_TO_SEND);
		g_check = UART_recieveByte();
		if (g_check == PASSWORD_MISMATCHED) {
			LCD_clearScreen();
			LCD_displayString("Not Matching");
			_delay_ms(DISPLAY_PERIOD);

		}


	}
	/*After Matching We should set check to Mismatch for Main Menu*/
	g_check = PASSWORD_MISMATCHED;
}
/*function Responsible for displaying Main menu options*/
void mainMenuDisplay(void){
	LCD_clearScreen();
	LCD_displayString("+:Open Door");
	LCD_displayStringRowColumn(1, 0, "-:Reset Password");
}

/*Function Responsible For:
 * 1-Getting Password in The right Format For UART Send string function.
 * 2-Send Password as a String.
 * */
void sendPassword(uint8*Pass){
	uint8 temp_pass[PASSWORD_LENGTH+2];
	uint8 i=0;
	for(i=0;i<PASSWORD_LENGTH;i++){
		temp_pass[i]=Pass[i];
	}
	temp_pass[5]='#';
	temp_pass[6]='\0';
	UART_sendString(temp_pass);
}

/*Function responsible For displaying Wrong password msg when entering wrong one*/
void DisplayError(){
	LCD_clearScreen();
	LCD_displayString("Wrong Password,Retry");
	_delay_ms(DISPLAY_PERIOD);
}

/*Call back Function for timer1*/
void callBack(){
	g_seconds++;
}

/*Function Responsible For:
 * 1-Opening ,closing and holding door messages display on LCD
 * 2-Manipulating g_tick variable according to Number of interrupts in TImer1*/


void doorStateDisplay(){
	LCD_clearScreen();
	g_seconds=0;
	LCD_displayString("Door is UNLOCKING");
	while(g_seconds< FIFTEEN_SECOND){}
	/*_delay_ms(1500);*/
	LCD_clearScreen();
	g_seconds=0;
	LCD_displayString("Door is UNLOCKED");
	while(g_seconds<THREE_SECOND){}
	LCD_clearScreen();
	g_seconds=0;
	LCD_displayString("Door is LOCKING");
	while(g_seconds< FIFTEEN_SECOND){}
	LCD_clearScreen();
	g_seconds=0;
	LCD_displayString("Door is LOCKED");
	_delay_ms(500);
}

