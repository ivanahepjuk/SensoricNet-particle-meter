#ifndef MAIN_H
#define MAIN_H

#include <libopencm3/stm32/spi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libopencm3/stm32/f0/nvic.h>
#include "inc/functions.h"
#include "inc/bme280.h"
#include "inc/opcn2.h"
#include "inc/wireless.h"
#include "inc/cayenne_lpp.h"
#include "inc/iwdg.h"
#include "m24c04.h"


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
 #include <libopencm3/stm32/iwdg.h>

//how long it waits at the end of each loop?
#define WAIT 50

//USART1 is used as debug output, if defined, debug output is send
#define DEBUG
#define DEBUG_USART USART1
#define DEBUG_BAUDRATE 57600

//define which hw we have connected
#define PARTICLEMETER 1

// define macro for device type description
#define NBIOT 1
#define LORAWAN 2

// define pins for peripherals

// leds
#define LED1_GPIO_GROUP GPIOC
#define LED1_GPIO GPIO5
#define LED2_GPIO_GROUP GPIOB
#define LED2_GPIO GPIO1
#define LED3_GPIO_GROUP GPIOB
#define LED3_GPIO GPIO2

// IoT module reset
#define IOT_RESET_GPIO_GROUP GPIOC
#define IOT_RESET_GPIO GPIO12


// u techto promennych se predpoklada jejich naplneni z cmdline

//Choose your platform here
#ifndef DEVICE_TYPE
#define DEVICE_TYPE NBIOT
#endif

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
#ifndef DEV_ID
#define DEV_ID "nbiot-test"
#endif

#if DEVICE_TYPE == LORAWAN
#define USART_BAUDRATE 57600
#define LORA_USART (USART4)
#endif

#if DEVICE_TYPE == NBIOT
#define USART_BAUDRATE 9600
#endif


#endif
