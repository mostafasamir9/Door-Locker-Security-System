#include "timer.h"
#include "avr/io.h"

volatile void ledony()
{
	DDRC = 0xFF;
	PORTC = ~PORTC;
}

int main()
{
	TIMER_Config config;

	SREG |= (1<<7);
	config.timer_id = TIMER1_ID;
	config.timer_mode = OVERFLOW_MODE;
	config.timer_clk = PRESCALER256;
	config.oc_mode = DISCONNECTED;
	config.interrupt = 1;
	config.inital_value = 0;
	config.compare_value = 155;

	Timer_Init(&config);

	 volatile void (*ptr)(void);
	 ptr = ledony;

	Timer_SetCallBack(TIMER1_ID,ptr, OVERFLOW_MODE);
	while(1)
	{

	}
}
