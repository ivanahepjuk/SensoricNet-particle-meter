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
#define LORAWAN


// predpoklada se naplneni techto promennych z cmdline
#ifndef LORAWAN_DEVEUI
#define LORAWAN_DEVEUI "0011223344556677"
#endif

#ifndef LORAWAN_APPEUI
#define LORAWAN_APPEUI "0011223344556677"
#endif

#ifndef LORAWAN_APPKEY
#define LORAWAN_APPKEY "00112233445566778899aabbccddeeff"
#endif

// dulezite pouze pro nbiot
//#define DEV_ID "nbiot-0001"

#ifdef LORAWAN
#define USART_BAUDRATE 57600
#define LORA_USART (USART4)
#endif

#ifdef NBIOT
#define USART_BAUDRATE 9600
#endif


#endif
