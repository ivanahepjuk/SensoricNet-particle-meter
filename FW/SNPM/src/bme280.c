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

void init_BME280(void)
{
	uint8_t cmd_w[2] = {0xF2, 0x07}; //CTRL_HUM, 00000111
	uint8_t data[8] = {0};
	
	//tohle zapise na eeprom adresu cmd_w[0] hodnoty co jsou dale v tom poli
	i2c_transfer7(I2C2, BME, cmd_w, 2, data, 0);
	/* //check	
	cmd_w [0]= 0xF2;
	i2c_transfer7(I2C2, BME, &cmd_w, 1, &data, 1);
	*/
	//nastaveni ctrl_meas
	cmd_w[0] = 0xF4;
        cmd_w[1] = 0x6F;  //011 011 11
	i2c_transfer7(I2C2, BME, cmd_w, 2, data, 0); 

	//nastaveni ctrl_meas
	cmd_w[0] = 0xF5;
        cmd_w[1] = 0x80;  //011 011 11
	i2c_transfer7(I2C2, BME, cmd_w, 2, data, 0);


	//vycte kompenzacni data
	compensation_data_readout_BME280(comp_data);	
}

void compensation_data_readout_BME280(uint8_t arrayy[])
{
	wait(0.001);
	i2c_set_7bit_address(I2C2, BME);
	i2c_set_write_transfer_dir(I2C2);
	i2c_set_bytes_to_transfer(I2C2, 1); //pocet bajtu	
	i2c_enable_autoend(I2C2);
	
	i2c_send_start(I2C2);
	i2c_send_data(I2C2,0x88);
	
	wait(0.001);

	//cteni		
	i2c_set_read_transfer_dir(I2C2);
	i2c_set_bytes_to_transfer(I2C2, 34);
	/* start transfer */
	i2c_send_start(I2C2);
	/* important to do it afterwards to do a proper repeated start! */
	i2c_enable_autoend(I2C2);

	for (size_t i = 0; i < 34; i++) {
		while (i2c_received_data(I2C2) == 0);
		arrayy[i] = i2c_get_data(I2C2);
	}

///////globals:)
dig_T1 = (arrayy[1] << 8) | arrayy[0];
dig_T2 = (arrayy[3] << 8) | arrayy[2];
dig_T2 = (arrayy[5] << 8) | arrayy[4];
dig_P1 = (arrayy[7] << 8) | arrayy[6];
dig_P2 = (arrayy[9] << 8) | arrayy[8];
dig_P3 = (arrayy[11] << 8) | arrayy[10];
dig_P4 = (arrayy[13] << 8) | arrayy[12];
dig_P5 = (arrayy[15] << 8) | arrayy[14];
dig_P6 = (arrayy[17] << 8) | arrayy[16];
dig_P7 = (arrayy[19] << 8) | arrayy[18];
dig_P8 = (arrayy[21] << 8) | arrayy[20];
dig_P9 = (arrayy[23] << 8) | arrayy[22];
dig_H1 = arrayy[24];
dig_H2 = (arrayy[26] << 8) | arrayy[25];
dig_H3 = arrayy[27];
dig_H4 = (arrayy[29] << 8) | arrayy[28];
dig_H5 = (arrayy[31] << 8) | arrayy[30];
dig_H6 = arrayy[32];


}

void data_readout_BME280(uint8_t array[])
{
	size_t number_of_bytes = 8;
	uint8_t addr = BME;
	uint8_t cmd_w = 0xF7;
	//uint8_t burst_data[8] = {0};
	i2c_transfer7(I2C2, addr, &cmd_w, 1, array, number_of_bytes);
}



float temp_BME280(void) //!!!!!!!!!!! before calling this, data_readout and _compensation_data_readout must by called !!!!!!!!!!
{

uint32_t temp_raw;
 
    float tempf;

    temp_raw = (burst_read_data[3] << 12) | (burst_read_data[4] << 4) | (burst_read_data[5] >> 4);

    int32_t temp;

    temp =
        (((((temp_raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11) +
        ((((((temp_raw >> 4) - dig_T1) * ((temp_raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14);

    t_fine = temp;
    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp;

    return (tempf/100.0f);
}

float press_BME280(void)
{
	uint32_t press_raw;
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

float hum_BME280(void)
{
    uint32_t hum_raw;
    float humf;
     
    hum_raw = (burst_read_data[6] << 8) | burst_read_data[7];
 
    int32_t v_x1;
 
    v_x1 = t_fine - 76800;
    v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
               ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                            (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                            (int32_t)dig_H2 + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
 
    humf = (float)(v_x1 >> 12);
 
    return (humf/1024.0f);
}
