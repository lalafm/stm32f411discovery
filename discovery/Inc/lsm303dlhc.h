/*
 * accel.h
 *
 *  Created on: 29 Dec 2023
 *      Author: laisf
 */

#ifndef LSM303DLHC_H_
#define LSM303DLHC_H_

#include <stdint.h>

#define DEVICE_ADDR  (0x19)
#define CTRL_REG1_A  (0x20)
#define CTRL_REG4_A	 (0x23)
#define OUT_X_L_A    (0x28)

void lsm303_init(void);
void lsm303_read(void);

#endif /* LSM303DLHC_H_ */
