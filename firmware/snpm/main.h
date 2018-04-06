#ifndef MAIN_H
#define MAIN_H

#include <libopencm3/stm32/spi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <libopencm3/stm32/nvic.h>
#include "inc/functions.h"
#include "inc/bme280.h"
#include "inc/opcn2.h"
#include "inc/wireless.h"
#include "inc/cayenne_lpp.h"
#include "m24c04.h"

//how long it waits at the end of each loop?
#define WAIT 30

//USART2 debug output, if defined, all USART4 communication is routed also to USART2
#define DEBUG

//Choose your platform here
//#define LORAWAN
#define NBIOT



#ifdef LORAWAN
#define USART_BAUDRATE 57600
#define LORA_USART (USART4)
#endif

#ifdef NBIOT
#define USART_BAUDRATE 9600
#endif


#endif
