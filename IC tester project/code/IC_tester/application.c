/*
 * application.c
 *
 *  Created on: Feb 24, 2023
 *      Author: mohammedhamdy32
 */


/*** libraries ***/
#include "application.h"
#include "keypad.h"
#include "lcd.h"
#include "std_types.h"
#include "common_macros.h"
#include "external_eeprom.h"

#include "avr/io.h"

/****************************************************************************
 * By : Mohammed hamdy
 *  This function is used to take an input from interfacing user by keypad,
 *  it takes input till the user press OK, This function writes on the
 *  second row on LCD only because the first row is describing what will
 *  be the entered.
 *
 *  !! take care, this function stores the array in BCD form
 *
 * @param a_a_entered_word : to save the word in it
 *
 * @return : void
 ****************************************************************************/
uint8 take_input_from_keypad(uint8 * a_entered_word)
{
	uint8 size=0;

	LCD_moveCursor(1,0);  /*Also starting from the second row*/

	uint8 pressed_key ;

	while(1){

		pressed_key = KEYPAD_getPressedKey();
		_delay_ms(DELAY_BETWEEN_CLICKS_IN_MS);

		if(REMOVING_BUTTON == pressed_key) /*The user wants to remove an char*/
		{
			if(size > 0)
			{
				size--;
				LCD_moveCursor(1,size);
				LCD_writeChar(' ');
				LCD_moveCursor(1,size);
			}
		}
		else if(OK_BUTTON == pressed_key) /*The user entered the word and pressed OK */
		{
			a_entered_word[size] = '\0';
			return size;
		}
		else if( '*' != pressed_key  &&  '-' != pressed_key  &&  '+' != pressed_key && '='  != pressed_key) /*The user entered a number*/
		{
			LCD_integerToString(pressed_key);
			a_entered_word[size] = pressed_key;
			size ++;
		}else
		{
			/*Do nothing*/
		}


	}/*while bracket*/


}


/**********************************************************************
 * Mohammed hamdy
 * This function display a max error message on LCD
 *
 * @param a_size : The max size
 *
 **********************************************************************/
void max_size_error_display(uint8 a_size)
{
	LCD_clearScreen();
	LCD_writeString("The max size is");
	LCD_moveCursor(1,0);
	LCD_integerToString(a_size);
	LCD_writeString(" number only");
	_delay_ms(3000); /*delay for 3sec*/
	LCD_clearScreen();
}

/***********************************************************************
 * This function is calculating the power 2 to a a number
 * @param a_num
 * @return the power
 ***********************************************************************/
uint16 power(uint8 a_num)
{
	uint16 result = 1;
	for(uint8 i=0;i<a_num;i++)
		result = result * 2;
	return result;
}


/**********************************************************************
 * Mohammed hamdy
 *
 * This function tests the IC, it takes three arguments, The first argument
 * a_io_map is the mapping of input and output pins of the IC, 0 for input
 * IC pin, and 1 for output IC pin
 * The second argument a_test is the test case which the pins must be like it
 * The third argument a_size is the pins number of the IC
 *
 * This function test the This case and returns 1 if the test passes,
 * an returns 0 if the test fails
 *
 * @return 1 if the test is true and 0 if false
 ***********************************************************************/
