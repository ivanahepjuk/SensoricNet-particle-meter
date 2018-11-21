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
#include <stdlib.h>
#include "../main.h"

//#define SPI_CR1_DFF_8BIT  	(0 << 11)

// set some constants, fixme


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

void flash(uint8_t loop, uint32_t delay)
{
	for (uint8_t i = 0; i < loop; i++)
	{
		gpio_set(LED1_GPIO_GROUP, LED1_GPIO);
		wait(delay);
		gpio_clear(LED1_GPIO_GROUP, LED1_GPIO);
		wait(delay);
	}
}

/**
 * zablika danou led x-krat s danou periodou
 */
void led_flash(uint8_t led, uint8_t loop, uint32_t delay)
{
	for (uint8_t i = 0; i < loop; i++)
	{
		led_on(led);
		wait(delay);
		led_off(led);
		wait(delay);
	}
}

/**
 * zapne danou ledku
 */
void led_on(uint8_t led)
{
	if (led==1) 
		gpio_set(LED1_GPIO_GROUP, LED1_GPIO);

	if (led==2) 
		gpio_set(LED2_GPIO_GROUP, LED2_GPIO);

	if (led==3) 
		gpio_set(LED3_GPIO_GROUP, LED3_GPIO);
	
}

/**
 * vypne danou ledku
 */
void led_off(uint8_t led)
{
	if (led==1)
		gpio_clear(LED1_GPIO_GROUP, LED1_GPIO);
	if (led==2)
		gpio_clear(LED2_GPIO_GROUP, LED2_GPIO);
	if (led==3)
		gpio_clear(LED3_GPIO_GROUP, LED3_GPIO);
}

/* 		void spi_setup(void)
 * 
 * This functions does setting-up of SPI peripheral. 
 * (1) Configuring GPIO's
 * (2) Configuring SPI block
 * (3) Enabling spi
 * 
 */
void spi_setup(void)
{
	// gpio setting for SS
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
	
	// gpio setting for SDI SDO SCK
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,  GPIO3); //gpio3 sck 
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,  GPIO3); //gpio4 pm-sdo (MISO)
	gpio_mode_setup(GPIOB, GPIO_MODE_INPUT,  GPIO_PUPD_NONE,  GPIO5); //gpio5  pm-sdi (MOSI) 
	//gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, GPIO5);

	// gpio alternative function SPI 1
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3  | GPIO4 | GPIO5);
	gpio_set_af(GPIOB, GPIO_AF0, GPIO3  | GPIO4 | GPIO5);
	//Reset SPI
	spi_reset(SPI1);

	/* Set up SPI in Master mode with:
	* Clock baud rate: 1/32? of peripheral clock frequency
	* Clock polarity: Idle High
	* Clock phase: Data valid on 2nd clock pulse
	* Data frame format: 8-bit
	* Frame format: MSB First
	* Setting NSS high (Even though I am using gpio as SS)
	*/
	
	//bitstream for register settinggs according to datasheet
	SPI_CR1(SPI1) |= 0b0000001100100101; //100101//4101 //7011  
	//bitstream for register settinggs according to datasheet
	SPI_CR2(SPI1) |= 0b0000011100000000; 
	 
	//this is super important for reliable reading 8-bit packets from bus   
	spi_fifo_reception_threshold_8bit(SPI1);

	//Enable SPI1 peripherals
	spi_enable(SPI1);
	
}
  
	/*
	while(j<10){
		if (dev_id[j] < 10) {
			c = dev_id[j] + '0';
		} else {
			c = (dev_id[j] - 10) + 'A';
		}
		   
		cmd_w[2*j+1] = c / 0x10;
		cmd_w[2*j+2] = c % 0x10;
		
		j++;
	}
	for(int k=0; k<21; k++){
		usart_send_blocking(USART2, cmd_w[k]);	
	}
*/


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

void wait(uint32_t usec)
{
	for (uint32_t i=0; i<usec; i++)
	{
		__asm__("NOP");
	}		
}

/**
 * void usartSend(char *phrase, uint8_t usart)
 * 
 * This functions sends string to one of four usarts.
 * Note: chosen usart must be allready configured in usart_setup! 
 */
