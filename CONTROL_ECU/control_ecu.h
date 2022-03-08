/******************************************************************************
 *
 * Module: CONTROL_ECU
 *
 * File Name: control_ecu.h
 *
 * Description: header file for the control ECU
 *
 * Author: Esmael Ehab
 *
 *******************************************************************************/

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

/*******************************************************************************
 *                             Definitions                                     *
 *******************************************************************************/
/*   definition for control_ecu   */
#define TWI_ADDRESS    0b0000001
#define PASSWORD_SIZE          5
#define SEND_RECEIVE_TIME     10
#define STORE_TIME            80
#define MAX_NUM_OF_MISTAKES    3
#define OPENING_DOOR_TIME     15
#define HOLDING_DOOR_TIME      3
#define CLOSING_DOOR_TIME     15
#define WARNING_TIME          60

/*   definitions for uart commands with hmi_ecu   */
#define MISMATCHED             0
#define MATCHED                1
#define OPEN_DOOR             '+'
#define CHANGE_PASSWORD       '-'
#define OPENING_DOOR          ')'
#define WRONG_PASSWORD        '_'
#define CHANGING_PASSWORD     '('
#define READY_TO_SEND         '$'
#define READY_TO_RECEIVE      '&'
#define RECEIVE_DONE          '@'
#define SEND_FIRST_PASSWORD   '^'
#define SEND_CONFIRM_PASSWORD '?'
#define SEND_CHECK_PASSWORD   ','

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* Description :
 * function that set a new password
 * if : 1- application start for the first time
 *      2- user choose to change the password
 */
void APP_newPassword(void);

/* Description :
 * function takes the password send for hmi_ecu by uart
 * store it in the argument a_Password[]
 */
void APP_receivePassword(char a_Password[], char SIZE);

/* Description :
 * function that compare the two passwords sent from hmi_ecu by uart
 * and return the match status (matched, mismatched)
 */
char APP_comparePassword(char a_password1[], char a_password2[], char SIZE1, char SIZE2);

/* Description :
 * function that save the matched password in EEPROM
 */
void APP_savePassword(char a_receivedPassword[], char SIZE);

/* Description :
 * function that gets the password from EEPROM
 * and store it in a_storedPassword
 */
void APP_readPassword(char a_storedPassword[], char SIZE);

/* Description :
 * function that initiate the timer with the required configuration
 * to calculate the seconds
 */
void APP_startTimer(void);

/* Description :
 * function that set the call back with timer
 */
void TimerCallBackProcessing(void);

/* Description :
 * function that start the wrong password task
 * opens the buzzer if the password mistakes was 3
 */
void APP_wrongPassword(void);

/* Description :
 * function that start opening door task
 * opens the dcmotor and hold it then close it
 */
void APP_openingDoor(void);

/* Description :
 * function receive commands by the UART from hmi_ecu
 * and return that commands
 */
char APP_receiveFromHMI_ECU(void);

/* Description :
 * function send command to the UART from control_ecu
 */
void APP_sendToHMI_ECU(char UART_command);

#endif /* CONTROL_ECU_H_ */
