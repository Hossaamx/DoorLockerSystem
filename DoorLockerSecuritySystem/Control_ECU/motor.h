/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.h
 *
 * Description: Header file for the DC Motor Driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/
#ifndef MOTOR_H_
#define MOTOR_H_


#define DC_MOTOR_PORT_ID PORTD_ID
#define DC_MOTOR_INPUT1_PIN PIN2_ID
#define DC_MOTOR_INPUT2_PIN PIN3_ID



typedef enum{
	Stop, CW, ACW
}DcMotor_State;


void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state);

#endif /* MOTOR_H_ */
