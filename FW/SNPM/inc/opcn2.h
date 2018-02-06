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

#ifndef BME280_H
#define BME_280_H
 

#include <stdint.h> 
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "functions.h"

#define DELAY				0.01
#define DELAY_1				2
#define FAN_SPEED			70




extern uint8_t histogram_buffer[62];
extern uint8_t pm_values_buffer[12];


uint8_t pm_set_command(uint8_t command_byte, uint32_t delay);

void particlemeter_set_fan(uint8_t speed);
//read_serial_number
//read_config_values
//read_config_values_2
void read_pm_values(void);
void read_histogram_all(void);
void particlemeter_set_laser(uint8_t laser);
float particlemeter_pm1(void);
float particlemeter_pm2_5(void);
float particlemeter_pm10(void);

void particlemeter_ON(void);
void particlemeter_read(void);

void pm_SS_on(void);
void pm_SS_off(void);
void pm_SS_toggle(uint32_t delay);

#endif
