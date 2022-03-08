/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Esmael Ehab
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Types Declaration                                      *
 *******************************************************************************/

typedef enum
{
	DATA_BITS_5, DATA_BITS_6, DATA_BITS_7, DATA_BITS_8, DATA_BITS_9=7
}UART_DataBits;

typedef enum
{
	ONE_STOP_BIT, TWO_STOP_BIT
}UART_StopBits;

typedef enum
{
    NO_PARITY, RESERVED, EVEN_PARITY, ODD_PARITY
}UART_ParityMood;

typedef struct
{
    UART_DataBits data_bits;
	UART_StopBits stop_bits;
	UART_ParityMood parity_bits;
	uint32 baud_rate;
}UART_ConfigTypes;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigTypes * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */


