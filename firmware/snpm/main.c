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


#include <libopencm3/stm32/spi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <libopencm3/stm32/nvic.h>
#include "inc/functions.h"
#include "inc/bme280.h"
#include "inc/opcn2.h"
#include "inc/wireless.h"
#include "inc/cayenne_lpp.h"


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma GCC diagnostic ignored "-Wmissing-declarations"
//#pragma GCC diagnostic ignored "-Wreturn-type"


//#define LORAWAN
#define NBIOT

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
	init_BME280();

// semihosting - stdio po debug konzoli, inicializace
/*
#if defined(ENABLE_SEMIHOSTING) && (ENABLE_SEMIHOSTING)
	initialise_monitor_handles();
	setbuf(stdout, NULL);
#endif
*/
	flash(3, 50000);

	usartSend("Entering main loop.\r\n\r\n", 2);

	struct CayenneLPP *lpp;
	unsigned char *buf;
	int size;

	//gps/lora module HW reset
	gpio_clear(GPIOA, GPIO9);
	wait(SEC*0.5);
	gpio_set(GPIOA, GPIO9);

#ifdef NBIOT
	usartSend("Quectel reset.\r\n", 2);
	wait(SEC*5);//until quectel wakes up
#endif
/*
	//tohle udela debugovaci spike, je to tady kvuli logicke sondy, v produkci dat pryc
	usartSend("Debug spike.\r\n", 2);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
	gpio_set(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
*/
	//printf ("Wait...\r\n", 2);
	//wait(SEC*5);

	//Connect to network
#ifdef LORAWAN
	connect_lorawan();
#endif

#ifdef NBIOT
	connect_nbiot();
	usartSend("Nb-IOT network connected.\r\n", 2);
#endif

	particlemeter_ON();
	particlemeter_set_fan(FAN_SPEED);
	usartSend("Particlemeter set.\r\n", 2);

	flash(10, 50000);

	while (1){
		usartSend("New loop\r\n", 2);
		read_pm_values();
		data_readout_BME280(burst_read_data);

		float hum = hum_BME280();
		float temp = temp_BME280();
		float press = press_BME280();
		float pm1 = 1.1;//particlemeter_pm1();
		float pm2_5 = 2.5;//particlemeter_pm2_5();
		float pm10 = 10.10;//particlemeter_pm10();

		usartSend("Encode values\r\n", 2);

		// init cayenne lpp
		lpp = CayenneLPP__create(100);

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

		char* hex_string = string_to_hex(buf, size);
		char* send_string = concat("mac tx uncnf 1 ", hex_string);
		free(hex_string);
//		send_string = concat(send_string, "\r\n");

		//printf("Send string: %s\n", send_string);
		#ifdef LORAWAN
		//Send string: mac tx uncnf 1 0167010e0273260c0368000402006e050200fa
		//								060203f2078807fdd800bee10000c8
		

		lora_sendCommand(send_string);
		//usartSend(send_string, 2);
		#endif
		
		#ifdef NBIOT
		
		//socket opening
		while (nbiot_sendCommand("AT+NSOCR=DGRAM,17,9999,1\r\n", "OK\r\n", 4))
                wait(1);
        //make string 
        char nbiot_cmd[100] = "AT+NSOST=0,193.84.207.60,9999,";
        char nbiot_data_length[10];
        char ending[] = "\r\n";
        
        //convert dta length number into string
        sprintf (nbiot_data_length, "%d", size);
        //concatenate this packet length at the end of string
        strcat(nbiot_cmd, nbiot_data_length);
        //separate by comma
        strcat(nbiot_cmd, ',');
        //add LPP encoded packet
        strcat(nbiot_cmd, hex_string);
        //add \r\n
        strcat(nbiot_cmd, ending);
        
        usartSend(nbiot_cmd, 2);//send_string);
        usartSend("\r\n", 2);
        //Sending datagram
 //       while (nbiot_sendCommand(nbiot_cmd, "OK\r\n", 4))   //sending "!!!LABKA->up-and-running!!!" string to specified port
 //               wait(1);
		//Closing socket     fixme: chtelo by to vychytat konkretni socket, pripadne to nejak jinak obechcat
        while (nbiot_sendCommand("AT+NSOCL=0\r\n", "OK\r\n", 2)){;}
                wait(1);
       
		#endif
		
		free(send_string);

		usartSend("Sending done\r\n", 2);
		free(lpp);

		usartSend("Wait at the end...\r\n", 2);

		wait(SEC *1);
		
	}
	return 0;
}
