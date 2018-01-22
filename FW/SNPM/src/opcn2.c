
#include "opcn2.h"


void particlemeter_set_fan(uint8_t speed)
{
	wait(DELAY);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	wait(DELAY_1);
	  
	spi_send(SPI1, 0x42);
	wait(DELAY);
	spi_send(SPI1, 0x00);//fan command
	wait(DELAY);
	spi_send(SPI1, speed);
	wait(DELAY);
	gpio_set(GPIOA, GPIO8); //SS Log 1
}

//read_serial_number
//read_config_values
//read_config_values_2
void read_pm_values(void)
{

	wait(DELAY);
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	
	wait(DELAY_1);
	 for(int i=0; i<12;i++)
	{
		wait(DELAY);
		spi_send(SPI1, 0x32);
		pm_values_buffer[i] = spi_read(SPI1);
	} 
	
	gpio_set(GPIOA, GPIO8); //SS Log 1
}

void read_histogram_all(void)
{

	wait(DELAY);
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
double particemeter_pm1(void)
{
	double pm1 = calculate_float(pm_values_buffer[3], pm_values_buffer[2], pm_values_buffer[1] , pm_values_buffer[0]);
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


void particlemeter_ON(void)
{
		  
	  gpio_clear(GPIOA, GPIO8); //SS Log 0
	  wait(1); //FIXME 1 sec je na hrane, pro produkci pak dat klidne vice! u vsech funkci?
	  
	  //fan and laser power ON
	  spi_send(SPI1, 0x03);
	  wait(DELAY);
	  spi_send(SPI1, 0x00);

	  gpio_set(GPIOA, GPIO8); //SS Log 1	
}

void particlemeter_read(void)
{
	//read registers
;	
}
