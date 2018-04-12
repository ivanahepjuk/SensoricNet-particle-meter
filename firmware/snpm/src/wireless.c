
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
#include <libopencm3/stm32/usart.h>
#include "../main.h"

uint8_t usart_read(void);


/**********************************************************
 * LORAWAN RN2483 WIRELESS MODULE FUNCTIONS	
 **********************************************************/


/**
 * resets rn2483 module
 */
void lorawan_reset(void)
{
	lorawan_sendCommand("rn2483 reset", "ok", 1);
	
	//TODO loop
}

/**
 * save rn2483 module state
 */
void lorawan_mac_save(void)
{
	lorawan_sendCommand("mac save", "ok", 1);

	//TODO loop
}


/**
 * connects into lorawan network
 */
void lorawan_connect(void)
{
	// selects 868 bands defaults
	while(lorawan_sendCommand("mac reset 868", "ok", 1)) {
		wait(SEC*3);
	}

	// set data rate (0-7)
	while(lorawan_sendCommand("mac set dr 5", "ok", 1)) {
		wait(SEC*3);
	}

	// set tx power
	while(lorawan_sendCommand("radio set pwr 14", "ok", 1)) {
		wait(SEC*3);
	}

	// set adr (adaptive data rate, default off), nemusi byt podporovane siti, tak bacha!
	while(lorawan_sendCommand("mac set adr on", "ok", 1)) {
		wait(SEC*3);
	}

	//set device EUI
	while(lorawan_sendCommand("mac set deveui " LORAWAN_DEVEUI, "ok", 1)) {
		wait(SEC*3);
	}

	//set application EUI
	while(lorawan_sendCommand("mac set appeui " LORAWAN_APPEUI, "ok", 1)) {
		wait(SEC*3);
	}

	//set application key
	while(lorawan_sendCommand("mac set appkey " LORAWAN_APPKEY, "ok", 1)) {
		wait(SEC*3);
	}

	//save module state
	lorawan_mac_save();

	//join network
	int i = 1;
	while (lorawan_sendCommand("mac join otaa", "accepted", 2)) {
		wait(SEC*5*(2*i));
		if (i<8) {
			i++;
		}
	}
}

/**
 *  sends lora command
 *
 * @phrase - string to send (without \r\n)
 * @check - string which I am expecting to get if everything is ok
 * @pocetentru - number of \n characters in response
 * @pocetpokusu - number of tries before calling master reset TODO
 */
int lorawan_sendCommand(char *phrase, char *check, int pocetentru)
{
	char incomming[50] = {0};	//readed string
	int i=0;		//iteracni promenna
	int enter=0;	//detekce znaku CR

	//posila na linku
	while(phrase[i] != '\0') {
		//posle string
		usart_send_blocking(USART4, phrase[i]);

		#ifdef DEBUG
		//sending stuff to usart2 debug port
		usart_send_blocking(USART2, phrase[i]);
		#endif
		i++;
	}
	// command is ended by CR, LF
	usart_send_blocking(USART4, '\r');
	usart_send_blocking(USART4, '\n');


	//interrupt status register poreseny
	USART_ICR(USART4) |= 0b00000000000010100011101110101111;
	//cte z linky dokud neprijme tolik entru kolik ceka
	//TODO pujde implementovat timeout cteni?
	i=0;
	while(enter < pocetentru) {

		incomming[i] = usart_recv_blocking(USART4);
		#ifdef DEBUG
		//sending stuff to usart2 debug port
		usart_send_blocking(USART2, incomming[i]);
		#endif

		if (incomming[i] == '\n'){
			enter++;
		}
		i++;
	}

	// check output
	if ((strstr(incomming, check)) == NULL) {
		return 1;
	} else {
		return 0;
	}
}




 /** * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 		NBIOT QUECTEL WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

 /**
  *  void connect_lorawan(void)
  * 
  * Initial network connection
  * 
  * */
  
void nbiot_connect(void)
{
	flash(5, 50000);
	//usartSend("test uvnitr\r\n", 2);
	while(nbiot_sendCommand("AT+CFUN=1\r\n", "OK", 2))
	//usartSend("at+cfun\r\n", 2);
		wait(SEC*3);
	while(nbiot_sendCommand("AT+COPS=1,2,\"23003\"\r\n", "OK", 2))
	//usartSend("at+cops\r\n", 2);
		wait(SEC*3);
	while(nbiot_sendCommand("AT+CGATT?\r\n", "CGATT:1", 4)) //timeout = number of tries	
	//usartSend("at+cgatt?\r\n", 2);
		wait(SEC*3);
}

 
 /**
  *  int nbiot_sendCommand(char *phrase, char *check, int pocetentru)
  * 
  * @phrase - string to send (including \r\n)
  * @check - string which I am expecting to get if everything is ok
  * @pocetentru - number of \n characters in response
  * @pocetpokusu - number of tries before calling master reset
  * 
  * */

int nbiot_sendCommand(char *phrase, char *check, int pocetentru)
{
	char incomming[50] = {0};	//readed string
	int i=0;	//iteracni promenna
	int enter=0;	//detekce znaku CR

	//posila na linku
	while(phrase[i] != '\0'){	//posle string
		usart_send_blocking(USART4, phrase[i]);
	#ifdef DEBUG
		//sending stuff to usart2 debug port
		usart_send_blocking(USART2, phrase[i]);
	#endif
		i++;
	}
	i=0;
	//interrupt status register poreseny
	USART_ICR(USART4) |= 0b00000000000010100011101110101111;
	//cte z linky dokud neprijme tolik entru kolik ceka
	while(enter < pocetentru){

		incomming[i] = usart_recv_blocking(USART4);
		#ifdef DEBUG 
		//sending stuff to usart2 debug port
		usart_send_blocking(USART2, incomming[i]);
		#endif
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



