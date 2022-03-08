/******************************************************************************
 *
 * Module: CONTROL_ECU
 *
 * File Name: control_ecu.c
 *
 * Description: source file for the control ECU
 *
 * Author: Esmael Ehab
 *
 *******************************************************************************/

#include "micro_config.h"
#include "control_ecu.h"
#include <avr/io.h>
#include <avr/delay.h>
#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "timer.h"

char g_receivedPassword[PASSWORD_SIZE];   /*global array to receive the password from hmi_ecu*/

char g_confirmPassword[PASSWORD_SIZE];    /*global array to receive the confirmation password*/

char g_storedPassword[PASSWORD_SIZE];     /*global password to store the password in the EEPROM*/

char g_matchStatus = MISMATCHED;          /*global variable that hold the matching status*/

uint8 g_passwordMistake=0;                /*global variable that hold the number of input password mistakes*/

uint8 g_tick = 0;                         /*global variable that count the seconds by timer*/

char UART_command;                        /*global variable that hold the command from hmi_ecu*/

int main(void)
{
	/* Enable global interrupts by setting I-bit */
	SREG  |= (1<<7);

	/* initialize the UART with configuration :
	 * (8 data bits, one stop bit, no parity bit, 9600 baud rate)
	 */
	UART_ConfigTypes UART_Config = {DATA_BITS_8, ONE_STOP_BIT, NO_PARITY, 9600};
	UART_init(&UART_Config);


	/* initialize the TWI with configuration :
	 * (zero prescaler(bit rate 400.000 kbps), and give an address to the CONTROL_ECU TWI
	 */
	TWI_ConfigTypes TWI_Config = {FAST_MODE_400Kb, Prescaler_1, TWI_ADDRESS};
	TWI_init(&TWI_Config);

	/* initialize the dc motor and the buzzer */
	DCMOTOR_init();
	BUZZER_init();

	/* save the password for the first time */
	APP_newPassword();

	uint8 mainOptions_key;

	while(1)
	{
		/* 1-Wait until hmi_ecu send command to inform the input password
		 * 2-Receive the input password from hmi_ecu
		 * 3-Receive user choice from hmi_ecu
		 */
		while(APP_receiveFromHMI_ECU() != SEND_CHECK_PASSWORD);
		APP_receivePassword(g_receivedPassword, PASSWORD_SIZE);
		mainOptions_key = APP_receiveFromHMI_ECU();

		switch (mainOptions_key)
		{
		/* in case user chosen open door*/
		case OPEN_DOOR :

			/*1-Read the password from the EEPROM and
			  2-compare it with the user input password
			 */
			APP_readPassword(g_storedPassword, PASSWORD_SIZE);
			g_matchStatus = APP_comparePassword(g_receivedPassword, g_storedPassword, PASSWORD_SIZE, PASSWORD_SIZE);

			/*if the two password are the same*/
			if (g_matchStatus == MATCHED)
			{
				/* 1-Send opening door command to hmi_ecu
				 * 2-Start opening door task
				 */
				APP_sendToHMI_ECU(OPENING_DOOR);
				APP_openingDoor();
			}
			/*if the two passwords aren't the same*/
			else if(g_matchStatus == MISMATCHED)
			{
				/* 1-Send wrong password command to hmi_ecu
				 * 2-Start wrong password command
				 */
				APP_sendToHMI_ECU(WRONG_PASSWORD);
				APP_wrongPassword();
			}
			break;

			/*in case user chosen change password*/
		case CHANGE_PASSWORD :
			/*1-Read the password from the EEPROM and
              2-compare it with the user input password
			 */
			APP_readPassword(g_storedPassword, PASSWORD_SIZE);
			g_matchStatus = APP_comparePassword(g_receivedPassword, g_storedPassword, PASSWORD_SIZE, PASSWORD_SIZE);

			/*if the two password are the same*/
			if(g_matchStatus == MATCHED)
			{
				/* 1-Send changing password command to hmi_ecu
				 * 2-Start new password task
				 */
				APP_sendToHMI_ECU(CHANGING_PASSWORD);
				APP_newPassword();
			}
			else if(g_matchStatus == MISMATCHED)
			{
				/* 1-Send wrong password command to hmi_ecu
			     * 2-Start wrong password command
				 */
				APP_sendToHMI_ECU(WRONG_PASSWORD);
				APP_wrongPassword();
			}
			break;
		}
	}

}

void APP_newPassword(void)
{
	g_matchStatus = MISMATCHED;  /* set it to mismatched for later new passwords */

	/* doesn't terminate until the the CONTROL_ECU gets the same two password*/
	while (g_matchStatus == MISMATCHED)
	{
		/* 1-Wait until the hmi_ecu send command to inform for the first password
		 * 2-Receive the first password from hmi_ecu
		 */
		while (APP_receiveFromHMI_ECU() != SEND_FIRST_PASSWORD );
		APP_receivePassword(g_receivedPassword, PASSWORD_SIZE);

		/* 1-wait until the hmi_ecu is send command to inform for the confirm password
		 * 2-receive the confirm password from hmi_ecu
		 * 3-Compare the two passwords
		 */
		while (APP_receiveFromHMI_ECU() != SEND_CONFIRM_PASSWORD );
		APP_receivePassword(g_confirmPassword, PASSWORD_SIZE);
		g_matchStatus = APP_comparePassword(g_receivedPassword, g_confirmPassword, PASSWORD_SIZE, PASSWORD_SIZE);
		/*in case password mismatched*/
		if (g_matchStatus == MISMATCHED)
		{
			/*send command with confirmation status witch is mismatched*/
			APP_sendToHMI_ECU(MISMATCHED);
		}
		/*in case the two password are the same*/
		else if (g_matchStatus == MATCHED)
		{
			/* 1- Save the password in the EEPROM
 			 * 2-send command with confirmation status witch is mismatched
			 */
			APP_savePassword(g_receivedPassword, PASSWORD_SIZE);
			APP_sendToHMI_ECU(MATCHED);
		}
	}
}

