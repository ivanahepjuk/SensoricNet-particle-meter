
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
 #include <libopencm3/stm32/iwdg.h>
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
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, phrase[i]);
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
		flash(3, 10000);
		return 1;
	} else {
		flash(1, 30000);
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

void nbiot_reset(void)
{
	//IoT module HW reset
	debug_usart_send("IoT module hw reset");
	gpio_clear(IOT_RESET_GPIO_GROUP, IOT_RESET_GPIO);
	wait(SEC*0.5);
	gpio_set(IOT_RESET_GPIO_GROUP, IOT_RESET_GPIO);
	wait(SEC*3);
	debug_usart_send("IoT module hw reset done");
}
  
void nbiot_connect(void)
{
	uint8_t wdg_resets = 10;

	while(nbiot_sendCommand("AT+CFUN=1\r\n", "OK", 2))
	//usartSend("at+cfun\r\n", 2);
		wait(SEC*3);
	while(nbiot_sendCommand("AT+COPS=1,2,\"23003\"\r\n", "OK", 2))
	//usartSend("at+cops\r\n", 2);
		wait(SEC*5);
	while(nbiot_sendCommand("AT+CGATT?\r\n", "CGATT:1", 4)) //timeout = number of tries	
	{
		if (wdg_resets > 0)
		{
			iwdg_reset();
			wdg_resets--;
		}
		wait(SEC*7);
	}
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

	uint32_t timestamp = ticker;

	//posila na linku
	while(phrase[i] != '\0')
	{	//posle string
		usart_send_blocking(USART4, phrase[i]);
	#ifdef DEBUG
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, phrase[i]);
	#endif
		wait(1000);
		i++;
		//if anything goes wrong, escape function and do it again
		if((ticker - timestamp)>8)
		{
			usart_send_blocking(USART4, '\r');
			usart_send_blocking(USART4, '\n');
			wait(1000);
			return 1;	
		}
	}
	i=0;
	//interrupt status register poreseny
	USART_ICR(USART4) |= 0b00000000000010100011101110101111;
	//cte z linky dokud neprijme tolik entru kolik ceka
	while(enter < pocetentru){

		incomming[i] = usart_recv_blocking(USART4);
		#ifdef DEBUG 
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, incomming[i]);
		#endif
		if (incomming[i] == '\n')
			enter++;
		i++;
		//if anything goes wrong, escape function and do it again
		if((ticker - timestamp)>8)
		{
			usart_send_blocking(USART4, '\r');
			usart_send_blocking(USART4, '\n');
			wait(1000);
			return 1;	
		}
	}

	//led_flash(1, 3, 200000);
	//led_flash(1, 2, 20000);
/*	if ( (strstr(incomming, "ERROR")) == NULL) {
		led_flash(2,3, 10000);
		return 1;
	} */
	
	if ( (strstr(incomming, check)) == NULL) {
		led_flash(2,3, 10000);
		return 1;
	} else {
		led_flash(2,1,10000);
		return 0;
	}
}

uint8_t nbiot_csq(void)
{
	char incomming[50] = {0};	//readed string
	int i=0;	//iteracni promenna
	int enter=0;	//detekce znaku CR
	int pocetentru = 2;

	uint32_t timestamp = ticker;

	//posila na linku:
	char phrase[] = "AT+CSQ\r\n";


	while(phrase[i] != '\0'){	//posle string
		usart_send_blocking(USART4, phrase[i]);
	#ifdef DEBUG
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, phrase[i]);
	#endif
		wait(1000);
		i++;
		//if anything goes wrong, escape function and do it again
		if((ticker - timestamp)>8)
		{
			usart_send_blocking(USART4, '\r');
			usart_send_blocking(USART4, '\n');
			wait(1000);
			return 1;	
		}
	}
	i=0;
	//interrupt status register poreseny
	USART_ICR(USART4) |= 0b00000000000010100011101110101111;
	//cte z linky dokud neprijme tolik entru kolik ceka
	while(enter < pocetentru){

		incomming[i] = usart_recv_blocking(USART4);
		#ifdef DEBUG 
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, incomming[i]);
		#endif
		if (incomming[i] == '\n')
			enter++;
		i++;
		//if anything goes wrong, escape function and do it again
		if((ticker - timestamp)>8)
		{
			usart_send_blocking(USART4, '\r');
			usart_send_blocking(USART4, '\n');
			wait(1000);
			return 1;	
		}
	}
	sscanf(incomming, "\r\n+CSQ:%d,%d", &csq[0], &csq[1]);
	//rozebere precteny string a ulozi do global promenne:
	//signal strength, bit error rate

	//test jestli vratil spravne cisla nebo je mimo
	if ((csq[0] < 0) || ((csq[0] > 51) && (csq[0] < 99)) || (csq[0] > 99))
		csq[0] = 52; //52 znamena error
	if ((csq[1] < 0) || ((csq[1] > 7 ) && (csq[0] < 99)) || (csq[1] > 99))
		csq[1] = 52; //52 znamena error
	#ifdef DEBUG
	char pokus[30];
	sprintf(pokus, "%d %d", csq[0], csq[1]);
	debug_usart_send(pokus);
	#endif
	return 0;
}