void usartSend(char *phrase, uint8_t usart)
{

	uint32_t i=0;

	while(phrase[i] != '\0')
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

void debug_usart_send(char *phrase)
{
	char debug_txt[] = "DEBUG: ";
	uint32_t i=0;

	while(debug_txt[i] != '\0')
	{
		usart_send_blocking(DEBUG_USART, debug_txt[i]);
		i++;
	}

	i=0;
	while(phrase[i] != '\0')
	{
		usart_send_blocking(DEBUG_USART, phrase[i]);
		i++;
	}

	usart_send_blocking(DEBUG_USART, '\r');
	usart_send_blocking(DEBUG_USART, '\n');
}


void clock_setup(void)
{
	//clk for gsm, leds, FIXME
	rcc_periph_clock_enable(RCC_GPIOA);

	//clk for spi and FIXME i2c
	rcc_periph_clock_enable(RCC_GPIOB);
	
	// Enable GPIOC clock for LED
	rcc_periph_clock_enable(RCC_GPIOC);

	// Enable GPIOC clock for LED
	rcc_periph_clock_enable(RCC_GPIOD);
	
	//clk for SPI1
	rcc_periph_clock_enable(RCC_SPI1);
	
	//clk for USART4 (IoT module), PC10 tx, PC11 rx
	rcc_periph_clock_enable(RCC_USART4);

	//clk for USART2 (GPS module), PA2 tx, PA3 rx
	rcc_periph_clock_enable(RCC_USART2);

	//clk for USART1 (DEBUG), PB6 tx, PB7 rx
	rcc_periph_clock_enable(RCC_USART1);

}


void i2c_setup(void)
{
	//clocks
	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(RCC_GPIOB);
	
	i2c_reset(I2C2);
	
	// gpio setting for data and clock
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO13 | GPIO14);
	// gpio setting to alternative function I2C2
	gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14);
	//disable i2c2 before configuration
	i2c_peripheral_disable(I2C2);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(I2C2);
	//digital filter
	//i2c_set_digital_filter(I2C1, I2C_CR1_DNF_DISABLED);
	//speed
	i2c_set_speed(I2C2, i2c_speed_sm_100k, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	i2c_enable_stretching(I2C2);
	//addressing mode
	i2c_set_7bit_addr_mode(I2C2);
	//After setting it up, it must be enabled.
	i2c_peripheral_enable(I2C2);
}


void usart_setup(void)
{
	// setup GPS module USART2 parameters
	nvic_enable_irq(NVIC_USART2_IRQ);
	usart_set_baudrate(USART2, 9600);
	usart_set_databits(USART2, 8);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	usart_enable_rx_interrupt(USART2);
	usart_enable(USART2);

	// setup quectel(gsm)/lora USART4 parameters
	usart_set_baudrate(USART4, USART_BAUDRATE);  //lora 57600 quectel 9600
	usart_set_databits(USART4, 8);
	usart_set_parity(USART4, USART_PARITY_NONE);
	usart_set_stopbits(USART4, USART_STOPBITS_1);
	usart_set_mode(USART4, USART_MODE_TX_RX);
	usart_set_flow_control(USART4, USART_FLOWCONTROL_NONE);
	usart_enable(USART4);

	// setup debug USART1 parameters
	usart_set_baudrate(DEBUG_USART, DEBUG_BAUDRATE); //default 57600
	usart_set_databits(DEBUG_USART, 8);
	usart_set_parity(DEBUG_USART, USART_PARITY_NONE);
	usart_set_stopbits(DEBUG_USART, USART_STOPBITS_1);
	usart_set_mode(DEBUG_USART, USART_MODE_TX_RX);
	usart_set_flow_control(DEBUG_USART, USART_FLOWCONTROL_NONE);
	usart_enable(DEBUG_USART);

}


void gpio_setup(void)
{

	//gpio LEDs setup
	gpio_mode_setup(LED1_GPIO_GROUP, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_GPIO);
	gpio_mode_setup(LED2_GPIO_GROUP, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_GPIO);
	gpio_mode_setup(LED3_GPIO_GROUP, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_GPIO);
	
	//iot module reset
	gpio_mode_setup(IOT_RESET_GPIO_GROUP, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, IOT_RESET_GPIO);

	//gps standby pin
	//gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO0);
	//gpio_set(GPIOA, GPIO0);


	// USART1 DEBUG
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO6); //tx
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO7); //rx

	// USART1 setup pins as alternate function AF0
	gpio_set_af(GPIOB, GPIO_AF0, GPIO6);
	gpio_set_af(GPIOB, GPIO_AF0, GPIO7);

	// USART2 GPS
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO2); //tx
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3); //rx

	// USART2 setup pins as alternate function AF1
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO3);

	// USART4 iot module
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO10); //tx
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO11); //rx

	// USART4 setup pins as alternate function AF0
	gpio_set_af(GPIOC, GPIO_AF0, GPIO10);
	gpio_set_af(GPIOC, GPIO_AF0, GPIO11);


}

