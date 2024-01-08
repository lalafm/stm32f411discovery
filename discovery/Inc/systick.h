/*
 * systick.h
 *
 *  Created on: 18 Apr 2023
 *      Author: laisf
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32f4xx.h"
#include <stddef.h>

typedef void (*sysTick_callback_t)(void);

void systickDelay_ms(int delay);
void systick_1Hz_interrupt(void);
void sysTick_register_callback(sysTick_callback_t callback_function);

#endif /* SYSTICK_H_ */
