/*
 * tim.h
 *
 *  Created on: 16 Nov 2023
 *      Author: laisf
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stddef.h>

typedef void (*tim2_callback_t)(void);

void tim2_1hz_init(void);
void tim2_1hz_interrupt(void);
void tim2_register_callback(tim2_callback_t callback_function);
void tim4_pd12_output_compare(void);
void tim3_pa6_input_capture(void);

#endif /* TIM_H_ */
