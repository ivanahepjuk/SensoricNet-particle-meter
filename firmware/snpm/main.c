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



#define LORAWAN


/* For semihosting on newlib */
extern void initialise_monitor_handles(void);

/////////////////////////////////////////////////////////////
//Global variables for burst register reading, for bme280: //
/////////////////////////////////////////////////////////////
int32_t     t_fine;
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


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


char* string_to_hex(char *string, int len)
{
	char *result = malloc(len*2+1);
	unsigned int pointer = result;
//	char hex[3];
	int x;

	for (x = 0; x < len; ++x) {
		sprintf(pointer, "%02x", *string);

//		sprintf(hex, "%02x", *string);
//		printf("hex: %s ", hex);

		string++;
		pointer++;
		pointer++;
	}

//	printf("done\n");

	return result;
}


/********************************************************************************************
 *
 * MAIN
 *
 ********************************************************************************************/

int main(void)
{

	/*
    //interrupts:
	//uart_enable_interrupts(USART4, UART_INT_RX);
	// Unmask receive interrupt
	uart_enable_rx_interrupt(UART4);
	// Make sure the interrupt is routed through the NVIC
	nvic_enable_irq(NVIC_UART4_RX);
	*/
	
	clock_setup();
	gpio_setup();
	usart_setup();
	i2c_setup();
	spi_setup();
	init_BME280();

// semihosting - stdio po debug konzoli, inicializace
#if defined(ENABLE_SEMIHOSTING) && (ENABLE_SEMIHOSTING)
	initialise_monitor_handles();
	setbuf(stdout, NULL);
#endif

	flash(10);

	printf ("Entering main loop.\n");

	struct CayenneLPP *lpp;
	unsigned char *buf;
	int w, size;


#ifdef NBIOT
	printf ("Quectel reset.\n");

	//Quectel wireless modul HW reset
	gpio_clear(GPIOA, GPIO9); 
	wait(SEC*0.2); 
	gpio_set(GPIOA, GPIO9);
	
	wait(SEC*5);//until quectel wakes up
	//wait(1);//until quectel wakes up
#endif

	printf ("Debug spike.\n");

	//tohle udela debugovaci spike, je to tady kvuli logicke sondy, v produkci dat pryc
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
	gpio_set(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?

	printf ("Wait.\n");
	wait(SEC*5);

	printf ("Network connect.\n");

	//Connect to network
#ifdef LORAWAN
	connect_lorawan();
#endif

#ifdef NBIOT
	connect_nbiot();
#endif

	printf ("Network connect done.\n");

	printf ("Enabling particlemeter.\n");
	particlemeter_ON();

	printf ("Setting particlemeter fan.\n");
	particlemeter_set_fan(FAN_SPEED);

	flash(3);

	printf ("Settings done.\n");
	
	while (1){

		printf ("New loop\n");

		printf ("Reading pm values\n");
		read_pm_values();
		printf ("Reading BME280 values\n");
//		data_readout_BME280(burst_read_data);

		float hum = hum_BME280();
		float temp = temp_BME280();
		float press = press_BME280();
		float pm1 = particlemeter_pm1();
		float pm2_5 = particlemeter_pm2_5();
		float pm10 = particlemeter_pm10();


		//send nbiot
		//char str_data[210];
		//int i = 0;

		//char str1[]="AT+NSOST=0,89.103.47.53,8089,";
		//char str2[]="hodnoty,box=Adrspach hum=";
		//char str3[]=",temp=";
		//char str4[]=",pres=";
		//char str5[]=",pm1=";
		//char str6[]=",pm25=";
		//char str7[]=",pm10=";

		//vezme retezec ze str2, konvertuje na hexadecimal a pricte k retezci str_data
		/*
		while(str2 != '\0'){
		  charToHex(str2[i], hex_string);
		  strcat(str_data, hex_string);
		  i++;
		}
		
		//vynuluje iteracni promennou
		i=0;
		*/
		
		flash(1);
		
		// mame tyto senzory
		// teplota, tlak, vlhkost, pm1, pm2_5, pm10, gps

		printf ("Data: %f, %f, %f, %f, %f, %s\n", temp, press, hum, pm1, pm2_5, pm10);

		printf ("Encode values\n");

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

		printf ("Encoded data size: %i\n", size);

		char* hex_string = string_to_hex(buf, size);
		char* send_string = concat("mac tx uncnf 1 ", hex_string);
		free(hex_string);
		send_string = concat(send_string, "\r\n");

		printf ("Send string: %s\n", send_string);
		usartSend(send_string, 4);
		free(send_string);

		printf ("Sending done\n");
		free(lpp);

		printf ("Wait...\n");

		wait(SEC *10);
		
	}
	return 0;
}
