/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the timer Driver
 *
 * Author: Hossam Hassan
 *
 *******************************************************************************/
#include "timer.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_types.h"
static volatile void(*g_callBackPtr)(void)=NULL_PTR;
static volatile uint8 g_initialTimerValue;

/*Timer overflow ISR*/
ISR(TIMER1_OVF_vect){
	TCNT1=g_initialTimerValue;
	if(g_callBackPtr!=NULL_PTR){
		(*g_callBackPtr)();
	}
}

/*Timer Compare Mode ISR*/
ISR(TIMER1_COMPA_vect){
	TCNT1=g_initialTimerValue;
	if(g_callBackPtr!=NULL_PTR){
		(*g_callBackPtr)();
	}
}

void Timer1_init(const Timer1_ConfigType *ConfigPtr){
	/*Set initial Value of  Counter*/
	TCNT1=ConfigPtr->initialValue;
	g_initialTimerValue=ConfigPtr->initialValue;
	/*Set prescaler for Timer and Applying Non PWM mode*/
	TCCR1B=(((ConfigPtr->preScaler)&0x07)<<CS10);
	CLEAR_BIT(TCCR1A, WGM10);
	CLEAR_BIT(TCCR1A, WGM11);
	CLEAR_BIT(TCCR1B, WGM13);

	if (ConfigPtr->mode == NORMAL) {

		CLEAR_BIT(TCCR1B, WGM12);
		/*Enabling Overflow Interrupt enable*/
		SET_BIT(TIMSK,TOIE1);
	}
	/*In case of Compare mode*/
	else {
		/*Setting Compare Value to value in passed Structure*/
		OCR1A = ConfigPtr->compareValue;
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1A,FOC1A);

		/*Enabling Output compare match Interrupt Enable*/
		SET_BIT(TIMSK, OCIE1A);
	}
}
void Timer1_Deinit(){
	TCCR1B=0;
	/*CLEAR_BIT(TIMSK,OCIE0);
	CLEAR_BIT(TIMSK,TOIE0);*/
}
void Timer1_setCallBack(void(*a_ptr)(void)){
	g_callBackPtr=a_ptr;
}
