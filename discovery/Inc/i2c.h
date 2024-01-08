/*
 * i2c.h
 *
 *  Created on: 20 Dec 2023
 *      Author: laisf
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx.h"

void i2c1_init(void);
void i2c_read(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t reg_address, int nBytes, uint8_t* buffer);
void i2c_write(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t reg_address, int nBytes, uint8_t* data);

#endif /* I2C_H_ */
