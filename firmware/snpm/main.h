#ifndef MAIN_H
#define MAIN_H

//how long it waits at the end of each loop?
#define WAIT 10

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
