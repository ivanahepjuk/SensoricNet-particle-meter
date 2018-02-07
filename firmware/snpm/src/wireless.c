
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

#include "rn2483.h"
#include "functions.h"


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 			LORWAN WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void connect_lorawan(void)
{
//mac set deveui 0004A30B00222137		
//mac set appeui 70B3D57ED00082D2
//mac set appkey D94AC6F27881D3505F3E595B69472898
	//usartSend("sys get ver\r\n", 4);
    //wait(1);
	usartSend("radio set pwr 14   \r   \n", 4);
	wait(SEC*3);
	usartSend("mac set deveui 0004A30B00222137\r\n", 4);
	wait(SEC*2);
	usartSend("mac set appeui 70B3D57ED00082D2\r\n", 4);
	wait(SEC*2);
	usartSend("mac set appkey D94AC6F27881D3505F3E595B69472898\r\n", 4);
	wait(SEC*2);
	usartSend("mac save\r\n", 4);
	wait(SEC*2);
	usartSend("mac join otaa\r\n", 4);
	wait(SEC*15);
	///debug
	usartSend("mac get status\r\n", 4);
	wait(SEC*5);
	usartSend("mac get devaddr\r\n", 4);
	wait(SEC*5);
	
	
	
}


int sendCommand(char *phrase, char *check, int pocetentru)
{
	char incomming[50] = {0};   //readed string
	int i=0;                                              //iteracni promenna
	int enter=0;                                          //detekce znaku CR

	//posila na linku
	while(phrase[i] != '\0'){  //posle string
		usart_send_blocking(USART4, phrase[i]);
		i++;
	}
	i=0;

	//cte z linky dokud neprijme tolik entru kolik ceka
	while(enter < pocetentru){                                 
		incomming[i] = usart_recv_blocking(USART4);
        	if (incomming[i] == '\n')
			enter++;
		i++;
	}

	if ( (strstr(incomming, check)) == NULL) {
		return 1;
	} else {
		return 0;
	}
}


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 		NBIOT QUECTEL WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/


void connect_nbiot(void)
{
	while(sendCommand("AT+CFUN=1\r\n", "OK", 2))
		wait(3.0);
	while(sendCommand("AT+COPS=1,2,\"23003\"\r\n", "OK", 2))
		wait(0.5);
	while( sendCommand("AT+CGATT?\r\n", "CGATT:1", 4)) //timeout = number of tries	
		wait(0.5);
}



