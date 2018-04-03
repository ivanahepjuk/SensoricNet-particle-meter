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

#include <libopencm3/stm32/i2c.h>
#include "m24c04.h"
#include "functions.h"			 //smazat	


int eeprom_write_id(uint8_t *dev_id)
{

	char id[30] = {0};
	char readed[30]={0};
	
	int j = 0;
	char znak[3];
	
	//nbiot-0001
	while(j<10){
		charToHex(dev_id[j], znak);
		strcat(id, znak);
		j++;
	}
	
	//i2c write
	i2c_transfer7(I2C2, EEPROM, dev_id, 20, (uint8_t)readed, 0);
	//usartSend("de\r\n", 2);
	return 1;
}


void eeprom_read_id(void)
{

	char id[30] = {0};
	char readed[30]={0};
		
	i2c_transfer7(I2C2, EEPROM, (uint8_t)id, 0, (uint8_t)readed, 20);

	//i2c read
	usartSend(readed, 2);
	
	return 1;
}


