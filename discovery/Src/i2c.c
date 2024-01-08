/*
 * i2c.c
 *
 *  Created on: 20 Dec 2023
 *      Author: laisf
 */

#include "i2c.h"


void i2c1_init(void)
{
	/* Enable clock access to I2C1 Must be done before doing the AFR GPIO */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	/* SCL - PB6, SDA - PB9
	/* Enable clock access to I2C GPIO */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	/* Set PB6 and PB9 to alternate function (10) */
	GPIOB->MODER &= ~GPIO_MODER_MODER6_0;
	GPIOB->MODER |= GPIO_MODER_MODER6_1;

	GPIOB->MODER &= ~GPIO_MODER_MODER9_0;
	GPIOB->MODER |= GPIO_MODER_MODER9_1;
	/* Set PB6 and PB9 alternate function to I2C(AF04) - 0100 */
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_3 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_0);

	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL9_3 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_0);
	/* Set PB6 and PB9 output type to open drain */
	GPIOB->OTYPER |= GPIO_OTYPER_OT6;
	GPIOB->OTYPER |= GPIO_OTYPER_OT9;
	/* Enable pull-up for PB6 and PB9 (01)*/
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6_1;

	GPIOB->PUPDR |= GPIO_PUPDR_PUPD9_0;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD9_1;

	/* Enter I2C reset mode */
	I2C1->CR1 |= I2C_CR1_SWRST;
	/* Leave I2C reset mode */
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	/* Set peripheral APB1 clock frequency: 16 MHz */
	I2C1->CR2 |= I2C_CR2_FREQ_4;
	/* Set SCL freq. APB1 Clock = 16MHz = 62.5ns, to have a 100kHz freq in sm mode-> CCR = T/(2 * Tapb1) = 80 */
	I2C1->CCR = 80;
	/* Set maximum rise time. Trise = Max allowed clock in sm mode / Tapb1 + 1 = 1000ns / 62.5ns + 1 = 17 */
	I2C1->TRISE = 17;
	/* Enable I2C */
	I2C1->CR1 |= I2C_CR1_PE;
}

/* Start I2C communication */
void i2c_start(I2C_TypeDef *I2Cx)
{
	/* Generate start */
	I2Cx->CR1 |= I2C_CR1_START;
	/* Wait for start, bit SB to be set */
	while(!(I2Cx->SR1 & I2C_SR1_SB)){ }
}

void i2c_stop(I2C_TypeDef *I2Cx)
{
	/* Generate stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
}

void i2c_check_busy(I2C_TypeDef *I2Cx)
{
	/* Check device is not busy, bit BUSY is clear */
	while(I2C1->SR2 & I2C_SR2_BUSY){ }
}

void i2c_send_address(I2C_TypeDef *I2Cx, uint8_t address)
{
	/* Transmit slave address + write/read 0/1, bit 0 is reserved for W/R when sending the slave address */
	I2C1->DR = address;
	/* Wait until addr flag is set */
	while(!(I2C1->SR1 & I2C_SR1_ADDR)) {}
}

void i2c_clear_address_flag(I2C_TypeDef *I2Cx)
{
	/* Clear addr flag (read SR2) */
	volatile uint32_t temp = I2C1->SR2;
}

void i2c_write_byte(I2C_TypeDef *I2Cx, uint8_t data)
{
	/* Wait until TXE flag is set, indicating that DR is empty */
	while(!(I2C1->SR1 & I2C_SR1_TXE)) {}
	/* Send data to DR register */
	I2Cx->DR = data;
	/* Wait until BTF bit is set, indicating byte was transfered */
	while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void i2c_read_byte(I2C_TypeDef *I2Cx, uint8_t* buffer)
{
	/* Wait until RXNE flag is set */
	while(!(I2Cx->SR1 & I2C_SR1_RXNE)) {}
	/* Read data from DR */
	*buffer = I2C1->DR;
}

void i2c_enable_ack(I2C_TypeDef *I2Cx, bool shouldEnable)
{
	if(shouldEnable)
	{
		/* Enable Acknowledge */
		I2Cx->CR1 |= I2C_CR1_ACK;
	}
	else
	{
		/* Disable Acknowledge */
		I2Cx->CR1 &= ~I2C_CR1_ACK;
	}
}

void i2c_read(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t reg_address, int nBytes, uint8_t* buffer)
{
	/* Check device is not busy, bit BUSY is clear */
	i2c_check_busy(I2Cx);
	i2c_start(I2Cx);
	/* Transmit slave address + write 0, bit 0 is reserved for W/R when sending the slave address */
	i2c_send_address(I2Cx, (slave_address << 1));
	i2c_clear_address_flag(I2Cx);
	i2c_write_byte(I2Cx, reg_address);
	/* Generate restart */
	i2c_start(I2Cx);
	/* Transmit slave address + read 1, bit 0 is reserved for W/R when sending the slave address */
	i2c_send_address(I2Cx, (slave_address << 1 | 1));

	if(nBytes == 1)
	{
		/* Disable Acknowledge */
		i2c_enable_ack(I2Cx, false);
		i2c_clear_address_flag(I2Cx);
		i2c_stop(I2Cx);
		i2c_read_byte(I2Cx, buffer);
		buffer++;
	}
	else
	{
		i2c_clear_address_flag(I2Cx);
		/* Enable Acknowledge */
		i2c_enable_ack(I2Cx, true);
		int remainingBytes = nBytes;
		while(remainingBytes > 0)
		{
			if(remainingBytes == 1)
			{
				/* Disable Acknowledge */
				i2c_enable_ack(I2Cx, false);
				i2c_stop(I2Cx);
				i2c_read_byte(I2Cx, buffer);
				buffer++;
				break;
			}
			else
			{
				i2c_read_byte(I2Cx, buffer);
				buffer++;
			}
			remainingBytes--;
		}
	}
}

void i2c_write(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t reg_address, int nBytes, uint8_t* data)
{
	/* Check device is not busy */
	i2c_check_busy(I2Cx);
	i2c_start(I2Cx);
	/* Transmit slave address + write 0, bit 0 is reserved for W/R when sending the slave address */
	i2c_send_address(I2Cx, (slave_address << 1));
	i2c_clear_address_flag(I2Cx);
	i2c_write_byte(I2Cx, reg_address);

	for(int i = 0; i < nBytes; i++)
	{
		i2c_write_byte(I2Cx, *data);
		data++;
	}
	i2c_stop(I2Cx);
}
