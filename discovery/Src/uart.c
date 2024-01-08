#include "uart.h"
#include <stddef.h>

#define SYS_FREQ 		16000000
#define APB1_CLK 		SYS_FREQ
#define UART_BAUDRATE 	57600


/* Function prototypes */
static void uart_set_baudrate(USART_TypeDef  *USARTx, uint32_t peripheralClock, uint32_t baudRate);
void uart2_rxtx_init(void);
void uart2_register_callback(uart_callback_t callback_function);
void uart2_write(int character);
char uart2_read(void);

/* Variables */
static uart_callback_t callback = NULL;

int __io_putchar(int character)
{
	uart2_write(character);
	return character;
}

void uart2_register_callback(uart_callback_t callback_function)
{
	callback = callback_function;
}

/* Name in vector table in startup file */
void USART2_IRQHandler(void)
{
	// Check if RXNE is set
	if(USART2->SR && USART_SR_RXNE && (callback != NULL))
	{
		callback(USART2->DR);
	}
}

void uart2_rxtx_init(void)
{
	/**** Configure USART GPIO pin ****/
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Set PA2 and PA3 to alternate function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODER2_0;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;

	GPIOA->MODER &= ~GPIO_MODER_MODER3_0;
	GPIOA->MODER |= GPIO_MODER_MODER3_1;
	// Set PA2 and PA3 alternate function to UART_TX(AF07) - 0111
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;

	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
	/**** Configure USART module ****/
	// Enable clock access to UART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// Configure baud rate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	// Configure the transfer direction
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
	// Enable UART module
	USART2->CR1 |= USART_CR1_UE;
}

void uart2_rxtx_interrupt_init(void)
{
	/**** Configure USART GPIO pin ****/
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Set PA2 and PA3 to alternate function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODER2_0;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;

	GPIOA->MODER &= ~GPIO_MODER_MODER3_0;
	GPIOA->MODER |= GPIO_MODER_MODER3_1;
	// Set PA2 alternate function to UART_TX(AF07) - 0111
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;

	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
	/**** Configure USART module ****/
	// Enable clock access to UART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// Configure baud rate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	// Configure the transfer direction
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;

	// Enable rx interrupt
	USART2->CR1 |= USART_CR1_RXNEIE;
	// Enable UART interrupt in NVIC
	__NVIC_EnableIRQ(USART2_IRQn);

	// Enable UART module
	USART2->CR1 |= USART_CR1_UE;
}

void uart2_write(int character)
{
	// Make sure the transmit data register is empty
	while( !(USART2->SR & USART_SR_TXE)){}
	// Write to transmit data register
	USART2->DR = (character & 0xFF);
}

char uart2_read(void)
{
	// Make sure the receive data register is not empty
	while (!(USART2->SR & USART_SR_RXNE))
	{}

	return USART2->DR;
}

static void uart_set_baudrate(USART_TypeDef  *USARTx, uint32_t peripheralClock, uint32_t baudRate)
{
	uint16_t USARTDIV_MANT;
	uint16_t USARTDIV_FRAC;
	// From reference manual, when OVER8 = 0
	double usartDiv = (double)peripheralClock / (16.0 * (double)baudRate);
	USARTDIV_MANT = usartDiv;
	double fractional = 16.0*(usartDiv - (long)usartDiv);
	USARTDIV_FRAC = (uint16_t)(fractional + 0.5);
	if(USARTDIV_FRAC == 16)
	{
		USARTDIV_FRAC = 0;
		USARTDIV_MANT++;
	}
	USARTx->BRR = (USARTDIV_MANT << 4) | (USARTDIV_FRAC);
}

