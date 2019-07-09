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

#include <libopencm3/stm32/i2c.h>
#include "bme280.h"
#include "functions.h"		//smazat



void atecWake(void) {

  //rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO14);
  gpio_clear(GPIOB, GPIO14); //SS Log 0
  wait(50);//60us
  gpio_set(GPIOB, GPIO14); //SS Log 1
  wait(1000);//1500 us
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO14);
  
  //i2c_setup();

}


void atec(void) {
  //register ctrl_hum, set oversampling x16
  uint8_t packet[] = {0xc0, 0x77, 0x30, 0xff, 0xbb};
	
  //zapis cmd na i2c adresu BME cmd je v cmd_w
  //i2c_transfer7(I2C2, 0x00, packet, 4, data, 4);

//i2c_write7_v1 (I2C2, 0xc0, packet, 4); 
  i2c_send_start(I2C2);	
  wait(5);
  for (uint8_t i = 0; i< 5; i++) {
    i2c_send_data(I2C2, packet[i]);
    while(i2c_busy(I2C2))
      ;

  }
}




void BME280_init(void)
{
	//register ctrl_hum, set oversampling x16
	uint8_t cmd_w[2] = {0xF2, 0x05};
	uint8_t data[8] = {0};
	
	//zapis cmd na i2c adresu BME cmd je v cmd_w
	i2c_transfer7(I2C2, BME, cmd_w, 2, data, 0);
	
	//register ctrl_meas, temperature oversampling x4, pressure oversampling x4, normal mode
	cmd_w[0] = 0xF4;
	cmd_w[1] = 0x6F;	//011 011 11
	i2c_transfer7(I2C2, BME, cmd_w, 2, data, 0); 

//	//register config, standby 250ms, filter coefficient 8, i2c
//	cmd_w[0] = 0xF5;
//	cmd_w[1] = 0x80;  //011 011 0 0
//	i2c_transfer7(I2C2, BME, cmd_w, 2, data, 0);

	//vycti kompenzacni data
	BME280_compensation_data_readout(comp_data);
}

void BME280_compensation_data_readout(uint8_t array[])
{
	//	calib 00-25: od 0x88 26 bajtu
	uint8_t cmd_w[1] = {0x88};
	i2c_transfer7(I2C2, BME, cmd_w, 1, array, 26);

	dig_T1 = (array[1] << 8) | array[0];
	dig_T2 = (array[3] << 8) | array[2];
	dig_T3 = (array[5] << 8) | array[4];
	dig_P1 = (array[7] << 8) | array[6];
	dig_P2 = (array[9] << 8) | array[8];
	dig_P3 = (array[11] << 8) | array[10];
	dig_P4 = (array[13] << 8) | array[12];
	dig_P5 = (array[15] << 8) | array[14];
	dig_P6 = (array[17] << 8) | array[16];
	dig_P7 = (array[19] << 8) | array[18];
	dig_P8 = (array[21] << 8) | array[20];
	dig_P9 = (array[23] << 8) | array[22];
	dig_H1 = array[25];

	//	calib 26-41: od 0xE1 16 bajtu
	cmd_w[0] = 0xE1;
	i2c_transfer7(I2C2, BME, cmd_w, 1, array, 16);

	dig_H2 = (array[1] << 8) | array[0];
	dig_H3 = array[2];
	dig_H4 = (array[4] & 0x0F) | (array[3] << 4);
	dig_H5 = (array[4] >> 4) | (array[5] << 4);
	dig_H6 = array[7];
}

void BME280_data_readout(uint8_t array[])
{
	// cteni dat - teplota, tlak a vlhkost = 0xF7 - 0xFE
	uint8_t cmd_w[1] = {0xF7};
	
	i2c_transfer7(I2C2, BME, cmd_w, 1, array, 8);
}

/**
 * before calling this, data_readout and _compensation_data_readout must by called
 */
float BME280_temp(void)
{
	int32_t temp_raw;
	int32_t temp;
	float tempf;

	temp_raw = (burst_read_data[3] << 12) | (burst_read_data[4] << 4) | (burst_read_data[5] >> 4);

	temp =
			( ( ( ( (temp_raw >> 3) - ((int32_t)dig_T1 << 1) ) ) * ((int32_t)dig_T2) ) >> 11 ) +
			( ( ( ( ( (temp_raw >> 4) - ((int32_t)dig_T1) ) * ( (temp_raw >> 4) - ((int32_t)dig_T1) ) ) >> 12) * ((int32_t)dig_T3) ) >> 14 );

	t_fine = temp;
	temp = (temp * 5 + 128) >> 8;
	tempf = (float)temp;

	return (tempf/100.0f);
}

float BME280_press(void)
{
	int32_t press_raw;
	float pressf;

	press_raw = (burst_read_data[0] << 12) | (burst_read_data[1] << 4) | (burst_read_data[2] >> 4);

	int32_t var1, var2;
	uint32_t press;

	var1 = (t_fine >> 1) - 64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
	var2 = var2 + ((var1 * dig_P5) << 1);
	var2 = (var2 >> 2) + (dig_P4 << 16);
	var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
	var1 = ((32768 + var1) * dig_P1) >> 15;
	if (var1 == 0) {
		return 0;
	}
	press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
	if(press < 0x80000000) {
		press = (press << 1) / var1;
	} else {
		press = (press / var1) * 2;
	}
	var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
	var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
	press = (press + ((var1 + var2 + dig_P7) >> 4));

	pressf = (float)press;
	return (pressf/100.0f);
}

float BME280_hum(void)
{
	int32_t hum_raw;
	int32_t v_x1;
	float humf;

	hum_raw = (burst_read_data[6] << 8) | burst_read_data[7];

	v_x1 = t_fine - 76800;
	v_x1 =
			( ( ( ( (hum_raw << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1) ) + ((int32_t)16384) ) >> 15) *
			( ( ( ( ( ( (v_x1 * ((int32_t)dig_H6) ) >> 10 ) * ( ( (v_x1 * ((int32_t)dig_H3) ) >> 11 ) + ((int32_t)32768) ) ) >> 10 ) + ((int32_t)2097152) ) *
			((int32_t)dig_H2) + 8192 ) >> 14 ) );
	v_x1 = (v_x1 - ( ( ( ( (v_x1 >> 15) * (v_x1 >> 15) ) >> 7 ) * ((int32_t)dig_H1) ) >> 4 ) );
	v_x1 = (v_x1 < 0) ? 0 : v_x1;
	v_x1 = (v_x1 > 419430400) ? 419430400 : v_x1;

	humf = (v_x1 >> 12);

	return (humf/1024.0);
}
