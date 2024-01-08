/*
 * exti.c
 *
 *  Created on: 22 Nov 2023
 *      Author: laisf
 */
#include "exti.h"

void pa0_exti_init(void)
{
	/* Good practise: disable global interrupts */
	__disable_irq();

	/* Enable GPIO clock access */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* Set PA0 as input: 00*/
	GPIOA->MODER &= ~(GPIO_MODER_MODER0);

	/* Enable clock access to SYSCFG */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/* Select PORTA to EXTI0 set to 0000*/
	SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0);

	/* Unsmask EXTI0 */
	EXTI->IMR |= EXTI_IMR_MR0;

	/* Select falling edge trigger */
	EXTI->FTSR |= EXTI_FTSR_TR0;

	/* Enable EXTI0 line in NVIC */
	__NVIC_EnableIRQ(EXTI0_IRQn);

	/* Enable global interrupts */
	__enable_irq();
}
