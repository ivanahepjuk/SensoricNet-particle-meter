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
//#include <libopencm3/stm32/nvic.h>
#include "inc/functions.h"
#include "inc/bme280.h"
#include "inc/opcn2.h"
#include "inc/wireless.h"
#include "inc/cayenne_lpp.h"


#define LORAWAN

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
int main(void)
{
	struct CayenneLPP *lpp;
	unsigned char *buf;
	int w, size;

	// init cayenne lpp
	lpp = CayenneLPP__create(51);

	/*
    //interrupts:
	//uart_enable_interrupts(USART4, UART_INT_RX);
	// Unmask receive interrupt
	uart_enable_rx_interrupt(UART4);
	// Make sure the interrupt is routed through the NVIC
	nvic_enable_irq(NVIC_UART4_RX);
	*/
	
	//pomocna promenna pro prepocitavani ascii znaku na hexadecimalni substringy
	//char hex_string[3];
	//do techto stringu se zkonvertuji nactene hodnoty
	char hum_str [10];
	char temp_str[10];
	char pres_str[10];
	char pm1_str [10];
	char pm2_5_str [10];
	char pm10_str [10];
	
	
	clock_setup();
	gpio_setup();
	usart_setup();
	i2c_setup();
	spi_setup();
	init_BME280();
	
	
	//Quectel wireless modul HW reset
	gpio_clear(GPIOA, GPIO9); 
	wait(SEC*0.2); 
	gpio_set(GPIOA, GPIO9);
	
	wait(SEC*5);//until quectel wakes up
	//wait(1);//until quectel wakes up

	//tohle udela debugovaci spike, je to tady kvuli logicke sondy, v produkci dat pryc
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
	gpio_set(GPIOA, GPIO8); //SS Log 0
	wait(0.2); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?

	wait(SEC*5);
  
	//Connect to network
	#ifdef LORAWAN
	connect_lorawan();
    #endif
    
    #ifdef NBIOT
    connect_nbiot();
    #endif
	

	particlemeter_ON();

	particlemeter_set_fan(FAN_SPEED);

	
    flash(3);
	while (1){

		read_pm_values();
		data_readout_BME280(burst_read_data);

		float hum  = hum_BME280();
		float temp = temp_BME280();
		float press = press_BME280();
		float pm1  = particlemeter_pm1();
		float pm2_5 = particlemeter_pm2_5();
		float pm10 = particlemeter_pm10();

		//prenasobi vsechno, ..?
		///hum  *= 100;
		///temp *= 100;
		///pres *= 100;
		///pm1  *= 100;
		///pm25 *= 100;
		///pm10 *= 100;

		//prelozi na stringy
		

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
		
		wait(SEC *5);
		
		
		///LORAWAN
		//navrh na organizaci paketu:
		// -> nene, hezky encodovat do lpp
		/*
		nazev			kanal		typ		size[B]		poznamka					encoded:
		cislo_senosoru	0x01		00		1			cislo mericiho boxu			010001
		verze_fw		0x02		00		1			verze fw					020010
		teplota			0x03		67		2			teplota						03670110
		tlak			0x04		73		2			tlak						04730220
		vlhkost			0x05		68		1			vlhkost						056830
		pm1				0x06		02		2			analog input? prozatim		06022332
		pm2_5			0x07		02		2			analog input? prozatim		07022345
		pm10			0x08		02		2			analog input? prozatim		08024939
		gps				0x09		88		9			gps							098806765ff2960A0003E8
	
		*/
		/*
		uint8_t buffer[100];
		static char cislo_sensoru[] = "010001";
		static char verze_fw[] = "010001";
		
		strcat(buffer, cislo_sensoru
		*/
		
		/*
		 * 	Data 	Channel 	Type 				Value
			03 ⇒ 	3 			67 ⇒ Temperature 	0110 = 272 ⇒ 27.2°C
			05 ⇒ 	5 			67 ⇒ Temperature 	00FF = 255 ⇒ 25.5°C
		 */
		 
		//hardcoded teoreticke hodnoty:  0100010200100367011004730220056830060223320702234508024939098806765ff2960A0003E8


		// mame tyto senzory
		// teplota,  tlak,  vlhkost,  pm1,  pm2_5, pm10, gps

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

		usartSend("mac tx uncnf 1 ", 4);
		for (w = 0; w < size; ++w) {
			usartSend(buf[w], 4);
		}
		usartSend("\r\n", 4);
		
		wait(SEC *10);
		
		
	}
	return 0;
}


/*		DEBUG SERIAL SEQUENCE
 
		usartSend("MEASURED VALUES:\r\n", 4);
		//temp
		sprintf(temp_str, "%.2f", temp);
		usartSend("temp \t[°C]: \t\t", 4);
		usartSend(temp_str, 4);
		usartSend("\r\n", 4);

		//press
		sprintf(pres_str, "%.2f", pres);
		usartSend("pres \t[XX]: \t\t", 4);
		usartSend(pres_str, 4);
		usartSend("\r\n", 4);


		//hum
		sprintf(hum_str, "%.2f", hum);
		usartSend("hum \t[XX]: \t\t", 4);
		usartSend(hum_str, 4);
		usartSend("\r\n", 4);

		//pm1
		sprintf(pm1_str, "%.2f", pm1);
		usartSend("pm 1 \t[ug/m3]: \t", 4);
		usartSend(pm1_str, 4);
		usartSend("\r\n", 4);


		//pm1
		sprintf(pm2_5_str, "%.2f", pm2_5);
		usartSend("pm 2.5 \t[ug/m3]: \t", 4);
		usartSend(pm2_5_str, 4);
		usartSend("\r\n", 4);
		
		
		//pm1
		sprintf(pm10_str, "%.2f", pm10);
		usartSend("pm 10 \t[ug/m3]: \t", 4);
		usartSend(pm10_str, 4);
		usartSend("\r\n", 4);	
			usartSend("\r\n", 4);
 
  
 */




