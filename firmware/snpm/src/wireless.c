
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

#include "functions.h"
#include "wireless.h"


/**********************************************************
 * LORWAN
 **********************************************************/
void connect_lorawan(void)
{

	lora_sendCommand("sys reset");

	// po resetu musim pockat na sestupnou hranu RX a pak
	// zase na vzestupnou, pak rn vybleje



	lora_sendCommand("sys get hweui");
	lora_sendCommand("mac get deveui");

	lora_sendCommand("radio get mod");
	lora_sendCommand("radio get freq");
	lora_sendCommand("radio get sf");
	lora_sendCommand("radio get bw");
	lora_sendCommand("radio get cr");
	lora_sendCommand("radio get prlen");
	lora_sendCommand("radio get pwr");

	lora_sendCommand("mac set dr 1");
	lora_sendCommand("radio set pwr 14");
	lora_sendCommand("mac set deveui 0004A30B00222137");
	lora_sendCommand("mac set appeui 70B3D57ED00082D2");
	lora_sendCommand("mac set appkey D94AC6F27881D3505F3E595B69472898");
	lora_sendCommand("mac save");
	lora_sendCommand("mac join otaa");
//	lora_sendCommand("mac get status\r\n");
//	lora_sendCommand("mac get devaddr\r\n");

}


void lora_sendCommand(char *phrase)
{
	uint32_t i=0;
	uint16_t recieved_char;

	printf("lora cmd: ");
	while(phrase[i] != '\0')
	{
		usart_send_blocking(USART4, phrase[i]);
		printf("%c", phrase[i]);
		i++;
	}

//	wait (1000);
	usart_send_blocking(USART4, '\r');
	usart_send_blocking(USART4, '\n');
	printf("\n");

//	wait (1000);
/*
	do {
		printf(".");
		recieved_char = usart_recv (USART4);
		printf("%04x", recieved_char);
	} while (recieved_char != '\n');
*/
}




 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 		NBIOT QUECTEL WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/


void connect_nbiot(void)
{
	printf ("test_uvnitr\n");
	while(nbiot_sendCommand("AT+CFUN=1\r\n", "OK", 1))
	printf ("at+cfun\n");
		wait(3.0);
	while(nbiot_sendCommand("AT+COPS=1,2,\"23003\"\r\n", "OK", 2))
	printf ("at+cops\n");
		wait(0.5);
	while(nbiot_sendCommand("AT+CGATT?\r\n", "CGATT:1", 4)) //timeout = number of tries	
	printf ("at+cgatt?\n");
		wait(0.5);
}


int nbiot_sendCommand(char *phrase, char *check, int pocetentru)
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


