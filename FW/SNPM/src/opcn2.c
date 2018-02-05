
#include "opcn2.h"
#include "functions.h"

typedef uint8_t byte;

uint8_t spi_xferr(uint32_t spi, uint8_t data)
 {
         spi_send8(spi, data);
 
         /* Wait for transfer finished. */
         while (!(SPI_SR(spi) & SPI_SR_RXNE));
 
         /* Read the data (8 or 16 bits, depending on DFF bit) from DR. */
         return SPI_DR(spi);
 }
 


uint8_t pm_set_command(uint8_t command_byte, uint32_t delay)
{
	uint8_t incomming;
	/*
	incomming = spi_xferr(SPI1, command_byte);
	cekej(delay);
	usart_send_blocking(USART4, incomming);
	*/	
	spi_send8(SPI1, command_byte);
	//cekej(1000);
	//steals incomming data directly from shift register
	incomming = spi_read8(SPI1);//(uint8_t)SPI_DR(SPI1);
	usart_send_blocking(USART4, incomming);
	cekej(delay);
	
	return incomming;
}

void pm_SS_on(void)
{
	gpio_clear(GPIOA, GPIO8); //SS Log 0
	cekej(100000); //1s
}

void pm_SS_off(void)
{	cekej(10000); //100ms
	gpio_set(GPIOA, GPIO8); //SS Log 1
	cekej(50000); //100ms
}

void pm_SS_toggle(uint32_t delay)
{
	cekej(delay); //0.1s
	gpio_set(GPIOA, GPIO8); //SS Log 0
	cekej(delay); //0.1s
	gpio_clear(GPIOA, GPIO8); //SS Log 1
	cekej(10000); //1s
}


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
	pm_SS_on();
	while(pm_set_command(0x03, 14000) != 243)       {;}
	cekej(14000);
	while( pm_set_command(0x00, 14000) != 0x03){;}
	pm_SS_off();
	
}

void particlemeter_set_fan(uint8_t speed)
{
	pm_SS_on();
	while(pm_set_command(0x42, 14000) == 0xf3)       {;}
	cekej(14000);
	while( pm_set_command(0x00, 14000) == 0x42){;}
	cekej(14000);
	while( pm_set_command(speed, 14000) == 0x00){;}
	pm_SS_off();
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

#define SPI_DR8(spi_base) 	   MMIO8((spi_base) + 0x0c)



void read_pm_values(void)
{
	
	pm_SS_on();
	//while(pm_set_command(0x32, 14000) == 0xf3)       {;}
	pm_set_command(0x32, 14000);
	//cekej(14000);
	pm_SS_toggle(20000);
	cekej(10000);
	
	for(uint8_t i = 0; i<12; i++)
	{
		spi_send8(SPI1, 0x32);
		cekej(20000);
		//steals incomming data directly from shift register
		pm_values_buffer[i] = spi_read8(SPI1);

		
			
	}
	for (int i=0; i<12; i++)
	{
		usart_send_blocking(USART4, pm_values_buffer[i]);			

	}
		usartSend("\r\n", 4);	
	
	pm_SS_off();
}

//read_pm_data
float particlemeter_pm1(void)
{
//	pm1 = calculate_float(122,70, 86, 66);//53.568825
	float pm1 = calculate_float(pm_values_buffer[0], pm_values_buffer[1] , pm_values_buffer[2], pm_values_buffer[3]);
	//pm1 = calculate_float(0xBC, 0x43, 0xE5, 0x6C);//2217311033871905637061885952.000000
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
