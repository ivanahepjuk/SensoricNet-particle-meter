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
#include "functions.h"

extern char ID[];

void eeprom_write_id(char *dev_id)
{
	uint8_t cmd_w[11] = {0};  //10 pismen -> 10x hexadecimal znaku, plus 0 na zacatku jako adresa v eeprom pameti, cele pole se pak nasype jako command do epromky
	uint8_t data[2] = {0};

	//nbiot-0001       6e 62 69 6f 74 2d 30 30 30 31 00
	
	for (int i=0; i<11; i++){
		cmd_w[i+1] = dev_id[i];
	}

	//i2c write
	i2c_transfer7(I2C2, EEPROM, cmd_w, 11, data, 0);
}

//It reads data from eeporom and stores it into global array
void eeprom_read_id(void)
{

	uint8_t cmd_w[2] = {0x00, 0x00};
	uint8_t data[10] = {0};
		
	i2c_transfer7(I2C2, EEPROM, cmd_w, 1, data, 10);
	
	for(int i=0; i<10; i++)
		ID[i] = data[i];
	ID[10] = NULL;
	
	return data;
}



	//i2c_transfer7(I2C2, EEPROM, cmd_w, 1, data, 20);

//}



