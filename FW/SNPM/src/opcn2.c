
#include "opcn2.h"
//



void read_pm_serial_number(void)
{
uint8_t scrap;
	char buffer[100] = {0};
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	cekej(200000);
	
	spi_send8(SPI1, 0x3F);
	scrap = spi_read8(SPI1);//spi_send8(SPI1, 0x00);  //to empty buffer

	cekej(200000);
	gpio_set(GPIOA, GPIO8); //SS Log 1
	cekej(200000);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	cekej(1500);
	 for(int i=0; i<60;i++)
	{
		cekej(5);
		spi_send8(SPI1, 0x3F);
		//cekej(5);
		buffer[i] = spi_read8(SPI1);
		
	} 
	buffer[60] = NULL;
	//buffer[0] = 'X';
	usartSend(buffer, 4);
	usartSend("\r\n", 4);
	wait(DELAY_1);
	gpio_set(GPIOA, GPIO8); //SS Log 1
}


//

void particlemeter_ON(void)
{
	uint8_t scrap;
		  
	  gpio_clear(GPIOA, GPIO8); //SS Log 0
	  wait(DELAY_1); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
	  
	  //fan and laser power ON
	  spi_send8(SPI1, 0x03);
	  //scrap = spi_read8(SPI1);//spi_send8(SPI1, 0x00);  //to empty buffer
	  wait(DELAY);
	  spi_send8(SPI1, 0x00);
	  scrap = spi_read8(SPI1);//spi_send8(SPI1, 0x00);  //to empty buffer
	  wait(DELAY);
	  //spi_send8(SPI1, 0x00);
	  //wait(DELAY);
	  

	  gpio_set(GPIOA, GPIO8); //SS Log 1	
	  wait(1);
}

void particlemeter_set_fan(uint8_t speed)
{
	//wait(DELAY);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	wait(DELAY_1);
	  
	spi_send8(SPI1, 0x42);
	wait(DELAY);
	spi_send8(SPI1, 0x00);//fan command
	wait(DELAY);
	spi_send8(SPI1, speed);
	wait(DELAY);
	//spi_send8(SPI1, 0x00);  //to empty buffer
		  
	gpio_set(GPIOA, GPIO8); //SS Log 1
}

//read_serial_number
//read_config_values
//read_config_values_2

void cekej(int usec)
{
	int x= 0;
		for (int i=0; i<usec; i++)
		{
			__asm__("NOP");
			}
}


void read_pm_values(void)
{

	uint8_t scrap;
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	wait(DELAY_1);
	
	spi_send8(SPI1, 0x32);
	scrap = spi_read8(SPI1);
	//wait(0.02);
	//gpio_set(GPIOA, GPIO8); //SS Log 1
	//wait(1.2);
	//gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	cekej(5000);
	
	 for(int i=0; i<12;i++)
	{
		cekej(5);
		spi_send8(SPI1, 0x32);
		//cekej(5);
		pm_values_buffer[i] = spi_read8(SPI1);
	} 
	//scrap = spi_read8(SPI1);//spi_send8(SPI1, 0x00);  //to empty buffer

	wait(DELAY);
	gpio_set(GPIOA, GPIO8); //SS Log 1
}

void read_histogram_all(void)
{

	wait(DELAY_1);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	wait(DELAY_1);
	 for(int i=0; i<62;i++)
	{
		wait(DELAY);
		spi_send(SPI1, 0x30);
		histogram_buffer[i] = spi_read(SPI1);
	} 
	
	gpio_set(GPIOA, GPIO8); //SS Log 1
}
//read_pm_data
float particlemeter_pm1(void)
{
	float pm1 = calculate_float(pm_values_buffer[4], pm_values_buffer[5] , pm_values_buffer[6], pm_values_buffer[7]);
	//float pm1 = calculate_float(pm_values_buffer[0], pm_values_buffer[1] , pm_values_buffer[2], pm_values_buffer[3]);
//	float pm1 = calculate_float(pm_values_buffer[3], pm_values_buffer[2] , pm_values_buffer[1], pm_values_buffer[0]);
//	float pm1 = calculate_float(pm_values_buffer[1], pm_values_buffer[2] , pm_values_buffer[3], pm_values_buffer[4]);//	
	return pm1;
}

void particlemeter_set_laser(uint8_t laser)
{
	wait(DELAY);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	wait(DELAY_1);
	  
	spi_send(SPI1, 0x42);
	wait(DELAY);
	spi_send(SPI1, 0x01);//laser command
	wait(DELAY);
	spi_send(SPI1, laser);
	wait(DELAY);
	gpio_set(GPIOA, GPIO8); //SS Log 1
}



void particlemeter_read(void)
{
	//read registers
;	
}
