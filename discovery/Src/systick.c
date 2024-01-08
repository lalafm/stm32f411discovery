/*
 * systic.c
 *
 *  Created on: 18 Apr 2023
 *      Author: laisf
 */
#include "systick.h"

#define SYSTICK_LOAD_VALUE (16000-1)

static sysTick_callback_t callback = NULL;

void sysTick_register_callback(sysTick_callback_t callback_function)
{
	callback = callback_function;
}

void SysTick_Handler(void)
{
	callback();
}

void systickDelay_ms(int delay)
{
	/* Configure SysTick */
	// Reload with number of clocks per millisecond
	SysTick->LOAD = SYSTICK_LOAD_VALUE;
	//Clear SysTick current value register
	SysTick->VAL = 0;
	//Enable SysTick and select internal clk src
	SysTick->CTRL |= ( SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);

	for(int i = 0; i < delay; i++)
	{
		// Wait until the COUNTFLAG is set
		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		{

		}
	}
	SysTick->CTRL = 0;
}

void systick_1Hz_interrupt(void)
{
	// Reload with number of clocks per sec
	SysTick->LOAD = 16000000-1;
	// Enable SysTick and select internal clk src
	SysTick->CTRL |= ( SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
	// Enable SysTick exception
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}
