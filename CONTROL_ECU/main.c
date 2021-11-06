#include "helpers.h"

uint8 g_pass[PASS_SIZE];

uint8 EEPROM_pass[PASS_SIZE];
uint8 EE[PASS_SIZE];

uint8 key;

int main() {
	LCD_init();
	LCD_displayString("meme");

	/* Buzzer Initialization */
	Buzzer_init(PORTB_ID, PIN0_ID);

	/* Motor Initialization */
	DcMotor_Init(PORTB_ID, PIN5_ID, PIN4_ID);

	/* EEPROM Init */
	EEPROM_init();

	/* Uart Initialization */
	UART_Config uconfig;

	uconfig.RxEn = ON;
	uconfig.TxEn = ON;
	uconfig.EMPInt = OFF;
	uconfig.RxInt = OFF;
	uconfig.TxInt = OFF;
	uconfig.doublespeed = ON;
	uconfig.parity = EVEN;
	uconfig.stopbits = ONE;
	uconfig.databits = EIGHT;
	uconfig.baudrate = 9600;

	UART_init(&uconfig);

	LCD_displayString("meme");

	//Buzzer_ON();
	//DcMotor_Rotate(CW,10);
	State state = IDLE;

	while (1) {
		switch (state) {
		case IDLE:
			key = UART_recieveByte();
			LCD_displayString("ok");
			if (key == '!') {
				state = SAVE_PASSWORD;
				LCD_displayString("go to save");
			}
			break;
		case SAVE_PASSWORD:
			UART_sendByte('!');
			UART_receiveString(g_pass);
			LCD_displayString(g_pass);
			EEPROM_writeString(g_pass, PASS_SIZE);
			LCD_displayString("written");
			state = OPTIONS;
			break;

		case OPTIONS:
			key = UART_recieveByte();

			switch (key) {
			case '+':
				state = CHECK_PASSWORD;
				break;
			case '-':
				key = UART_recieveByte();
				LCD_displayString("ok");
				if (key == '!') {
					state = SAVE_PASSWORD;
					LCD_displayString("go to save");
				}
				state = SAVE_PASSWORD;

				break;
			case '0':
				state = THIEF;
				break;
			}
			break;

		case CHECK_PASSWORD:
			LCD_displayCharacter(key);

			UART_sendByte('!');
			UART_receiveString(g_pass);
			EEPROM_readString(EEPROM_pass, PASS_SIZE);
			LCD_moveCursor(1, 0);

			if (check_identical(EE, g_pass)) {
				LCD_displayString("identical");

				state = OPEN_DOOR;
			} else {
				key = UART_recieveByte();
				if (key == '+') {
					UART_sendByte('X');
				}
				LCD_displayString("not");
				state = OPTIONS;
			}
			break;
		case OPEN_DOOR:
			key = UART_recieveByte();
			if (key == '+')
				UART_sendByte('+');
			DcMotor_Rotate(CW, 100);
			state = CLOSE_DOOR;
			break;
		case CLOSE_DOOR:
			key = UART_recieveByte();
			if (key == '+')
				DcMotor_Rotate(A_CW, 100);
			state = OPTIONS;
			break;
		case THIEF:
			LCD_displayString("ttt");
			Buzzer_ON();
			_delay_ms(100);
			Buzzer_OFF();
			_delay_ms(100);
			break;

		}

	}
}
