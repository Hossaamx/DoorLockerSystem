/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: header file for the timer Driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer1_PreScaler;

typedef enum{
	NORMAL,COMPARE
}Timer1_Mode;
typedef struct{
	/*A member for defining initial value of timer in TCNT1*/
	uint16 initialValue;
	/*A member for Compare Value*/
	uint16 compareValue;
	/*A member for defining Mode of Timer*/
	Timer1_Mode mode;
	/*A member for defining Prescaler of Timer*/
	Timer1_PreScaler preScaler;
	}Timer1_ConfigType;


void Timer1_init(const Timer1_ConfigType *ConfigPtr );

void Timer1_setCallBack(void(*a_ptr)(void));

void Timer1_Deinit();

#endif /* TIMER_H_ */
