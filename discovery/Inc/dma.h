/*
 * dma.h
 *
 *  Created on: 20 Dec 2023
 *      Author: laisf
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stddef.h>

typedef void (*dma_callback_t)(void);

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);
void dma_register_callback(dma_callback_t callback_function);

#endif /* DMA_H_ */
