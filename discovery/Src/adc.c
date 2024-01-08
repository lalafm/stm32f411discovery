/*
 * adc.c
 *
 *  Created on: 18 Apr 2023
 *      Author: laisf
 */
#include "adc.h"

void pa1_adc_init(void);
void pa1_adc_interrupt_init(void);
void adc_register_callback(adc_callback_t callback_function);
void start_conversion(void);
uint32_t adc_read(void);

static adc_callback_t callback = NULL;

void adc_register_callback(adc_callback_t callback_function)
{
	callback = callback_function;
}

void ADC_IRQHandler(void)
{
	/* Check for EOC flag */
	if(ADC1->SR & ADC_SR_EOC)
	{
		ADC1->SR &= ~ADC_SR_EOC;
		if(callback != NULL)
		{
			callback(ADC1->DR);
		}
	}
}

/* ADC configured with 3 channels
 * ch2, ch3 and ch5
 * first = ch5
 * second = ch2
 * third = ch3
 */
void pa1_adc_init(void)
{
	/* Configure ADC GPIO pin */
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Set the GPIOmode register to analog mode
	GPIOA->MODER |= GPIO_MODER_MODER1_0;
	GPIOA->MODER |= GPIO_MODER_MODER1_1;

	/* Configure ADC module */
	// Enable clock access to ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	/* Configure ADC parameters */
	// Conversion sequence start
	ADC1->SQR3 = 1U; // put channel 1 in

	// Conversion sequence length
	ADC1->SQR1 = 0U;
	// Enable continuous conversion
	ADC1->CR2 |= ADC_CR2_CONT;
	// Enable ADC module
	ADC1->CR2 |= ADC_CR2_ADON;
}

void pa1_adc_interrupt_init(void)
{
	/* Configure ADC GPIO pin */
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Set the GPIOmode register to analog mode
	GPIOA->MODER |= GPIO_MODER_MODER1_0;
	GPIOA->MODER |= GPIO_MODER_MODER1_1;

	/* Configure ADC module */
	// Enable clock access to ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// Enable ADC end of conversion interrupt
	ADC1->CR1 |= ADC_CR1_EOCIE;

	/* Configure ADC parameters */
	// Conversion sequence start
	ADC1->SQR3 = 1U; // put channel 1 in

	// Conversion sequence length
	ADC1->SQR1 = 0U;
	// Enable continuous conversion
	ADC1->CR2 |= ADC_CR2_CONT;
	// Enable ADC module
	ADC1->CR2 |= ADC_CR2_ADON;

	// Enable ADC interrupt in NVIC
	__NVIC_EnableIRQ(ADC_IRQn);
}


void start_conversion(void)
{
	// Start ADC conversion
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t adc_read(void)
{
	// Wait for conversion to be complete
	while(!(ADC1->SR & ADC_SR_EOC))
	{

	}
	// Read converted result
	return (ADC1->DR);
}
