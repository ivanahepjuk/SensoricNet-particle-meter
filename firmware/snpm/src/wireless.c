
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
  *  int lora_sendCommand(char *phrase, char *check, int pocetentru)
  * 
  * @phrase - string to send (including \r\n)
  * @check - string which I am expecting to get if everything is ok
  * @pocetentru - number of \n characters in response
  * @pocetpokusu - number of tries before calling master reset
  * 
  * */
  
 int lora_sendCommand(char *phrase, char *check, int pocetentru)
{
	char incomming[50] = {0};   //readed string
	int i=0;                                              //iteracni promenna
	int enter=0;                                          //detekce znaku CR

	//posila na linku
	while(phrase[i] != '\0'){  //posle string
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
        	if (incomming[i] == '\n'){
			enter++;
		}
		i++;
	}

	
	if ( (strstr(incomming, check)) == NULL) {
		return 1;
	} else {
		return 0;
	}
}

 
 /**
  *  void connect_lorawan(void)
  * 
  * Initial network connection
  * 
  * */
void connect_lorawan(void)
{
	//lora_sendCommand("sys reset");
	//po resetu musim pockat na sestupnou hranu RX a pak
	//zase na vzestupnou, pak rn vybleje

	//lora_sendCommand("sys get hweui"); //ok, invalid_param
	//lora_sendCommand("mac get deveui");

	//lora_sendCommand("radio get mod");
	//lora_sendCommand("radio get freq");
	//lora_sendCommand("radio get sf");
	//lora_sendCommand("radio get bw");
	//lora_sendCommand("radio get cr");
	//lora_sendCommand("radio get prlen");
	//lora_sendCommand("radio get pwr");
	//set data rate
	while(lora_sendCommand("mac set dr 1\r\n", "ok", 1)){
		wait(SEC*3);
	}
	//set tx power
	while(lora_sendCommand("radio set pwr 14\r\n", "ok", 1)){
		wait(SEC*3);
	}
	//set device EUI
	while(lora_sendCommand("mac set deveui 0004A30B00222137\r\n", "ok", 1)){
		wait(SEC*3);
	}
	//set application EUI
	while(lora_sendCommand("mac set appeui 70B3D57ED00082D2\r\n", "ok", 1)){
		wait(SEC*3);
	}
	//set application key
	while(lora_sendCommand("mac set appkey D94AC6F27881D3505F3E595B69472898\r\n", "ok", 1)){
		wait(SEC*3);
	}
	//save all
	while(lora_sendCommand("mac save\r\n", "ok", 1)){
		wait(SEC*3);
	}
	//join network!!!
	while(lora_sendCommand("mac join otaa\r\n", "ok", 1)){
		wait(SEC*15);
	}

	//lora_sendCommand("mac get status\r\n");
	//lora_sendCommand("mac get devaddr\r\n");

}





/*
void lora_sendCommand(char *phrase)
{
	uint32_t i=0;
//	uint16_t recieved_char;

	printf("lora cmd: ");
	while(phrase[i] != '\0')
	{
		usart_send_blocking(USART4, phrase[i]);
		printf("%c", phrase[i]);
		i++;
	}

	wait (1000);
	usart_send_blocking(USART4, '\r');
	usart_send_blocking(USART4, '\n');
	printf("\n");


	//wait (2000000);

//	do {
//		printf(".");
//		recieved_char = usart_recv (USART4);
//		printf("%04x", recieved_char);
//	} while (recieved_char != '\n');


}
*/



 /** * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 		NBIOT QUECTEL WIRELESS MODULE FUNCTIONS			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

 /**
  *  void connect_lorawan(void)
  * 
  * Initial network connection
  * 
  * */
  
void connect_nbiot(void)
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
	char incomming[50] = {0};   //readed string
	int i=0;                                              //iteracni promenna
	int enter=0;                                          //detekce znaku CR

	//posila na linku
	while(phrase[i] != '\0'){  //posle string
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



