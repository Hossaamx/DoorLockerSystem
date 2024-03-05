/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.c
 *
 * Description: Source file for the DC Motor Driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/
#include "gpio.h"
#include "motor.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void DcMotor_init(void){
	/*setting the two pins as output pins*/
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT2_PIN, PIN_OUTPUT);
	/*stopping the motor at the beginning*/
	GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT1_PIN, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT2_PIN, LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state){
	switch(state){
	case Stop:
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT1_PIN, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT2_PIN, LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT1_PIN, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT2_PIN, LOGIC_LOW);
		break;
	case ACW:
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT1_PIN, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID, DC_MOTOR_INPUT2_PIN, LOGIC_HIGH);
		break;

	}
}
