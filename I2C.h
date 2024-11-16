/* 
 * File:   I2C.h
 * Author: lievc
 *
 * Created on March 13, 2024, 5:28 PM
 */

#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

#define CLK_PER   4000000
#define TWI0_BAUD(F_SCL, T_RISE)  ((((((float)(CLK_PER) / (float)(F_SCL))) - 10 - ((float)(CLK_PER) * (T_RISE) / 1000000))) / 2)
#define I2C_SCL_FREQ  100000

void init_I2C(void);

void I2C_start_transfer(uint8_t addr, uint8_t dir);

uint8_t I2C_send_byte(uint8_t data, uint8_t end_trans);

uint8_t I2C_read_byte(uint8_t end_trans);

#endif