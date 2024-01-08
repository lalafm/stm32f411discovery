/*
 * accel.c
 *
 *  Created on: 29 Dec 2023
 *      Author: laisf
 */
#include "lsm303dlhc.h"
#include "i2c.h"

void lsm303_init(void)
{
	i2c1_init( );
}

char data_buffer[6];
int16_t x, y, z;
double xf, yf, zf;

void lsm303_read(void)
{
	uint8_t reg1_content = 0x47;
	uint8_t reg4_content = 0x00;
	i2c_write(I2C1, DEVICE_ADDR, CTRL_REG1_A, 1, &reg1_content);
	i2c_write(I2C1, DEVICE_ADDR, CTRL_REG4_A, 1, &reg4_content);
	i2c_read(I2C1, DEVICE_ADDR, (0x28 | (1<<7)), 6, &data_buffer[0]);
	x = (int16_t)((data_buffer[1] << 8) | data_buffer[0]);
	y = (int16_t)((data_buffer[3] << 8) | data_buffer[2]);
	z = (int16_t)((data_buffer[5] << 8) | data_buffer[4]);
	xf = (double)(x >> 4) * 0.001;
	yf = (double)(y >> 4) * 0.001;
	zf = (double)(z >> 4) * 0.001;
}