void APP_receivePassword(char a_Password[], char SIZE)
{
	uint8 i;
	for (i=0; i<PASSWORD_SIZE; i++)
	{
		a_Password[i] = UART_recieveByte();  /*receive the password from hmi_ecu*/
		_delay_ms(SEND_RECEIVE_TIME);        /*delay for the sending reciving time between the ECUS*/
	}
}

char APP_comparePassword(char a_password1[],char a_password2[], char SIZE1, char SIZE2)
{
	uint8 i;
	/*compare each number in the password*/
	for (i=0; i<PASSWORD_SIZE; i++)
	{
		if (a_password1[i] != a_password2[i])
		{
			return MISMATCHED;
		}
	}
	return MATCHED;
}

void APP_savePassword(char a_receivedPassword[], char SIZE)
{
	/* save every byte of the password in the EEPROM */
	uint8 i;
	for (i=0; i<PASSWORD_SIZE; i++)
	{
		EEPROM_writeByte(0x0311+i, a_receivedPassword[i]);
		_delay_ms(STORE_TIME);
	}
}

void APP_readPassword(char a_storedPassword[], char SIZE)
{
	/* get the password from EEPROM and store in the array */
	uint8 i;
	for(i=0; i<PASSWORD_SIZE; i++)
	{
		EEPROM_readByte(0x0311+i, &a_storedPassword[i]);
		_delay_ms(STORE_TIME);
	}
}

void APP_openingDoor(void)
{
	/*start the timer to count the seconds*/
	APP_startTimer();

	/*rotate the motor clock wise for 15 seconds (opening door task)*/
	DCMOTOR_rotate(DC_MOTOR_CW);
	while(g_tick != OPENING_DOOR_TIME);
	g_tick = 0;

	/*hold the motor 3 seconds (holding door task)*/
	DCMOTOR_rotate(DC_MOTOR_STOP);
	while(g_tick != HOLDING_DOOR_TIME);
	g_tick = 0;

	/*rotate the motor anti clock wise for 15 seconds (closing door task)*/
	DCMOTOR_rotate(DC_MOTOR_ACW);
	while(g_tick != CLOSING_DOOR_TIME);
	g_tick = 0;

	/*stop the motor (door is closed)*/
	DCMOTOR_rotate(DC_MOTOR_STOP);

	TIMER_DeInit(); /*stop the timer*/
}

void APP_startTimer(void)
{
	/* initialize the TIMER with configuration :
	 * (compare mode (CTC), initial value=0, prescaler 1024)
	 * F(timer)=8Mhz/1024=7812.5hz so the cycle = 128us so to make an interrupt
	 * every 1 second we set the compare value to 1s/128us = 7813
	 */
	TIMER_ConfigTypes TIMER_Config = {TIMER_1, CHANNEL_A, COMPARE, F_CPU_1024, 0, 7813};
	/*start the timer*/
	TIMER_init(&TIMER_Config);
	/* set the call back function called by the timer */
	TIMER1_setCallBack(TimerCallBackProcessing);
}

void APP_wrongPassword(void)
{
	g_passwordMistake++;
	/*in case input password was wrong 3 times start buzzer for 1 minute*/
	if (g_passwordMistake == MAX_NUM_OF_MISTAKES)
	{
		/*start the timer to count the seconds*/
		APP_startTimer();

		BUZZER_on();
		while(g_tick != WARNING_TIME);
		g_tick = 0;
		g_passwordMistake = 0;

		/*stop the timer*/
		TIMER_DeInit();
	}
	BUZZER_off();
}

void TimerCallBackProcessing(void)
{
	g_tick++;
}

void APP_sendToHMI_ECU(char UART_command)
{
	/* 1-inform hmi_ecu that you are ready to send
	 * 2-wait until hmi_ecu ready to receive
	 * 3-send the required command to hmi_ecu
	 * 4-wait until hmi_ecu receive it
	 */
	UART_sendByte(READY_TO_SEND);
	while(UART_recieveByte() != READY_TO_RECEIVE);
	UART_sendByte(UART_command);
	while(UART_recieveByte() != RECEIVE_DONE){};
}

char APP_receiveFromHMI_ECU(void)
{
	/* 1-wait until hmi_ecu ready to send
	 * 2-inform hmi_ecu that you are ready to receive
	 * 3-receive the command from chmi_ecu
	 * 4-inform hmi_ecu that you received the command
	 */
	while(UART_recieveByte() != READY_TO_SEND);
	UART_sendByte(READY_TO_RECEIVE);
	UART_command = UART_recieveByte();
	UART_sendByte(RECEIVE_DONE);
	return UART_command;
}



