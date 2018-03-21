
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

#ifndef WIRELESS_H
#define WIRELESS_H

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 			LORWAN WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
 

void connect_lorawan(void);
//void lora_sendCommand(char *phrase);
//int sendCommand(char *phrase, char *check, int pocetentru);

int nbiot_sendCommand(char *phrase, char *check, int pocetentru);
int lora_sendCommand(char *phrase, char *check, int pocetentru);

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 		NBIOT QUECTEL WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void connect_nbiot(void);

#endif