/*
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
*/

//char* string_to_hex(unsigned char *string, int len)
//{
//	char *result = malloc(len*2+1);
//	void* pointer = result;
////	char hex[3];
//	int x;
//
//	for (x = 0; x < len; ++x) {
//		sprintf(pointer, "%02x", *string);
//
////		sprintf(hex, "%02x", *string);
////		printf("hex: %s ", hex);
//
//		string++;
//		pointer++;
//		pointer++;
//	}

//	printf("done\n");
//
//	return result;
//}




// gps dava souradnice 5133.82,N,00042.24,W
//                     SSMM.mm   SSSMM.mm
// uhlova minuta je 1/60 stupne
// WGS84
// 49.9346397,17.89627
// 5133.82,N = 51.(33.82/60) = 51.563666667N (N je +, S je -)
// 00042.24,W = 0.(42.24/60) = 0.704W (E je +, W je -???)

float convert_gps_to_wgs84_latitude (char *gps_latitude_string) {

	char degree[2] = {'0'};
	char minutes[5] = {'0'};
	float wgs_latitude_float = 0;

	if (gps_latitude_string[0] != 0) {

		//odloz stupne
		degree[0] = gps_latitude_string[0];
		degree[1] = gps_latitude_string[1];

		//zjisti minuty
		minutes[0] = gps_latitude_string[2];
		minutes[1] = gps_latitude_string[3];
		minutes[2] = gps_latitude_string[4];
		minutes[3] = gps_latitude_string[5];
		minutes[4] = gps_latitude_string[6];

		// mozna to pujde udelat chytreji - rovnou zkonvertovat string na float a prepocitat
		// atoi neni zcela bezpecna fce, TODO
		wgs_latitude_float = atoi(degree) + ((float)atof(minutes) / 60);

		// jestli je latitude S, obrat znamenko
		if (gps_latitude_ns[0] == 'S') {
			wgs_latitude_float = -wgs_latitude_float;
		}

		return (wgs_latitude_float);

	} else {
		// invalid hodnota - treba -500? TODO
		return -500.0;
	}
}

float convert_gps_to_wgs84_longitude (char *gps_longitude_string) {

	char degree[3] = {'0'};
	char minutes[5] = {'0'};
	float wgs_longitude_float = 0;

	if (gps_longitude_string[0] != 0) {

		//odloz stupne
		degree[0] = gps_longitude_string[0];
		degree[1] = gps_longitude_string[1];
		degree[2] = gps_longitude_string[2];

		//zjisti minuty
		minutes[0] = gps_longitude_string[3];
		minutes[1] = gps_longitude_string[4];
		minutes[2] = gps_longitude_string[5];
		minutes[3] = gps_longitude_string[6];
		minutes[4] = gps_longitude_string[7];

		// mozna to pujde udelat chytreji - rovnou zkonvertovat string na float a prepocitat
		// atoi neni zcela bezpecna fce, TODO
		wgs_longitude_float = atoi(degree) + ((float)atof(minutes) / 60);

		// jestli je longitude W, obrat znamenko
		if (gps_longitude_ew[0] == 'W') {
			wgs_longitude_float = -wgs_longitude_float;
		}

		return (wgs_longitude_float);

	} else {
		// invalid hodnota - treba -500? TODO
		return -500.0;
	}
}


void get_nth_substring(unsigned int number, char separator, char* string, unsigned int string_size, char* buffer, unsigned int buffer_size)
{
	if (!string || !buffer || buffer_size<1)
		return; //bad input

	unsigned int w = 0;
	unsigned int q = 0;
	unsigned int count = 0;
	while (w < string_size && number > 0) {
		if (string[w] == separator) {
			count++;
			if (count == number) {
				w++;
				break;
			}
		}
		w++;
	}
	while (w < string_size) {
		if (string[w] != separator && string[w] != '\0') {
			buffer[q] = string[w];
			w++;
			q++;
		} else {
			break;
		}
	}

	buffer[q+1] = '\0';
}



