/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the TIMER AVR driver
 *
 * Author: Esmael Ehab
 *
 *******************************************************************************/

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer2CallBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_timer1CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer1CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_timer1CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer1CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPB_vect)
{
	if(g_timer1CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer1CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_timer0CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer0CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_OVF_vect)
{
	if(g_timer0CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer0CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_timer2CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer2CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_timer2CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the event is detected */
		(*g_timer2CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void TIMER_init(TIMER_ConfigTypes * Config_Ptr)
{
	if (Config_Ptr->timer == TIMER_0)
	{
		/* TCCR0 Register Bits Description :
		 *
		 * Non PWM Mode : FOC0=1
		 * No OC0 ,no compare output : COM00=0 & COM01=0
		 */
		TCCR0 |= (1<<FOC0);

		/* Insert the required timer mood (wave generation mood) in the right bits
		 * ( WGM00 , WGM01 ) in TCCR0 register
		 */
		TCCR0 = (TCCR0 & 0xF7) | ((Config_Ptr->mode)<<3);

		/* Insert the required timer frequency prescaler in the first three bits
		 * in TCCR0 register ( CS00 , CS01 , CS02 )
		 */
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->prescaler);

		/* TIMSK Register Bits Description :
		 *
		 * insert the required interrupt mode in the rights bit in TIMSK register
		 * (TOIE0) for interrupt overflow enable
		 * (OCIE0) for interrupt compare match enable
		 */
		TIMSK |= (1<<(Config_Ptr->mode));

		TCNT0 = (Config_Ptr->initial_value);  /* Insert the required initial value in TCNT0 register */
		OCR0 = (Config_Ptr->compare_value);   /*Insert the required compare value in OCR0 register */
	}
	else if (Config_Ptr->timer == TIMER_1)
	{
		if (Config_Ptr->channel == CHANNEL_A)
		{
			/* TCCR1A Register Bits Description :
			 *
			 * Non PWM Mode : FOC1A=1 , FOC1B=1
			 * Disconnect (OCA1,OCB1) ,no compare output : COM1A0/COM1B0=0 & COM1A1/COM1B1=0
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/* Insert the required timer frequency prescaler in the first three bits
			 * in TCCR1B register ( CS10 , CS11 , CS12 )
			 */
			TCCR1B = Config_Ptr->prescaler;

			/* Insert the required timer mood (wave generation mood) in the right bits
			 * ( WGM12 ) in TCCR1B register
			 */
			TCCR1B = (TCCR1B & 0xF7) | ((Config_Ptr->mode)<<WGM12);

			TCNT1 = Config_Ptr->initial_value;  /* Insert the required initial value in TCNT1 register */
			OCR1A = Config_Ptr->compare_value;   /*Insert the required compare value in OCR1A register */

			/* TIMSK Register Bits Description :
			 *
			 * insert the required interrupt mode in the rights bit in TIMSK register
			 * (TOIE1) for interrupt overflow enable
			 * (OCIE1A) for interrupt compare match enable channel A
			 */
			if ( Config_Ptr->mode == COMPARE )
				TIMSK |= (1<<OCIE1A);
			else if ( Config_Ptr->mode == OVERFLOW )
				TIMSK |= (1<<TOIE1);
		}
		else if (Config_Ptr->channel == CHANNEL_B)
		{
			/* TCCR1A Register Bits Description :
			 *
			 * Non PWM Mode : FOC1A=1 , FOC1B=1
			 * Disconnect (OCA1,OCB1) ,no compare output : COM1A0/COM1B0=0 & COM1A1/COM1B1=0
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);

			/* Insert the required timer frequency prescaler in the first three bits
			 * in TCCR1B register ( CS10 , CS11 , CS12 )
			 */
			TCCR1B = Config_Ptr->prescaler;

			/* Insert the required timer mood (wave generation mood) in the right bits
			 * ( WGM12 ) in TCCR1B register
			 */
			TCCR1B = (TCCR1B & 0xF7) | ((Config_Ptr->mode)<<WGM12);

			TCNT1 = Config_Ptr->initial_value;  /* Insert the required initial value in TCNT1 register */
			OCR1B = Config_Ptr->compare_value;   /*Insert the required compare value in OCR1A register */

			/* TIMSK Register Bits Description :
			 *
			 * insert the required interrupt mode in the rights bit in TIMSK register
			 * (TOIE1) for interrupt overflow enable
			 * (OCIE1A) for interrupt compare match enable channel A
			 */
			if ( Config_Ptr->mode == COMPARE )
				TIMSK |= (1<<OCIE1B);
			else if ( Config_Ptr->mode == OVERFLOW )
				TIMSK |= (1<<TOIE1);
		}
	}
	else if (Config_Ptr->timer == TIMER_2)
	{
		/* TCCR0 Register Bits Description :
		 *
		 * Non PWM Mode : FOC2=1
		 * No OC0 ,no compare output : COM20=0 & COM21=0
		 */
		TCCR0 |= (1<<FOC2);

		/* Insert the required timer mood (wave generation mood) in the right bits
		 * ( WGM20 , WGM21 ) in TCCR2 register
		 */
		TCCR2= (TCCR2 & 0xF7) | ((Config_Ptr->mode)<<3);

		/* Insert the required timer frequency prescaler in the first three bits
		 * in TCCR2 register ( CS20 , CS21 , CS22 )
		 */
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->prescaler);

		/* TIMSK Register Bits Description :
		 *
		 * insert the required interrupt mode in the rights bit in TIMSK register
		 * (TOIE2) for interrupt overflow enable
		 * (OCIE2) for interrupt compare match enable
		 */
		if ( Config_Ptr->mode == COMPARE )
			TIMSK |= (1<<OCIE2);
		else if ( Config_Ptr->mode == OVERFLOW )
			TIMSK |= (1<<TOIE2);

		TCNT2 = (Config_Ptr->initial_value);  /* Insert the required initial value in TCNT2 register */
		OCR2 = (Config_Ptr->compare_value);   /*Insert the required compare value in OCR2 register */
	}
}


void TIMER0_setCallBack(void(*a_ptr)(void))
{
	g_timer0CallBackPtr = a_ptr;
}

void TIMER1_setCallBack(void(*a_ptr)(void))
{
	g_timer1CallBackPtr = a_ptr;
}

void TIMER2_setCallBack(void(*a_ptr)(void))
{
	g_timer2CallBackPtr = a_ptr;
}




void TIMER_DeInit(void)
{
	TIMSK = 0;
}