uint8 test_the_ic(uint32 a_io_map , uint32  a_test , uint8 a_size)
{
	uint8 i,j;
	uint8 io_bit;
	uint8 test_bit;

	uint8 size_of_half = a_size/2;




	GPIO_setupPortDirection(FIRST_TESTING_PORT_ID,(uint8)~(a_io_map>>8) );
	GPIO_writePort(FIRST_TESTING_PORT_ID, (uint8)a_test);

	GPIO_setupPortDirection(SECOND_TESTING_PORT_ID, (uint8)~a_io_map );
	GPIO_writePort(SECOND_TESTING_PORT_ID, (uint8)(a_test>>8) );




	//	for(i=0 ; i<size_of_half && i < 8 ; i++) /*For first 8 pins of IC*/
	//	{
	//
	//		io_bit   =  GET_BIT(a_io_map,i);  /*To know if the bit i is input or output */
	//		test_bit =  GET_BIT(a_test,i);    /*To know if the bit i is logic high or logic low*/
	//
	//		if( INPUT_PIN == io_bit ) /* Pin i is input for IC */
	//		{
	//			GPIO_setupPinDirection(FIRST_TESTING_PORT_ID , i , OUTPUT); /*The direction is output because the IC pin is input, so I will write on it*/
	//			GPIO_writePin(FIRST_TESTING_PORT_ID , i , test_bit ); /*What value will be righten on pin */
	//		}
	//
	//
	//	} /*Will out from for statement if it counts all pins or it counts 8 pins only*/
	//
	//
	//
	//	/*put 0 at all remaining pins to make sure that it will not affects on test*/
	//	j=i;
	//	while(j < 8)
	//	{
	//		GPIO_setupPinDirection(FIRST_TESTING_PORT_ID , j , OUTPUT); /*The direction is output because the IC pin is input, so I will write on it*/
	//		GPIO_writePin(FIRST_TESTING_PORT_ID , j , LOGIC_LOW ); /*What value will be righten on pin */
	//		j++;
	//	}


	//	j=0;
	//
	//	for( ; i<a_size && i < 16 ; i++,j++) /*For pins from 8 to 16 in IC*/
	//	{
	//		io_bit   =  GET_BIT(a_io_map,i);  /*To know if the bit i is input or output */
	//		test_bit =  GET_BIT(a_test,i);    /*To know if the bit i is logic high or logic low*/
	//
	//		if( INPUT_PIN == io_bit ) /* Pin i is input for IC */
	//		{
	//			GPIO_setupPinDirection(SECOND_TESTING_PORT_ID , j , OUTPUT); /*The direction is output because the IC pin is input, so I will write on it*/
	//			GPIO_writePin(SECOND_TESTING_PORT_ID , j , test_bit ); /*What value will be righten on pin */
	//		}
	//	}
	//
	//	/*put 0 at all remaining pins to make sure that it will not affects on test*/
	//	while(j < 16)
	//	{
	//		GPIO_setupPinDirection(SECOND_TESTING_PORT_ID , j , OUTPUT); /*The direction is output because the IC pin is input, so I will write on it*/
	//		GPIO_writePin(SECOND_TESTING_PORT_ID , j , LOGIC_LOW ); /*What value will be righten on pin */
	//		j++;
	//	}


	_delay_ms(DELAY_BETWEEN_INPUT_AND_OUTPUT); /*make a delay between putting input and checking the output*/

	/*to test if the port is like the expected port, if yes to this test passes, if not to this test failed */

	if( GPIO_readPort(FIRST_TESTING_PORT_ID)  ==   (a_test & 0x00FF )  /*I made AND with adjust_test which is has one's number according to half number of pins*/
	 && GPIO_readPort(SECOND_TESTING_PORT_ID) == ( (a_test & 0XFF00 ) >>8 ) )
	{/*This test passes*/
		LCD_clearScreen();
		return 1;
	}else
	{
		/*This test fails */
//		LCD_clearScreen();
//		LCD_integerToString(DDRB);
//		LCD_moveCursor(1,0);
//		LCD_integerToString( ( (a_test & 0XFF00 ) >>8 ) );
//		KEYPAD_getPressedKey();
//		_delay_ms(500);
//		LCD_clearScreen();
//		LCD_integerToString(a_test);
//		KEYPAD_getPressedKey();

		return 0;
	}

}


/*******************************************************************************
 * EZZAT
 *******************************************************************************/