void usart2_isr(void)
{
	static char data = 'A';

	led_on(3);

	//Check if we were called because of RXNE.
	// dron: jake jine preruseni by mohlo prijit? imho vzdy to bude od RXNE, ale ok, je to safe
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) && ((USART_ISR(USART2) & USART_ISR_RXNE) != 0)) {

	
//		while((USART_ISR(USART2) & USART_ISR_RXNE) != 0){
		// dron: tento check ma delat co? imho je preruseni az po precteni celeho znaku, ne?
			data = (char) usart_recv(USART2);

			if (data == '$') {
				// zacatek radku
				gps_rx_buffer_pointer = 0;
			}

			gps_rx_buffer[gps_rx_buffer_pointer] = data;
			gps_rx_buffer_pointer++;

			if (data == '\n') {
				// konec radku, rovnou ho rozparsuj
				gps_rx_buffer[gps_rx_buffer_pointer] = '\0';

				// radek GPGGA obsahuje vsechny uzitecne hodnoty

				//	$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
				// 1    = UTC of Position
				// 2    = Latitude
				// 3    = N or S
				// 4    = Longitude
				// 5    = E or W
				// 6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
				// 7    = Number of satellites in use [not those in view]
				// 8    = Horizontal dilution of position
				// 9    = Antenna altitude above/below mean sea level (geoid)
				// 10   = Meters  (Antenna height unit)
				// 11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and
				//        mean sea level.  -=geoid is below WGS-84 ellipsoid)
				// 12   = Meters  (Units of geoidal separation)
				// 13   = Age in seconds since last update from diff. reference station
				// 14   = Diff. reference station ID#
				// 15   = Checksum

				if (strstr(gps_rx_buffer, "GPGGA") != NULL) {
					// UTC time
					get_nth_substring(1, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_utc_time, sizeof(gps_utc_time));
					// Latitude
					get_nth_substring(2, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_latitude, sizeof(gps_latitude));
					// N or S
					get_nth_substring(3, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_latitude_ns, sizeof(gps_latitude_ns));
					// Longitude
					get_nth_substring(4, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_longitude, sizeof(gps_longitude));
					// E or W
					get_nth_substring(5, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_longitude_ew, sizeof(gps_longitude_ew));
					// GPS quality indicator
					get_nth_substring(6, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_quality_indicator, sizeof(gps_quality_indicator));
					// Altitude
					get_nth_substring(9, ',', gps_rx_buffer, sizeof(gps_rx_buffer), gps_altitude, sizeof(gps_altitude));
				}
			}

//		}

		// USART Interrupt Flag Clear Register slouzi je read write a pokud mi behem prenosu nastavi ten procak
		// do Interrupt STATS register nejaky status bit, tenhle status bit je mozne vynulovat pouze zapsanim do 
		// tohohle interupt flag clear status register, protoze ten ISR je read only, anobrz ICRT je read write.
		// Trochu to haprovalo kdyz sem tam sazel znaky moc rychle, tak se to zaseklo - protoze nastavil 
		// nekde nejaky bit. Rozhodl sem se v tomto mioste pro jistotu vynulvat cely ICR, proto je na dalsim radku
		// takove ORove peklo:)

		// dron: teda v datasheetu sem se k tomu jeste nedocetl, ale cekal bych, ze rucne neni treba interupt flagy nulovat. nebo se pletu?

//		USART_ICR(USART2) |= (	USART_ICR_FECF   |  USART_ICR_PECF |
//								USART_ICR_NCF    | USART_ICR_ORECF |
//								USART_ICR_IDLECF | USART_ICR_TCCF  |
//								USART_ICR_LBDCF  | USART_ICR_CTSCF |
//								USART_ICR_RTOCF  | USART_ICR_EOBCF |
//								USART_ICR_CMCF   | USART_ICR_WUCF
//							);
	
		//flush neprectene data, 
		//USART_RQR(USART2) |= USART_RQR_RXFRQ;

	}
	led_off(3);
}

void sys_tick_handler(void)
{
/**
 * zablika danou led x-krat s danou periodou
 */
//led_flash(3, 2, 5000);

//Application interrupt and reset control register (AIRCR):
//mww 0xe000ed0c 0x05fa0004




//	gpio_toggle(GPIOC, GPIO8);
}
//static 
void systick_setup(int xms)
{
	/* div8 per ST, stays compatible with M3/M4 parts, well done ST */
	systick_set_clocksource(STK_CSR_CLKSOURCE_EXT);
	/* clear counter so it starts right away */
	STK_CVR = 0;

	systick_set_reload(rcc_ahb_frequency / 8 / 1000 * xms);
	systick_counter_enable();
	systick_interrupt_enable();
}


