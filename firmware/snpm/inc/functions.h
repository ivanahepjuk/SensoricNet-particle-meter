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
#include <stdio.h>
//#include <stdint.h>
	

#define SEC		(1000000)

extern char gps_rx_buffer[255];
extern uint8_t gps_rx_buffer_pointer;
extern char gps_utc_time[10];
extern char gps_latitude[9];
extern char gps_latitude_ns[1];
extern char gps_longitude[10];
extern char gps_longitude_ew[1];
extern char gps_quality_indicator[1];
extern char gps_altitude[8];

//eeprom
int eeprom_write(char *dev_id);

//pomocne
void charToHex(char c, char hex[3]);
char hexDigit(unsigned n);

//posle string na usart c. 1,2,3,4
void usartSend(char *phrase, uint8_t usart);
void debug_usart_send(char *phrase);
void clock_setup(void);
void i2c_setup(void);
void usart_setup(void);
void gpio_setup(void);
void spi_setup(void);

float calculate_float(uint8_t val0, uint8_t val1, uint8_t val2, uint8_t val3);

uint8_t spi_reaad8(uint32_t spi);
void wait(uint32_t usec);

void flash(uint8_t loop, uint32_t delay);
void led_flash(uint8_t led, uint8_t loop, uint32_t delay);
void led_on(uint8_t led);
void led_off(uint8_t led);

float convert_gps_to_wgs84_latitude (char *gps_latitude_string);
float convert_gps_to_wgs84_longitude (char *gps_longitude_string);
void get_nth_substring(unsigned int number, char separator, char* string, unsigned int string_size, char* buffer, unsigned int buffer_size);
//void usart4_isr(void);

void sys_tick_handler(void);
void systick_setup(int xms);



////////////////dronovo
//char* concat(const char *s1, const char *s2);
//char* string_to_hex(unsigned char *string, int len);

#endif
