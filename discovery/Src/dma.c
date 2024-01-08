/*
 * dma.c
 *
 *  Created on: 20 Dec 2023
 *      Author: laisf
 */
#include "dma.h"

static dma_callback_t callback = NULL;

void DMA1_Stream6_IRQHandler(void)
{
	if(DMA1->HISR & DMA_HISR_TCIF6)
	{
		DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
		if(callback != NULL)
		{
			callback();
		}
	}
}

void dma_register_callback(dma_callback_t callback_function)
{
	callback = callback_function;
}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len)
{
	/* Enabe clock access to DMA */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	/* Disable DMA1 */
	DMA1_Stream6->CR &= ~DMA_SxCR_EN;
	/* Wait until DMA1 is disabled */
	while(DMA1_Stream6->CR & DMA_SxCR_EN){}
	/* Clear all interrupt flags of Stream 6 */
	DMA1->HIFCR |= (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);
	/* Set the destination buffer */
	DMA1_Stream6->PAR = dst;
	/* Set the source buffer */
	DMA1_Stream6->M0AR = src;
	/* Set length */
	DMA1_Stream6->NDTR = len;
	/* Select Stream 6 CH4 */
	DMA1_Stream6->CR |= DMA_SxCR_CHSEL_2;
	DMA1_Stream6->CR &= ~(DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_0);
	/* Enable memory increment */
	DMA1_Stream6->CR |= DMA_SxCR_MINC;
	/* Configure transfer direction: Memory to peripheral */
	DMA1_Stream6->CR |= DMA_SxCR_DIR_0;
	DMA1_Stream6->CR &= ~DMA_SxCR_DIR_1;
	/* Enable DMA transfer complete interrupt */
	DMA1_Stream6->CR |= DMA_SxCR_TCIE;
	/* Enable direct mode and disable FIFO */
	DMA1_Stream6->FCR &= ~DMA_SxFCR_DMDIS;
	/* Enable DMA Stream 6 */
	DMA1_Stream6->CR |= DMA_SxCR_EN;
	/* Enable UART2 transmitter DMA */
	USART2->CR3 |= USART_CR3_DMAT;
	/* Enable DMA interrupt */
	__NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}
