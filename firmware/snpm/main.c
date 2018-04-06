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

#include "main.h"

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma GCC diagnostic ignored "-Wmissing-declarations"
//#pragma GCC diagnostic ignored "-Wreturn-type"

//debug
int cykly = 0;
char cykly_str[10];

/* For semihosting on newlib */
//extern void initialise_monitor_handles(void);

/////////////////////////////////////////////////////////////
//Global variables for burst register reading, for bme280: //
/////////////////////////////////////////////////////////////
int32_t t_fine;
uint8_t comp_data[34];//compensation data readed into this
uint8_t burst_read_data[8] = {0};//in loop measured data readed into this

/////////////////////////////////////////////////////////////
//Global variables for compensation functions for bme280:  //
/////////////////////////////////////////////////////////////
//temperature
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
//pressure
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
//humidity
unsigned char dig_H1, dig_H3;
int16_t dig_H2, dig_H4, dig_H5;
signed char dig_H6;

/////////
char ID[11];
char id_decoded[23]={0};

/////////////////////////////////////////////////////////////
//Global variables for burst register reading, for OPC-N2: //
/////////////////////////////////////////////////////////////

uint8_t histogram_buffer[62];//whole dataset of opc readed into this
uint8_t pm_values_buffer[12] = {0};//only pm data
/*
void usart4_isr(void)
{
	
	     uint32_t serviced_irqs = 0;
     // Process individual IRQs
     if (uart_is_interrupt_source(UART0, UART_INT_RX)) {
        process_rx_event();
        serviced_irq |= UART_INT_RX;
     }
     if (uart_is_interrupt_source(UART0, UART_INT_CTS)) {
        process_cts_event();
        serviced_irq |= UART_INT_CTS;
     }
     // Clear the interrupt flag for the processed IRQs
     uart_clear_interrupt_flag(UART0, serviced_irqs);
     
     
//tady dopsat kod preruseni	
flash(7);	
}
*/

/********************************************************************************************
 *
 * MAIN
 *
 ********************************************************************************************/

