/*
 * tim.c
 *
 *  Created on: 16 Nov 2023
 *      Author: laisf
 */
#include "tim.h"

static tim2_callback_t tim2_callback = NULL;

void tim2_register_callback(tim2_callback_t callback_function)
{
	tim2_callback = callback_function;
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		if(tim2_callback != NULL)
		{
			tim2_callback();
		}
	}
}

void tim2_1hz_init(void)
{
	/* Enable clock access to TIMER2 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	/* Set Prescaler value */
	TIM2->PSC = (10000 - 1);
	/* Set Auto-reload value */
	TIM2->ARR = (1600 - 1);
	/* Clear counter */
	TIM2->CNT = 0;
	/* Enable timer */
	TIM2->CR1 |= TIM_CR1_CEN;
}

void tim2_1hz_interrupt(void)
{
	/* Enable clock access to TIMER2 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	/* Set Prescaler value */
	TIM2->PSC = (10000 - 1);
	/* Set Auto-reload value */
	TIM2->ARR = (1600 - 1);
	/* Clear counter */
	TIM2->CNT = 0;
	/* Enable timer interrupt */
	TIM2->DIER |= TIM_DIER_UIE;
	/* Enable timer interrupt in NVIC */
	__NVIC_EnableIRQ(TIM2_IRQn);
	/* Enable timer */
	TIM2->CR1 |= TIM_CR1_CEN;
}

void tim4_pd12_output_compare(void)
{
	/* Enable clock access to GPIOD */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	/* Set PD12 (green  LED) to alternate function */
	GPIOD->MODER &= ~GPIO_MODER_MODER12_0;
	GPIOD->MODER |= GPIO_MODER_MODER12_1;
	/* Set PD12 alternate function to TIM4_CH1 (AF02)*/
	GPIOD->AFR[1] |= GPIO_AFRH_AFSEL12_1;
	GPIOD->AFR[1] &= ~(GPIO_AFRH_AFSEL12_0 | GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL12_3);

	/* Enable clock access to TIMER4 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	/* Set Prescaler value */
	TIM4->PSC = (1600 - 1);
	/* Set Auto-reload value */
	TIM4->ARR = (10000 - 1);

	/* Set output compare toggle mode(011) Channel 1*/
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;
	/* Enable TIM4 CH1 - connected to green LED in PD12 */
	TIM4->CCER |= TIM_CCER_CC1E;

	/* Clear counter */
	TIM4->CNT = 0;
	/* Enable timer */
	TIM4->CR1 |= TIM_CR1_CEN;
}

/* Setup: connect wire from PD12 to PA6*/
void tim3_pa6_input_capture(void)
{
	/* Enable clock access to GPIOA */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	/* Set PA6 to alternate function */
	GPIOA->MODER &= ~GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	/* Set PA6 alternate function to TIM3_CH1 (AF02)*/
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL6_3);

	/* Enable clock access to TIMER3 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	/* Set Prescaler value */
	TIM3->PSC = (16000 - 1);
	/* Set CH1 to capture every edge CC1S = 01*/
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	/* Enable Capture CH1 ( CC1P bits default is rising edge) */
	TIM3->CCER |= TIM_CCER_CC1E;
	/* Enable timer */
	TIM3->CR1 |= TIM_CR1_CEN;
}
