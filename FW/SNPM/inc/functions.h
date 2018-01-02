#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <string.h>

#define EEPROM 0x51
#define RCC_CFGR3_I2C2SW   (1 << 5)

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


void i2c_transfer77(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn, uint8_t *r, size_t rn);
void compensation_data_readout_BME280(uint8_t arrayy[]);

#endif
