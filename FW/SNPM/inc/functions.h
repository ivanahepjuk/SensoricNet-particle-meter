/*
 * This file is part of the SenoricNet project, https://sensoricnet.cz
 *
 * Copyright (C) 2017 Pavel Polach <ivanahepjuk@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <string.h>
//#include <stdint.h>
	
#define EEPROM 0x51
#define RCC_CFGR3_I2C2SW   (1 << 5)

//pomocne
void charToHex(char c, char hex[3]);
char hexDigit(unsigned n);

//posle string na usart c. 1,2,3,4
void usartSend(char *phrase, int usart);
void wait(float sec);
void clock_setup(void);
void i2c_setup(void);
void usart_setup(void);
void gpio_setup(void);
void connect_nbiot(void);
void connect_lorawan(void);
int sendCommand(char *phrase, char *check, int pocetentru);


void i2c_transfer77(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn, uint8_t *r, size_t rn);
void compensation_data_readout_BME280(uint8_t arrayy[]);



float calculate_float(uint8_t val0, uint8_t val1, uint8_t val2, uint8_t val3);



#endif
