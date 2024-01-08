/*
 * uart.h
 *
 *  Created on: 8 Apr 2023
 *      Author: laisf
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

typedef void (*uart_callback_t)(uint32_t data);

void uart2_rxtx_init(void);
void uart2_rxtx_interrupt_init(void);
void uart2_register_callback(uart_callback_t callback_function);
void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);
void uart2_write(int character);
char uart2_read(void);

#endif /* UART_H_ */
