/*
 * adc.h
 *
 *  Created on: 18 Apr 2023
 *      Author: laisf
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"

typedef void (*adc_callback_t)(uint32_t sensor_value);

void pa1_adc_init(void);
void pa1_adc_interrupt_init(void);
void adc_register_callback(adc_callback_t callback_function);
void start_conversion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
