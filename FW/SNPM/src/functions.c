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
 
#include "functions.h"
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/spi.h>

#define SPI_CR1_DFF_8BIT  	(0 << 11)


//delay
//volatile uint32_t system_millis;

float calculate_float(uint8_t val0, uint8_t val1, uint8_t val2, uint8_t val3)
{
  // Return an IEEE754 float from an array of 4 bytes
  union u_tag {
    uint8_t b[4];
    float val;
  } u;

  u.b[0] = val0;
  u.b[1] = val1;
  u.b[2] = val2;
  u.b[3] = val3;

  return u.val;
}


void spi_setup(void)
{

	// gpio setting for SS
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO8);
	
	// gpio setting for SDI SDO SCK
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,  GPIO3 | GPIO4 | GPIO5);

	// gpio alternative function SPI 1
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3  | GPIO4 | GPIO5);
	//gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO7);

//	spi_disable_crc(SPI1);
	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_32, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_2, (0 << 11)       , SPI_CR1_MSBFIRST);  //#define SPI_CR1_DFF_8BIT  (0 << 11)
	
	//spi_init_master(SPI1, 500000                       , SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_LSBFIRST);
	
	spi_enable_ss_output(SPI1);
//	spi_enable_software_slave_management(SPI1);
//	spi_set_nss_high(SPI1);
//	spi_clear_mode_fault(SPI1);
	spi_enable(SPI1);
;	
}


char hexDigit(unsigned n)
{
    if (n < 10) {
        return n + '0';
    } else {
        return (n - 10) + 'A';
    }
}

void charToHex(char c, char hex[3])
{
    hex[0] = hexDigit(c / 0x10);
    hex[1] = hexDigit(c % 0x10);
    hex[2] = '\0';
}

void wait(float sec)
{
	for (int i = 0; i < (sec*25000); i++) {	// wait a bit. 
		__asm__("NOP");	}
}


void usartSend(char *phrase, int usart)
{
//	char incomming[100] = {0};   //readed string
	int i=0;                    //iteracni promenn

	//posila na linku
	while(phrase[i] != '\0')//posle string
	{
		if(usart==1)
		{
			usart_send_blocking(USART1, phrase[i]);
		}
		if(usart==2)
		{
			usart_send_blocking(USART2, phrase[i]);
		}
		if(usart==3)
		{
			usart_send_blocking(USART3, phrase[i]);
		}
		if(usart==4)
		{
			usart_send_blocking(USART4, phrase[i]);
		}
		i++;
	}
}


void clock_setup(void)
{
    //clk for gsm,leds,
    rcc_periph_clock_enable(RCC_GPIOA);

	//clk for spi and FIXME i2c
    rcc_periph_clock_enable(RCC_GPIOB);
	
	// Enable GPIOC clock for LED
	rcc_periph_clock_enable(RCC_GPIOC);
	
	//clk for SPI1
	rcc_periph_clock_enable(RCC_SPI1);
	
	// clk for USART4 (quectel)  PC10 tx PC11 rx
	rcc_periph_clock_enable(RCC_USART4);

    // clk for USART2 (gsm) PA2 tx PA3 rx
	rcc_periph_clock_enable(RCC_USART2);
	//lora na serial1
	//rcc_periph_clock_enable(RCC_USART1);
}


void i2c_setup(void)
{
	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(RCC_GPIOB);
	//rcc_set_i2c_clock_hsi(I2C2);
	//RCC_CFGR3 &= ~(1 << 5);

	i2c_reset(I2C2);
	
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO13 | GPIO14);
	gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14);
	i2c_peripheral_disable(I2C2);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(I2C2);
	//i2c_set_digital_filter(I2C1, I2C_CR1_DNF_DISABLED);
	/* HSI is at 8Mhz */
	i2c_set_speed(I2C2, i2c_speed_sm_100k, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	i2c_enable_stretching(I2C2);
	//addressing mode
	i2c_set_7bit_addr_mode(I2C2);
	i2c_peripheral_enable(I2C2);
}


void usart_setup(void)
{


	//usart1
	/*
	//lora debug
	usart_set_baudrate(USART1, 57600);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_stopbits(USART1, USART_CR2_STOP_1_0BIT);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	// enable the USART2
	usart_enable(USART1);
	*/
	// setup gsm USART2 parameters
	usart_set_baudrate(USART2, 9600);
	//lora
	//usart_set_baudrate(USART2, 57600);
	usart_set_databits(USART2, 8);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	// enable the USART2
	usart_enable(USART2);

        // setup quectel USART4 parameters
	usart_set_baudrate(USART4, 57600);
	usart_set_databits(USART4, 8);
	usart_set_parity(USART4, USART_PARITY_NONE);
	usart_set_stopbits(USART4, USART_STOPBITS_1);
	usart_set_mode(USART4, USART_MODE_TX_RX);
	usart_set_flow_control(USART4, USART_FLOWCONTROL_NONE);
	// enable the USART4
	usart_enable(USART4);
}	


