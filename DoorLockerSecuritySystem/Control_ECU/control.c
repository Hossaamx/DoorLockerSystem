
/******************************************************************************
 *
 * Module: CONTROL_ECU
 *
 * File Name: control.c
 *
 * Description: Source file CONTROL ECU
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/
#include "control.h"
uint8 g_receivedPassword[PASSWORD_LENGTH];
uint8 g_confirmationPassword[PASSWORD_LENGTH];
uint8 g_eeprom_password[PASSWORD_LENGTH];
uint8 g_chances=0;
uint16 g_seconds;

int main(void){
	SREG|=(1<<7);
	uart_ConigType uartconfig_Ptr = { 9600, DISABLED, ONE, EIGHT };
	UART_init(&uartconfig_Ptr);
	Timer1_ConfigType timerconfig_ptr = { 0, 7813, COMPARE, F_CPU_1024 };
	Timer1_init(&timerconfig_ptr);
	Timer1_setCallBack(callBack);
	TWI_TypeConfig twiconfig_ptr = { MC_SLAVE_ADDRESS, Fast };
	TWI_init(&twiconfig_ptr);
	Buzzer_init();
	DcMotor_init();
	uint8 stateReceived;/*According to command sent via UART we are going to alter flow*/
	Entry();
	while (1) {
		if (UART_recieveByte() == MC_READY_TO_SEND) {
			UART_receiveString(g_receivedPassword);
			stateReceived = UART_recieveByte();
			if (stateReceived == UNLOCK_DOOR_REQUEST) {
				if (comparePasswords(g_eeprom_password,
						g_receivedPassword) == PASSWORD_MATCHED) {
					UART_sendByte(UNLOCK_DOOR);
					openDoor();
				} else {
					UART_sendByte(WRONG_MATCH);
					g_chances++;
					if (g_chances == MAX_CHANCES) {
						Buzzer_ON();
						g_seconds= 0;
						while (g_seconds < ONE_MINUTE)
							;
						Buzzer_OFF();
						g_chances = 0;
					}
				}
			} else if (stateReceived == CHANGE_PASSWORD_REQUEST) {
				if (comparePasswords(g_eeprom_password,
						g_receivedPassword)==PASSWORD_MATCHED) {
					UART_sendByte(CHANGE_PASSWORD);
					Entry();
				} else {
					UART_sendByte(WRONG_MATCH);
					g_chances++;
					if (g_chances == MAX_CHANCES) {
						Buzzer_ON();
						g_seconds = 0;
						while (g_seconds < ONE_MINUTE)
							;
						Buzzer_OFF();
						g_chances = 0;
					}
				}
			}

		}
	}
}

/*Call back Function*/
void callBack(){
	g_seconds++;
}
/*Description:
 * 1-Function Receives Password to be set and password to be received by UART
 * 2-Compare Passwords and send Confirmation or Wrong Entry to MC1
 * 3-IF passwords matched ,it stores password in EEPROM
 * 4-If not Sends UNMATCHING to repormpt user in MC1 to re-enter Password
 * */
void Entry(){
	uint8 check=PASSWORD_MISMATCHED;
	while(check== PASSWORD_MISMATCHED){
		while(UART_recieveByte()!=MC_READY_TO_SEND);
		UART_sendByte(MC_READY_TO_RECEIVE);
		UART_receiveString(g_receivedPassword);
		/*For Receiving Confirmation password for the password to be set*/
		while (UART_recieveByte() != MC_READY_TO_SEND);
		UART_sendByte(MC_READY_TO_RECEIVE);
		UART_receiveString(g_confirmationPassword);
		if(comparePasswords(g_receivedPassword,g_confirmationPassword)==PASSWORD_MATCHED){
			UART_sendByte(MC_READY_TO_SEND);
			UART_sendByte(PASSWORD_MATCHED);
			/*we need to store Password in e2prom*/
			check = PASSWORD_MATCHED;
			storePassword_e2prom();
		}
		else{
			UART_sendByte(MC_READY_TO_SEND);
			UART_sendByte(PASSWORD_MISMATCHED);
		}

	}
}

/*Function responsible for reading password from EEPROM and storing it global array*/
void readPassword_e2prom(){
	uint8 counter=0;
		for(counter=0;counter<PASSWORD_LENGTH;counter++){
			EEPROM_readByte(counter, g_eeprom_password+counter);
		}
}
uint8 comparePasswords(uint8*a_pass1,uint8*a_pass2){
	readPassword_e2prom();
	uint8 counter = 0;
	for (counter = 0; counter < PASSWORD_LENGTH; counter++) {
		if (a_pass1[counter] != a_pass2[counter]) {
			return PASSWORD_MISMATCHED;
		}
	}
	return PASSWORD_MATCHED;
}
/*Description:
 *Function responsible for storing Password in eeprom in case of matching
 * */
void storePassword_e2prom(){
	uint8 counter=0;
	for(counter=0;counter<PASSWORD_LENGTH;counter++){
		EEPROM_writeByte(counter, g_receivedPassword[counter]);
		_delay_ms(100);
	}
}

void openDoor(){
	g_seconds=0;
	DcMotor_Rotate(CW);
	while(g_seconds<FIFTEEN_SECOND);
	g_seconds=0;
	DcMotor_Rotate(Stop);
	while(g_seconds<THREE_SECOND);
	g_seconds=0;
	DcMotor_Rotate(ACW);
	while(g_seconds<FIFTEEN_SECOND);
	DcMotor_Rotate(Stop);
	g_seconds=0;
}
