#include "helpers.h"

/* Call Back Functions */
volatile void ledony()
{
	//LCD_intgerToString(000);
}

extern uint8 password1[PASS_SIZE];
extern uint8 password2[PASS_SIZE];
extern uint8 passwordCheck[PASS_SIZE];

uint8 check_identical(uint8* pass1, uint8* pass2)
{
	int pass_index;
	for(pass_index = 0 ; pass_index < PASS_SIZE ; pass_index++)
	{
		if(pass1[pass_index]!=pass2[pass_index])
		{
			return NO;
		}
	}

	return YES;
}


void enter_password(void)
{
	LCD_clearScreen();
	LCD_displayString("Please Enter Password:");
	LCD_moveCursor(1,5);
	uint8 key = 0;
	uint8 password_index = 0;
	while(password_index != PASS_SIZE)
	{
		key = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		password1[password_index] = key;
		password_index++;
		_delay_ms(500); /* Press time */
	}
}

uint8 reenter_password(void)
{
	LCD_clearScreen();
	LCD_displayString("Please Reenter Password:");
	LCD_moveCursor(1,5);
	uint8 key = 0;
	uint8 password_index = 0;

	while(password_index != PASS_SIZE)
	{
		key = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		password2[password_index] = key;
		password_index++;
		_delay_ms(500); /* Press time */
	}

	uint8 identical = check_identical(password1, password2);

	if(identical)
	{
		UART_sendByte('!');
		uint8 key = UART_recieveByte();
		if(key == '!')
		UART_sendString(password1);
	}

	return identical;
}

void password_not_identical(void)
{
	LCD_clearScreen();
	LCD_displayString("inputs are not identical");
	LCD_moveCursor(1,0);
	LCD_displayString("Please Enter same inputs twice.");
	_delay_ms(3000);

}

Option main_options(void)
{
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Password");

	uint8 key = 0;
	while(1)
	{
		key = KEYPAD_getPressedKey();

		if( key == '+' )
		{
			 _delay_ms(500); /* Press time */
			return OPENDOOR;
			break;
		}
		if( key == '-' )
		{
			 _delay_ms(500); /* Press time */
			return CHANGEPW;
			break;
		}
	}

}

uint8 enter_saved_password(void)
{
	LCD_clearScreen();
	LCD_displayString("Please Enter Password:");
	LCD_moveCursor(1,5);
	uint8 key = 0;
	uint8 password_index = 0;
	while(password_index != PASS_SIZE)
	{
		key = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		passwordCheck[password_index] = key;
		password_index++;
		_delay_ms(500); /* Press time */
	}

	UART_sendByte('+');
	key = UART_recieveByte();
	if(key == '!')
	UART_sendString(passwordCheck);


	UART_sendByte('+');

	key = UART_recieveByte();

	if(key == '+')
	{
		LCD_displayCharacter('V');
		return 1;
	}

	return 0;
}


void open_door(void)
{
	LCD_clearScreen();
	LCD_displayString("Opening Door");
	_delay_ms(2000); /* Press time */

}
uint8 ask_to_close(void)
{
	LCD_clearScreen();
	LCD_displayString("ASK Door");
	_delay_ms(2000); /* Press time */


	return 1;
}
void close_door(void)
{
	LCD_clearScreen();
	LCD_displayString("Closing Door");
	_delay_ms(2000); /* Press time */

}