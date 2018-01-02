#ifndef HW_FUNCTIONS_H
#define HW_FUNCTIONS_H

//all includes here

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <string.h>



#define debug
#define EEPROM 0x51
#define BME 0x76

#define RCC_CFGR3_I2C2SW   (1 << 5)


extern int32_t     t_fine;
extern uint8_t comp_data[34];//compensation data readed into this
extern uint8_t burst_read_data[8]; //burst read data readed into this

//compensation constants
//temperature
extern uint16_t dig_T1;
extern int16_t dig_T2, dig_T3;
//pressure
extern uint16_t dig_P1;
extern int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
//humidity
extern unsigned char dig_H1, dig_H3;
extern int16_t dig_H2, dig_H4, dig_H5;
extern signed char dig_H6;

//pomocne
void charToHex(char c, char hex[3]);
char hexDigit(unsigned n);

//posle string na usart c. 1,2,3,4
void usartSend(char *phrase, int usart);
void wait(float sec);
void clock_setup(void);
void i2c_setup(void);
void usart_setup(void);
void gpio_setup(void);
void connect_nbiot(void);
void connect_lorawan(void);
int sendCommand(char *phrase, char *check, int pocetentru);


void  init_BME280(void);
float press_BME280(void);
float hum_BME280(void);
float temp_BME280(void);
void data_readout_BME280(uint8_t array[]);

void i2c_transfer77(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn, uint8_t *r, size_t rn);
void compensation_data_readout_BME280(uint8_t arrayy[]);

#endif
