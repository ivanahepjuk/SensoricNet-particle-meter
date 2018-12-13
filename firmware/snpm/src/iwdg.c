#include "iwdg.h"
//#include <libopencm3/stm32/iwdg.h>

		

/*
1. Enable the IWDG by writing 0x0000 CCCC in the IWDG_KR register.
2. Enable register access by writing 0x0000 5555 in the IWDG_KR register.
3. Write the IWDG prescaler by programming IWDG_PR from 0 to 7.
4. Write the reload register (IWDG_RLR).
5. Wait for the registers to be updated (IWDG_SR = 0x0000 0000).
6. Refresh the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA)
*/

/*
#define IWDG_KR		(*((volatile unsigned int*) 0x40003000))  //		
#define IWDG_PR		(*((volatile unsigned int*) 0x40003004))
#define IWDG_RL		(*((volatile unsigned int*) 0x40003008))		
#define IWDG_SR		(*((volatile unsigned int*) 0x4000300C))
*/

void moje_iwdg_setup(void)
{
(*((volatile unsigned int*) 0x40003000)) = 0x0000CCCC;  //enable iwdg
(*((volatile unsigned int*) 0x40003000)) = 0x00005555;  //enable register access
(*((volatile unsigned int*) 0x40003004)) = 0x07;        //prescaler 0-7
(*((volatile unsigned int*) 0x40003008)) = 0x00000FFF;
while((*((volatile unsigned int*) 0x4000300C)) > 1)
	{;}
(*((volatile unsigned int*) 0x40003000)) = 0x0000AAAA;  //reset register 
}


/*
Restart watchdog
*/
void moje_iwdg_reset(void)
{
	(*((volatile unsigned int*) 0x40003000)) = 0x0000AAAA;
}