void gpio_setup(void)
{
	/*
//seral1 lora
	// GPIO pins to alternative mode for USART4 transmit receive. 
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO9);//tx
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO10);//rx

	// Setup USART4 pins as alternate function AF0
	gpio_set_af(GPIOC, GPIO_AF1, GPIO9);
	gpio_set_af(GPIOC, GPIO_AF1, GPIO10);
	
	//wireless reset pin
	*/

#ifdef debug
	//nucleo led 
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO5);
#endif
	//gpio LEDs setup
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);
	
	//wireless reset
	//gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO9);

	// USART2 GPIO pins 
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO2);//tx
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO3);//rx

	// USART2 setup pins as alternate function AF0
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO3);
    
    // USART4 GPIO pins 
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO10);//tx
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO11);//rx

	// USART4 setup pins as alternate function AF0
	gpio_set_af(GPIOC, GPIO_AF0, GPIO10);
	gpio_set_af(GPIOC, GPIO_AF0, GPIO11);
}

void connect_lorawan(void)
{
//mac set deveui 0004A30B00222137		
//mac set appeui 70B3D57ED00082D2
//mac set appkey D94AC6F27881D3505F3E595B69472898
	//usartSend("sys get ver\r\n", 4);
    //wait(1);
	usartSend("radio set pwr 14\r\n", 4);
	wait(3);
	usartSend("mac set deveui 0004A30B00222137\r\n", 4);
	wait(2);
	usartSend("mac set appeui 70B3D57ED00082D2\r\n", 4);
	wait(2);
	usartSend("mac set appkey D94AC6F27881D3505F3E595B69472898\r\n", 4);
	wait(2);
	usartSend("mac save\r\n", 4);
	wait(2);
	usartSend("mac join otaa\r\n", 4);
	wait(15);
}


void connect_nbiot(void)
{
	while(sendCommand("AT+CFUN=1\r\n", "OK", 2))
		wait(3.0);
	while(sendCommand("AT+COPS=1,2,\"23003\"\r\n", "OK", 2))
		wait(0.5);
	while( sendCommand("AT+CGATT?\r\n", "CGATT:1", 4)) //timeout = number of tries	
		wait(0.5);
}

int sendCommand(char *phrase, char *check, int pocetentru)
{
	char incomming[50] = {0};   //readed string
	int i=0;                                              //iteracni promenna
	int enter=0;                                          //detekce znaku CR

	//posila na linku
	while(phrase[i] != '\0'){  //posle string
		usart_send_blocking(USART4, phrase[i]);
		i++;
	}
	i=0;

	//cte z linky dokud neprijme tolik entru kolik ceka
	while(enter < pocetentru){                                 
		incomming[i] = usart_recv_blocking(USART4);
        	if (incomming[i] == '\n')
			enter++;
		i++;
	}

	if ( (strstr(incomming, check)) == NULL) {
		return 1;
	} else {
		return 0;
	}
}


void i2c_transfer77(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn, uint8_t *r, size_t rn)
{
	/*  waiting for busy is unnecessary. read the RM */
	if (wn) {
		i2c_set_7bit_address(i2c, addr);
		i2c_set_write_transfer_dir(i2c);
		i2c_set_bytes_to_transfer(i2c, wn);
		if (rn) {
			i2c_disable_autoend(i2c);
		} else {
			i2c_enable_autoend(i2c);
		}
		i2c_send_start(i2c);

		while (wn--) {
			bool wait = true;
			while (wait) {
				if (i2c_transmit_int_status(i2c)) {
					wait = false;
				}
				while (i2c_nack(i2c)); /* FIXME Some error */
			}
			i2c_send_data(i2c, *w++);
		}
		/* not entirely sure this is really necessary.
		 * RM implies it will stall until it can write out the later bits
		 */
//		if (rn) {
//			while (!i2c_transfer_complete(i2c));
//		}
	}

	if (rn) {
		/* Setting transfer properties */
		i2c_set_7bit_address(i2c, addr);
		i2c_set_read_transfer_dir(i2c);
		i2c_set_bytes_to_transfer(i2c, rn);
		/* start transfer */
		i2c_send_start(i2c);
		/* important to do it afterwards to do a proper repeated start! */
		i2c_enable_autoend(i2c);

		for (size_t i = 0; i < rn; i++) {
			while (i2c_received_data(i2c) == 0);
			r[i] = i2c_get_data(i2c);
		}
	}
}




/*
//problem je v i2c_transfer7 --> if (rn) { ...
void compensation_data_readout_BME280(uint8_t *arrayy)	
{
	uint8_t cmd_readout[2] = {0x88, 0x00};
	i2c_transfer7(I2C2, BME, &cmd_readout, 1, &arrayy, 28);
	//return 1;	
}
*/

/*
#define RCC_CFGR3_I2C2SW   (1 << 5)

void rcc_set_i2c_clock_hsi(uint32_t i2c)  
 {
         if (i2c == I2C1) {
                 RCC_CFGR3 &= ~RCC_CFGR3_I2C1SW;
         }
         if (i2c == I2C2) {
                 RCC_CFGR3 &= ~RCC_CFGR3_I2C2SW;
         }
 }
*/