uint8_t nbiot_nuestats(void)
{
	char incomming[200] = {0};	//readed string
	int i=0;	//iteracni promenna
	int enter=0;	//detekce znaku CR
	int pocetentru = 14;//17 u starsiho quectelu

	uint32_t timestamp = ticker;

	//posila na linku:
	char phrase[] = "AT+NUESTATS\r\n";
	
	while(phrase[i] != '\0'){	//posle string
		usart_send_blocking(USART4, phrase[i]);
	#ifdef DEBUG
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, phrase[i]);
	#endif
		wait(1000);
		i++;
		//if anything goes wrong, escape function and do it again
		if((ticker - timestamp)>8)
		{
			usart_send_blocking(USART4, '\r');
			usart_send_blocking(USART4, '\n');
			wait(1000);
			return 1;	
		}
	}
	i=0;
	//interrupt status register poreseny
	USART_ICR(USART4) |= 0b00000000000010100011101110101111;
	//cte z linky dokud neprijme tolik entru kolik ceka
	while(enter < pocetentru){

		incomming[i] = usart_recv_blocking(USART4);
		#ifdef DEBUG 
		//sending stuff to debug port
		usart_send_blocking(DEBUG_USART, incomming[i]);
		#endif
		if (incomming[i] == '\n')
			enter++;
		i++;
		//if anything goes wrong, escape function and do it again
		if((ticker - timestamp)>8)
		{
			usart_send_blocking(USART4, '\r');
			usart_send_blocking(USART4, '\n');
			wait(1000);
			return 1;	
		}
	}

	sscanf(incomming, "\r\nSignal power:%d\r\nTotal power:%d\r\nTX power:%d\r\nTX time:%d\r\nRX time:%d\r\nCell ID:%d\r\nDL MCS:%d\r\nUL MCS:%d\r\nDCI MCS:%d\r\nECL:%d\r\nSNR:%d\r\nEARFCN:%d\r\nPCI:%d\r\nRSRQ:%d\r\n", &nuestats[0], &nuestats[1], &nuestats[2], &nuestats[3], &nuestats[4], &nuestats[5], &nuestats[6], &nuestats[7], &nuestats[8], &nuestats[9], &nuestats[10], &nuestats[11], &nuestats[12], &nuestats[13]);
	//TODO testovat nejak jestli jsou hodnoty v poradku
	#ifdef DEBUG
	//debugovaci vypis
	char pokus[70];
	sprintf(pokus, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", nuestats[0], nuestats[1], nuestats[2], nuestats[3], nuestats[4], nuestats[5], nuestats[6], nuestats[7], nuestats[8], nuestats[9], nuestats[10], nuestats[11], nuestats[12], nuestats[13]);
	debug_usart_send(pokus);	
	#endif
	return 0;
}

void send_reboot_variable(void)
{
	
		//create string
		char send_string[200] = {0};
		char hex_string[100] = {0};
		
		strcat(send_string, "AT+NSOST=0,193.84.207.60,9999,15,"); 

		//prepare nbiot-000x ide:
		uint8_t j=0;
		char znak[3];

		while(j<10){
			charToHex(ID[j], znak);
			strcat(send_string, znak);
			j++;
		}
/*
		id_decoded[20] = '0';
		id_decoded[21] = '0';
		id_decoded[22] = NULL;
*/
		strcat(send_string, "00\0");  //t treti nul je tam jako ukoncjici retezec

		char lpp[10] = {0};

		lpp[0] = 0x22;//data channel
		lpp[1] = 0x01;//0x01 means "digital output" 
		lpp[2] = 0x1;//posilam jednicku proste, sizeof 1B
		lpp[3] = '\0';
		
		j=0;
		
		while(j<4){
			charToHex(lpp[j], znak);
			strcat(hex_string, znak);
			j++;
		}

		strcat(send_string, hex_string);
		strcat(send_string, "\r\n");
		//pocet_pulsu = 0;
		debug_usart_send(send_string);

		//socket opening
		while (nbiot_sendCommand("AT+NSOCR=DGRAM,17,9999,1\r\n", "OK\r\n", 4))
			wait(1);
		//Sending datagram
		while (nbiot_sendCommand(send_string,"OK", 4))
			wait(3);
		//Closing socket
		while (nbiot_sendCommand("AT+NSOCL=0\r\n", "OK", 2))
			wait(1);

		//send each 10s
		//blnk();
				
}


