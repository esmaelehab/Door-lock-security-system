/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: header file for the TIMER AVR driver
 *
 * Author: Esmael Ehab
 *
 *******************************************************************************/

#ifndef IIMER0_H_
#define TIMER0_H_

#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	TIMER_0, TIMER_1, TIMER_2
}TIMER_ID;

typedef enum
{
	NO_CHANNEL, CHANNEL_A, CHANNEL_B
}CHANNEL_ID;

typedef enum
{
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}TIMER_prescaler;

typedef enum
{
	OVERFLOW, COMPARE
}TIMER_mood;

typedef struct
{
	TIMER_ID timer;
	CHANNEL_ID 	channel;
	TIMER_mood mode;
	TIMER_prescaler prescaler;
	uint8 initial_value;
	uint16 compare_value;
}TIMER_ConfigTypes;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description: Function to initialize TIMER0 driver
 * 1. Set the required clock.
 * 2. Set the required mood.
 * 3. and the required initial and compare value (in case of compare mode).
 */
void TIMER_init(TIMER_ConfigTypes * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER0_setCallBack(void(*a_ptr)(void));
void TIMER1_setCallBack(void(*a_ptr)(void));
void TIMER2_setCallBack(void(*a_ptr)(void));
/*
 *  Description: Function to disable Timer0
 */
void TIMER_DeInit(void);

#endif /* TIMER0_H_ */