int main(void)
{
	
	clock_setup();
	gpio_setup();
	usart_setup();
	i2c_setup();
	spi_setup();

	//   !!!   Uncomment this only if you know what you are mdoing,   
	//   !!!!  This is used when deploying new devices   !!!!
	//eeprom_write_id("nbiot-0005");

	//reads ID from eeprom
	eeprom_read_id();
	usartSend(ID, 2);
	init_BME280();
	
	char pokus[100];
	sprintf(pokus, "id0: %d id1: %d id2: %d", DESIG_UNIQUE_ID0, DESIG_UNIQUE_ID1, DESIG_UNIQUE_ID2);
	usartSend(pokus, 2);

// semihosting - stdio po debug konzoli, inicializace
/*
#if defined(ENABLE_SEMIHOSTING) && (ENABLE_SEMIHOSTING)
	initialise_monitor_handles();
	setbuf(stdout, NULL);
#endif
*/
	flash(1, 100000);

	

	struct CayenneLPP *lpp;
	unsigned char *buf;
	int size;

	//gps/lora module HW reset
	gpio_clear(GPIOA, GPIO9);
	wait(SEC*0.5);
	gpio_set(GPIOA, GPIO9);

#ifdef NBIOT
	usartSend("Quectel reset.\r\n", 2);
	wait(SEC*15);//until quectel wakes up
#endif
/*
	//tohle udela debugovaci spike, je to tady kvuli logicke sondy, v produkci dat pryc
	usartSend("Debug spike.\r\n", 2);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
	gpio_set(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
*/

	//Connect to network
#ifdef LORAWAN
	usartSend("rn2483 reset.\r\n", 2);
	wait(SEC*5);//until rn2483 wakes up
	connect_lorawan();
#endif

#ifdef NBIOT
	connect_nbiot();
	usartSend("Nb-IOT network connected.\r\n", 2);
#endif

	particlemeter_ON();
	wait(200000);
	particlemeter_set_fan(FAN_SPEED);
	usartSend("Particlemeter set.\r\n", 2);

	flash(3, 50000);
	
	// init cayenne lpp
	lpp = CayenneLPP__create(200);

	while (1){
		usartSend("New loop\r\n", 2);
		read_pm_values();
		data_readout_BME280(burst_read_data);

		float hum = hum_BME280();
		float temp = temp_BME280();
		float press = press_BME280();
		float pm1 = particlemeter_pm1();
		float pm2_5 = particlemeter_pm2_5();
		float pm10 = particlemeter_pm10();

		usartSend("Encode values\r\n", 2);
		char debug_data_string[150] = {0};
		sprintf(debug_data_string, "hum: %.2f temp: %.2f press: %.2f pm1: %.2f pm2_5: %.2f pm10: %.2f\r\n", hum, temp, press, pm1, pm2_5, pm10);
		usartSend(debug_data_string, 2);
		
		
		
		CayenneLPP__addTemperature(lpp, 1, temp);
		CayenneLPP__addBarometricPressure(lpp, 2, press);
		CayenneLPP__addRelativeHumidity(lpp, 3, hum);
		CayenneLPP__addAnalogInput(lpp, 4, pm1);
		CayenneLPP__addAnalogInput(lpp, 5, pm2_5);
		CayenneLPP__addAnalogInput(lpp, 6, pm10);
		CayenneLPP__addGPS(lpp, 7, 52.37365, 4.88650, 2);

		buf=CayenneLPP__getBuffer(lpp);
		size=CayenneLPP__getSize(lpp);

		// Send it off
		//sendData(CayenneLPP__getBuffer(lpp), CayenneLPP__getSize(lpp));

		//printf("Encoded data size: %i\n", size);

		char hex_string[200] = {0};		//encoded string here
		char send_string[200] = {0};			//string to send here
		
		//ascii-hex encoding is happening here:
		int j = 0;
		char znak[3];
		
		while(j<size){
			charToHex(buf[j], znak);
			strcat(hex_string, znak);
			j++;
		}
		

		#ifdef LORAWAN
			//sestaveni stringu pro LORAWAN
			strcat(send_string, "mac tx uncnf 1 ");
			strcat(send_string, hex_string);
			strcat(send_string, "\r\n");
			
			//odeslani stringu, checkuje "ok", pokud nedostane ok tak to zkusi za chvili znova
			while(lora_sendCommand(send_string, "ok", 1)){
				wait(SEC*3);
			}
			
		#endif
		
		#ifdef NBIOT
			//tady se konvertuje na string a ulozi delka payloadu (int)
			char nbiot_data_length[10];
			sprintf (nbiot_data_length, "%d", size+(11));
			
			//sestaveni stringu pro Nb-IOT
			strcat(send_string, "AT+NSOST=0,193.84.207.60,9999,");
			strcat(send_string, nbiot_data_length);
			strcat(send_string, ",");
			
			////experimental, eeprom string decoding
			j=0;
			while(j<10){
			charToHex(ID[j], znak);
			strcat(id_decoded, znak);
			j++;
			}
			id_decoded[20] = 48;
			id_decoded[21] = 48;
			id_decoded[22] = NULL;
			
			
			usartSend(id_decoded, 2);
			
			strcat(send_string, id_decoded);  //nbiot-0001
			//strcat(send_string, "00");  //nbiot-0001
			strcat(send_string, hex_string);
			strcat(send_string, "\r\n");

			//socket opening
			while (nbiot_sendCommand("AT+NSOCR=DGRAM,17,9999,1\r\n", "OK\r\n", 4))
					wait(SEC*1);
			//Sending datagram
			usartSend(send_string, 2);
			while (nbiot_sendCommand(send_string, "OK", 4))   
					wait(SEC*3);
			//Closing socket
			while (nbiot_sendCommand("AT+NSOCL=0\r\n", "OK", 2)){;}
					wait(SEC*1);

		#endif
		
		CayenneLPP__reset(lpp);
		//lpp->cursor = NULL;
		
		//DEBUG CODE posila cislo loop smycky
		usartSend("Loop done: ", 2);
		sprintf(cykly_str, "%d", cykly);
		usartSend(cykly_str, 2);
		usartSend("\r\n", 2);
		cykly++;

		flash(3, 100000);

		wait(SEC *WAIT);		
	}
	return 0;
}
