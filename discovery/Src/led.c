/*
 * led.c
 *
 *  Created on: 20 Dec 2023
 *      Author: laisf
 */
#include "led.h"

void green_led_init(void)
{
	// Set PD12 as output
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->MODER &=  ~(GPIO_MODER_MODER12_1);
}

void green_led_toggle(void)
{
	GPIOD->ODR ^= GPIO_ODR_OD12;
}

void green_led_set(void)
{
	GPIOD->ODR |= GPIO_ODR_OD12;
}

void green_led_clear(void)
{
	GPIOD->ODR &= ~GPIO_ODR_OD12;
}



