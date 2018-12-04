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

#include "opcn2.h"
#include "functions.h"
#include <libopencm3/stm32/spi.h>

uint8_t pm_set_command(uint8_t command_byte, uint32_t delay)
{
	uint8_t incomming = 0;
	
	spi_send8(SPI1, command_byte);
	wait(50);
	incomming = spi_read8(SPI1);//(uint8_t)
	wait(delay);
	
	return incomming;
}

void pm_SS_on(void)
{
	gpio_clear(GPIOD, GPIO2); //SS Log 0
	wait(100000); //1s
}

void pm_SS_off(void)
{	wait(10000); //100ms
	gpio_set(GPIOD, GPIO2); //SS Log 1
	wait(50000); //100ms
}

void pm_SS_toggle(uint32_t delay)
{
	wait(delay); //0.1s
	gpio_set(GPIOD, GPIO2); //SS Log 0
	wait(delay); //0.1s
	gpio_clear(GPIOD, GPIO2); //SS Log 1
	wait(10000); //1s
}


void particlemeter_ON(void)
{
uint32_t dummy=0;
	
	pm_SS_on();

	while((pm_set_command(0x03, 20000)) != 0xF3)   //20000 je cca 35ms
	{
		dummy++;
	}
	iwdg_reset();
	while((pm_set_command(0x00, 3500000)) != 0x03) //3500000 je cca 6s?
	{
		iwdg_reset();
	}

	pm_SS_off();
}


void particlemeter_set_fan(uint8_t speed)
{
	pm_SS_on();
	
	wait(70000);
	
	while( (pm_set_command( 0x42, 14000)) != 0xF3){;}
	
	while( (pm_set_command( 0x00, 14000)) != 0x42){;}
	
	while( (pm_set_command(speed, 14000)) != 0x00){;}
	
	wait(70000);
	
	pm_SS_off();
	
}


void read_pm_values(void)
{
	
	pm_SS_on();
	//while(pm_set_command(0x32, 14000) == 0xf3)       {;}
	pm_set_command(0x32, 14000);
	//wait(14000);
	pm_SS_toggle(20000);
	wait(10000);
	
	for(uint8_t i = 0; i<12; i++)
	{
		spi_send8(SPI1, 0x32);
		wait(20000);
		//steals incomming data directly from shift register
		pm_values_buffer[i] = spi_read8(SPI1);		
	}
	
	/*
	//Debug
	for (int i=0; i<12; i++)
	{
		usart_send_blocking(USART4, pm_values_buffer[i]);			
	}
	usartSend("\r\n", 4);	
	*/
	
	pm_SS_off();
	
	/*
	pm_SS_on();
	
	while( pm_set_command(0x32, 140000) != 0xF3){;}// ok tady ten loop!!
	
		for(uint8_t i = 0; i<12; i++)
	{
		spi_send8(SPI1, 0x32);
		wait(2000);
		//steals incomming data directly from shift register
		pm_values_buffer[i] = spi_read8(SPI1);		
	}
	
	pm_SS_off();
	*/
}

//read_pm_data
float particlemeter_pm1(void)
{
	float pm1 = calculate_float(pm_values_buffer[0], pm_values_buffer[1] , pm_values_buffer[2], pm_values_buffer[3]);
	
	return pm1;
}

float particlemeter_pm2_5(void)
{
	float pm2_5 = calculate_float(pm_values_buffer[4], pm_values_buffer[5] , pm_values_buffer[6], pm_values_buffer[7]);
	
	return pm2_5;
}

float particlemeter_pm10(void)
{
	float pm10 = calculate_float(pm_values_buffer[8], pm_values_buffer[9] , pm_values_buffer[10], pm_values_buffer[11]);
	
	return pm10;
}

void particlemeter_set_laser(uint8_t laser)
{
	wait(DELAY);
	gpio_clear(GPIOD, GPIO2); //SS Log 0
	
	wait(DELAY_1);
	  
	spi_send(SPI1, 0x42);
	wait(DELAY);
	spi_send(SPI1, 0x01);//laser command
	wait(DELAY);
	spi_send(SPI1, laser);
	wait(DELAY);
	gpio_set(GPIOA, GPIO8); //SS Log 1
}



void particlemeter_read(void)
{
	//read registers
;	
}


void particlemeter_power_cycle(void)
{

	//PM reset
	gpio_clear(GPIOA,GPIO10);
	wait(SEC*5);
	gpio_set(GPIOA, GPIO10);
};
