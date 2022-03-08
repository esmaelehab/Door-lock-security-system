/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: header file for the Buzzer driver
 *
 * Author: Esmael Ehab
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/
/* The Ports and Pins IDs for the buzzer pin */
#define BUZZER_PORT_ID   PORTD_ID
#define BUZZER_PIN_ID    PIN6_ID


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/* Description:
 * set the buzzer pin direction as output pin by gpio
 */
void BUZZER_init(void);

/* Description:
 * turn on the buzzer by writing logic one in the buzzer pin by gpio
 */
void BUZZER_on(void);

/* Description:
 * turn off the buzzer by writing logic zero in the buzzer pin by gpio
 */
void BUZZER_off(void);

#endif /* BUZZER_H_ */
