/*
 * application.h
 *
 *  Created on: Feb 24, 2023
 *      Author: mohammedhamdy32
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

/*** libraries ***/
#include "std_types.h"


/****** #define ******/
#define  DELAY_BETWEEN_CLICKS_IN_MS        500   /*The delay between two clicks on keypad*/
#define  DELAY_BETWEEN_TWO_TESTS__IN_MS    50
#define  DELAY_BETWEEN_INPUT_AND_OUTPUT    50
#define  DELAY_BETWEEN_TWO_EEPROM_RW       10
#define  REMOVING_BUTTON               	   '/'
#define  OK_BUTTON                         ' '
#define  END_BUTTON                        '='

#define  FIRST_TESTING_PORT_ID              PORTB_ID
#define  SECOND_TESTING_PORT_ID             PORTD_ID

#define  MAX_IC_NAME_SIZE                   6
#define  MAX_PINS_FOR_IC                    16

#define  INPUT_PIN                          0  /*Input  pin according to IC, which means the pin is taking an input*/
#define  OUTPUT_PIN                         1  /*output pin according to IC, which means the pin is producing an output*/


#define   AVIRABLE_ADDRESS_MSBs             0
#define   AVIRABLE_ADDRESS_LSBs             1


/*** Functions ***/
uint8 take_input_from_keypad(uint8 * a_entered_word ); /*It takes the entered buttons from keypad till the user enter OK*/
void  max_size_error_display(uint8 ); /*This function display a max error massage on LCD*/
uint8 test_the_ic(  uint32   a_io_map ,  uint32 a_test , uint8 a_size);
uint32 search_IC_name(uint32 req_IC_name , uint8 *no_test_cases , uint16 *PIN_confg);
uint32 change_array_to_BCD(uint8 *a_arr , uint8 a_size);
void adjust_testcase_to_16_bit(uint8 *a_arr, uint8 a_size);

#endif /* APPLICATION_H_ */