//uint16 search_IC_name(uint32 req_IC_name,uint8 *no_test_cases,uint16 *PIN_confg)
//{
//	uint16 last_address=0;
//	uint16 current_address=0x0002;
//	uint16 next_address=0;
//	uint32 current_data=0;
//
//
//	/*read the last address in EEPROM*/
//	EEPROM_readByte(0x0000,&last_address);
//	_delay_ms(10);
//	last_address=last_address<<8;
//	EEPROM_readByte(0x0001,&last_address);
//	_delay_ms(10);
//
//
//	/*Read the next address from EEPROM*/
//	EEPROM_readByte(current_address+3,&next_address);
//	_delay_ms(10);
//	next_address=next_address<<8;
//	EEPROM_readByte(current_address+4,&next_address);
//	_delay_ms(10);
//
//	/*Read the current data(IC name) from EEPROM*/
//	EEPROM_readByte(current_address,&current_data);
//	_delay_ms(10);
//	current_data=current_data<<8;
//	EEPROM_readByte(current_address+1,&current_data);
//	_delay_ms(10);
//
//
//
//	while(1)
//	{
//
//		if(current_address == last_address){
//			return -1;
//		}
//
//
//
//
//
//		if(current_data == req_IC_name)
//		{
//
//			*no_test_cases =  ( ( (next_address  - current_address -7) / 2 ) ) ;
//
//
//			/*Read the PIN configuration from EEPROM*/
//			EEPROM_readByte(current_address+0x05,PIN_confg);
//			_delay_ms(10);
//			(*PIN_confg)=(*PIN_confg)<<8;
//			EEPROM_readByte(current_address+0x06,PIN_confg);
//			_delay_ms(10);
//
//			return current_address;
//		}
//		else
//		{
//			current_address=next_address;
//		}
//
//		/*Read the next address from EEPROM*/
//		EEPROM_readByte(current_address+3,&next_address);
//		_delay_ms(10);
//		next_address=next_address<<8;
//		EEPROM_readByte(current_address+4,&next_address);
//		_delay_ms(10);
//
//		/*Read the current data(IC name) from EEPROM*/
//		EEPROM_readByte(current_address,&current_data);
//		_delay_ms(10);
//		current_data=current_data<<8;
//		EEPROM_readByte(current_address+1,&current_data);
//		_delay_ms(10);
//	}
//}
//
uint32 search_IC_name(uint32 req_IC_name,uint8 *no_test_cases,uint16 *PIN_confg)
{
	uint16 last_address=0;
	uint16 current_address=0x0002;
	uint16 next_address=0;
	uint32 current_data=0;


	//read the last address in EEPROM/
	EEPROM_readByte(0x0000,&last_address);
	_delay_ms(10);
	last_address=last_address<<8;
	EEPROM_readByte(0x0001,&last_address);
	_delay_ms(10);



	//Read the next address from EEPROM/
	EEPROM_readByte(current_address+3,&next_address);
	_delay_ms(10);
	next_address=next_address<<8;
	EEPROM_readByte(current_address+4,&next_address);
	_delay_ms(10);

	//Read the current data(IC name) from EEPROM/
	EEPROM_readByte(current_address,&current_data);
	_delay_ms(10);
	current_data=current_data<<8;
	EEPROM_readByte(current_address+1,&current_data);
	_delay_ms(10);
	current_data=current_data<<8;
	EEPROM_readByte(current_address+2,&current_data);
	_delay_ms(10);



	while(1)
	{

		if(current_address == last_address){
			return -1;
		}

//		LCD_clearScreen();
//		LCD_integerToString((uint8)current_data);
//		LCD_writeChar(' ');
//		LCD_integerToString((uint8)(current_data>>8));
//		LCD_writeChar(' ');
//		LCD_integerToString((uint8)(current_data>>16));
//		LCD_writeChar(' ');
//		LCD_moveCursor(1,0);
//		LCD_integerToString((uint8)req_IC_name);
//		LCD_writeChar(' ');
//		LCD_integerToString((uint8) (req_IC_name>>8) );
//		LCD_writeChar(' ');
//		LCD_integerToString((uint8)(req_IC_name>>16));
//		LCD_writeChar(' ');
//		KEYPAD_getPressedKey();
//		_delay_ms(100);





		if(current_data == req_IC_name)
		{

			*no_test_cases =  ( ( (next_address  - current_address -7) / 2 ) ) ;


			//Read the PIN configuration from EEPROM/
			EEPROM_readByte(current_address+0x05,PIN_confg);
			_delay_ms(10);
			(*PIN_confg)=(*PIN_confg)<<8;
			EEPROM_readByte(current_address+0x06,PIN_confg);
			_delay_ms(10);

			return current_address;
		}
		else
		{
			current_address=next_address;
		}

		//Read the next address from EEPROM/
		EEPROM_readByte(current_address+3,&next_address);
		_delay_ms(10);
		next_address=next_address<<8;
		EEPROM_readByte(current_address+4,&next_address);
		_delay_ms(10);

		current_data = 0;
		//Read the current data(IC name) from EEPROM/
		EEPROM_readByte(current_address,&current_data);
		_delay_ms(10);
		current_data=current_data<<8;
		EEPROM_readByte(current_address+1,&current_data);
		_delay_ms(10);
		current_data=current_data<<8;
		EEPROM_readByte(current_address+2,&current_data);
		_delay_ms(10);
	}
}

/******************************************************************
 *  ezzat
 *****************************************************************/
uint32 change_array_to_BCD(uint8 *a_arr , uint8 a_size)
{
	/*Change the IC number in array to hexa form*/
	uint32 IC_number_in_hexa=0;

	for(int i=0;i<a_size-1;i++)
	{
		IC_number_in_hexa|=a_arr[i];
		IC_number_in_hexa=IC_number_in_hexa<<4;
	}
	IC_number_in_hexa|=a_arr[a_size-1];

	return IC_number_in_hexa;
}


/****************************************************************
 * Mohammed hamdy
 * @param arr  : The array which I wants to adjust
 * @param size : The real size of the test case
 *
 * This function take the test case which has a certain size and
 * adjust it to 16 bit size, becase all test cases in the memory
 * is 16 bit and I didn't save the pins number in memory, so all
 * pins are 16 pins
 ***************************************************************/
void adjust_testcase_to_16_bit(uint8 *a_arr, uint8 a_size)
{
	uint8 half_size = a_size/2; /*half of the size*/
	uint8 temp_arr[MAX_PINS_FOR_IC] = {0}; /*temp array to the result in it and initalize it with zero*/


	uint8 j = 0;
	for(uint8 i=0; i<half_size ; i++,j++)/*The first half of the test case will be add in the first 8 bits in temp array*/
		temp_arr[j] = a_arr[i];



	uint8 counter = half_size;
	if( a_size%2 == 1 )  /*If the size is odd so half of size will be decimal and half, so I will increment the counter by 1*/
		counter ++;

	j = MAX_PINS_FOR_IC/2;
	for(uint8 i=0 ; i<counter ; i++,j++)/*The second half of the test case will be add in the last 8 bits in temp array*/
		temp_arr[j] = a_arr[i+half_size];


	for(uint8 i=0;i<MAX_PINS_FOR_IC;i++) /*Finally copy the temp array in origanl array */
	{
		a_arr[i] = temp_arr[i];
	}


}







