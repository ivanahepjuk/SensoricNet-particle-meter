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

int frame_counter = 0;

/* For semihosting on newlib */
//extern void initialise_monitor_handles(void);


//Global variables for burst register reading, bme280
int32_t t_fine;
uint8_t comp_data[26];	//used for readings of compensation data
uint8_t burst_read_data[8] = {0};	//in loop measured data readed into this

//Global variables for compensation functions, bme280:
//temperature
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
//pressure
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
//humidity
uint8_t dig_H1, dig_H3;
int16_t dig_H2, dig_H4, dig_H5;
int8_t dig_H6;



/////////
char ID[11];
char id_decoded[23]={0};
char gps_string[400] = {0};

/////////////////////////////////////////////////////////////
//Global variables for burst register reading, for OPC-N2: //
/////////////////////////////////////////////////////////////

uint8_t histogram_buffer[62];//whole dataset of opc readed into this
uint8_t pm_values_buffer[12] = {0};//only pm data

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
	

	gpio_set(GPIOA, GPIO6|GPIO7);


//usartSend("\r\n$PMTK225,2,3000,18000,72000\r\n", 2);


while (1){

for(uint16_t x = 0; x<280; x++){
	usart_send_blocking(USART4, gps_string[x]);
}

wait(SEC*10);

}

//

//gpio_set(GPIOA, GPIO6);
//wait(SEC*5);


}
/*	
	//   !!!   Uncomment this only if you know what you are doing,   
	//   !!!!  This is used when deploying new devices   !!!!
	//eeprom_write_id("nbiot-0005");
	//reads ID from eeprom
	#if DEVICE_TYPE == NBIOT
		eeprom_read_id();
	#endif
	
	BME280_init();



	struct CayenneLPP *lpp;
	unsigned char *buf;
	int size;

	//gps/lora module HW reset
	gpio_clear(GPIOA, GPIO9);
	wait(SEC*0.5);
	gpio_set(GPIOA, GPIO9);
	wait(SEC*3);

	//Connect to nbiot network
	#if DEVICE_TYPE == NBIOT
		//wait(SEC*15);//until quectel wakes up
		//nbiot_connect();
	#endif

	//Connect to lora network
	#if DEVICE_TYPE == LORAWAN
		//usartSend("DEBUG: rn2483 reset.\r\n", 2);
		lorawan_reset();
		//usartSend("DEBUG: lora connect.\r\n", 2);
		lorawan_connect();
		//usartSend("DEBUG: lora connected.\r\n", 2);
	#endif

nvic_disable_irq(NVIC_USART2_IRQ);
	particlemeter_ON();
	wait(SEC * 1);
	particlemeter_set_fan(FAN_SPEED);
nvic_enable_irq(NVIC_USART2_IRQ);

	// init cayenne lpp
	lpp = CayenneLPP__create(200);

	

	while (1) {
gps_read();
		//usartSend("DEBUG: New loop\r\n", 2);
	
		nvic_disable_irq(NVIC_USART2_IRQ);
		read_pm_values();
		BME280_data_readout(burst_read_data);
		nvic_enable_irq(NVIC_USART2_IRQ);
		
		float temp = BME280_temp();
		float press = BME280_press();
		float hum = BME280_hum();
		float pm1 = particlemeter_pm1();
		float pm2_5 = particlemeter_pm2_5();
		float pm10 = particlemeter_pm10();

		//usartSend("DEBUG: Encode values\r\n", 2);
		//char debug_data_string[150] = {0};
		//sprintf(debug_data_string, "DEBUG: hum: %.2f, temp: %.2f, press: %.2f, pm1: %.2f, pm2_5: %.2f, pm10: %.2f\r\n", hum, temp, press, pm1, pm2_5, pm10);
		//usartSend(debug_data_string, 2);
		
		CayenneLPP__addTemperature(lpp, 1, temp);
		CayenneLPP__addBarometricPressure(lpp, 2, press);
		CayenneLPP__addRelativeHumidity(lpp, 3, hum);
		CayenneLPP__addAnalogInput(lpp, 4, pm1);
		CayenneLPP__addAnalogInput(lpp, 5, pm2_5);
		CayenneLPP__addAnalogInput(lpp, 6, pm10);
		CayenneLPP__addGPS(lpp, 7, 18.3087525, 49.8346883, 1234);

		buf=CayenneLPP__getBuffer(lpp);
		size=CayenneLPP__getSize(lpp);

		// Send it off
		//sendData(CayenneLPP__getBuffer(lpp), CayenneLPP__getSize(lpp));

		//printf("Encoded data size: %i\n", size);

		char hex_string[200] = {0};		//encoded string here
		char send_string[200] = {0};	//string to send here
		
		//ascii-hex encoding is happening here:
		int j = 0;
		char znak[3];
		
		while(j<size){
			charToHex(buf[j], znak);
			strcat(hex_string, znak);
			j++;
		}
		

		#if DEVICE_TYPE == LORAWAN

		//sestaveni stringu pro LORAWAN
		strcat(send_string, "mac tx uncnf 1 ");
		strcat(send_string, hex_string);

		//odeslani stringu, checkuje "ok", pokud nedostane ok tak to zkusi za chvili znova
		while(lorawan_sendCommand(send_string, "ok", 1)) {
			// TODO odstupnovat, nekolik pokusu, reset (nebo aspon connect)
			wait(SEC*3);
		}

		frame_counter++;

		// jednou za 8 frejmu uloz frame counter
		if ((frame_counter & 0x07) == 0) {
			lorawan_mac_save();
		}
		#endif
		
		#if DEVICE_TYPE == NBIOT
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
		while (nbiot_sendCommand("AT+NSOCL=0\r\n", "OK", 2))
		wait(SEC*1);
		#endif
		
		CayenneLPP__reset(lpp);
		//lpp->cursor = NULL;

		cykly++;

		flash(3, 100000);


		//vypise pozice co chci:
				//vypise pozice co chci:
//		for(uint8_t j = 0; j<400; j++)
		//{
	//		usart_send_blocking(USART2, gps_string[j]);
		//}




		wait(SEC *WAIT);
	}
	return 0;

}

*/

