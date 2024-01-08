/*
 * led.h
 *
 *  Created on: 20 Dec 2023
 *      Author: laisf
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f4xx.h"

void green_led_init(void);
void green_led_toggle(void);
void green_led_set(void);
void green_led_clear(void);

#endif /* LED_H_ */
