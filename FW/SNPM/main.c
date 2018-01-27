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
#include "inc/functions.h"
#include "inc/bme280.h"
#include "inc/opcn2.h"


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
uint8_t pm_values_buffer[12];//only pm data




int main(void)
{
	
  //pomocna promenna pro prepocitavani ascii znaku na hexadecimalni substringy
  //char hex_string[3];
  //do techto stringu se zkonvertuji nactene hodnoty
  //char hum_str [10];
  char temp_str[10];
  //char pres_str[10];
  char pm1_str [10];
  //char pm25_str [10];
  //char pm10_str [10];
  
  //Quectel wireless modul HW reset
  //gpio_clear(GPIOA, GPIO9); wait(0.2); 
  //gpio_clear(GPIOA, GPIO9);
  //wait(14);//until quectel wakes up
//  wait(1);//until quectel wakes up

  clock_setup();
  gpio_setup();
  usart_setup();
  i2c_setup();
  
  spi_setup();

 
  init_BME280();
 
   


compensation_data_readout_BME280(&comp_data);
    
    
  //wait(1);
  //Connect to nbiot network
  //connect_nbiot();
  //connect_lorawan();

	  
	  
//	  wait(1);
//particlemeter_ON();
//particlemeter_set_fan(70); //unstable?


  while (1){
//read_histogram_all();
//read_pm_values();

//data_readout_BME280(&burst_read_data);
/*
for(int i = 0; i<8; i++)
{
	usart_send_blocking(USART4, burst_read_data[i]);
}
* */
//usart_send_blocking(USART4, 21.21);

	// usartSend("\r\ncuus\r\n", 4);  

    //debug bme
   
    //debug gme konec
/*
i2c_send_start(I2C2);
i2c_send_data(I2C2,0xEC); //slave address
i2c_send_data(I2C2,0xD0); //id register address


//i2c_set_read_transfer_dir(I2C2);
i2c_send_start(I2C2);
i2c_send_data(I2C2,0xED); //slave address
uint8_t incomming = i2c_get_data(I2C2); //id register address
i2c_send_stop(I2C2);

usart_send_blocking(USART2, 0x65);

*/

//usartSend("mac get devaddr\r\n", 4);
//wait(0.01);
//usart_send_blocking(USART4, 10);
//wait(0.01);
//usart_send_blocking(USART4, 13);

    //usartSend("mac tx uncnf 1 AABABBB\r\n", 4);	
    //precte vsechno
 //   float hum  = hum_BME280();
    float temp = 21.21;// temp_BME280();
 //   float pres = press_BME280();
    float pm1  = 21.21;//particemeter_pm1();
    float pm25 = 50.50;
    float pm10 = 60.60;

    //prenasobi vsechno
///    hum  *= 100;
///    temp *= 100;
///    pres *= 100;
///    pm1  *= 100;
    pm25 *= 100;
    pm10 *= 100;

    //prelozi na stringy
    

    //send nbiot
    char str_data[210];
    int i = 0;

    char str1[]="AT+NSOST=0,89.103.47.53,8089,";
    char str2[]="hodnoty,box=Adrspach hum=";
    char str3[]=",temp=";
    char str4[]=",pres=";
    char str5[]=",pm1=";
    char str6[]=",pm25=";
    char str7[]=",pm10=";


//temp
	sprintf(temp_str, "%.2f", temp);
	usartSend(temp_str, 4);

//pm1
//	sprintf(pm1_str, "%.2f", pm1);
//	usartSend(pm1_str, 4);
    //vezme retezec ze str2, konvertuje na hexadecimal a pricte k retezci str_data
    /*
    while(str2 != '\0'){
      charToHex(str2[i], hex_string);
      strcat(str_data, hex_string);
      i++;
    }
    */
    //vynuluje iteracni promennou
    i=0;

    //nucleo ledka
    gpio_set(GPIOA, GPIO11); wait(0.2); gpio_clear(GPIOA, GPIO11); 

    wait(1);
  }
  return 0;
}


/*

RN2483 1.0.3 Mar 22 2017 06:00:42
invalid_param
invalid_param
0004A30B00222137
RN2483 1.0.3 Mar 22 2017 06:00:42
radio set pwr
invalid_param
radio set

invalid_param
ge
invalid_param
radio get pwr
1
radio set pwr 14
ok
radio get pwr
14
radio get mod
lora
radio get freq
868100000
radio get sg
invalid_param
radio get sf
sf12
mac set deveui 0004A30B00222137
ok
mac set appeui 70B3D57ED00082D2
ok

invalid_param
mac set appkey D94AC6F27881D3505F3E595B69472898
ok
mac save
ok
mac join otaa
ok
accepted
mac get status
00000401
mac get devaddr
260127E9
mac tx uncnf 1 AABABBB
ok
mac_tx_ok


*/

//double num = 123412341234.123456789; 
		//char output[10];

		//sprintf(output, 10, "%f", 12.2278889);//temp_readout_BME280());



/*
     //686f646e6f74792c626f783d41647273706163682068756d3d
     //hodnoty,box=Adrspach hum=
     //

     char hodnota[20]={0};
     char hexadec[20];
     int x = 0;
     
     //Hum
     float hum = 12.121;//BMEsensor.getHumidity();
     sprintf(hodnota, "%f.2", hum);

     while(hodnota[x] != '\0'){
         charToHex(hodnota[x], hexadec);
         strcat(zprava, hexadec);   
     }
     
     //pricist znaky  ,temp=
     //2c74656d703d
     strcat(zprava, "2c74656d703d");   
     
     //temp
     float temp = 12.121;//BMEsensor.getTemperature();
     sprintf(hodnota, "%f.2", temp);

     while(hodnota[x] != '\0'){
         charToHex(hodnota[x], hexadec);
         strcat(zprava, hexadec);   
     }
     
     //pricist znaky  ,pres=
     //2c707265733d
     strcat(zprava, "2c707265733d");
     
     //press
     float press = 12.121;//BMEsensor.getTemperature();
     sprintf(hodnota, "%f.2", press);

     while(hodnota[x] != '\0'){
         charToHex(hodnota[x], hexadec);
         strcat(zprava, hexadec);   
     }   
     
     //pricist konec  \r\n
     //5c725c6e
     strcat(zprava, "5c725c6e");

////////////////////////////////////////////////////////
*/



