#include "functions.h"

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
	for (int i = 0; i < (sec*50000); i++) {	// wait a bit. 
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
	// Enable GPIOC clock for LED
	rcc_periph_clock_enable(RCC_GPIOC);//quectel 

	// clk for USART4 (quectel)  PC10 tx PC11 rx
	rcc_periph_clock_enable(RCC_USART4);
        // clk for USART2 (gsm) PA2 tx PA3 rx
	rcc_periph_clock_enable(RCC_USART2);
	//lora na serial1
	rcc_periph_clock_enable(RCC_USART1);
}


void i2c_setup(void)
{
	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(RCC_GPIOB);
	//rcc_set_i2c_clock_hsi(I2C2);
	RCC_CFGR3 &= ~(1 << 5);

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
//seral1 lora
	// GPIO pins to alternative mode for USART4 transmit receive. 
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO9);//tx
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO10);//rx

	// Setup USART4 pins as alternate function AF0
	gpio_set_af(GPIOA, GPIO_AF1, GPIO9);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO10);

#ifdef debug
	//nucleo led 
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO5);
#endif
	//gpio LEDs setup
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);
        gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);
	
	//wireless reset
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);

	// USART2 GPIO pins 
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO2);//tx
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO3);//rx

	// USART2 setup pins as alternate function AF0
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO3);

	// GPIO pins to alternative mode for USART4 transmit receive. 
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO10);//tx
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO11);//rx

	// Setup USART4 pins as alternate function AF0
	gpio_set_af(GPIOC, GPIO_AF0, GPIO10);
	gpio_set_af(GPIOC, GPIO_AF0, GPIO11);
}

void connect_lorawan(void)
{
//mac set deveui 0004A30B00222137		
//mac set appeui 70B3D57ED00082D2
//mac set appkey D94AC6F27881D3505F3E595B69472898
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
		if (rn) {
			while (!i2c_transfer_complete(i2c));
		}
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


