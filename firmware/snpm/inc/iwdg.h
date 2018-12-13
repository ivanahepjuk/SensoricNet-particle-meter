#ifndef IWDG_H
#define IWDG_H
/*
#define IWDG_KR		(*((volatile unsigned int*) 0x40003000))  //		
#define IWDG_PR		(*((volatile unsigned int*) 0x40003004))
#define IWDG_RL		(*((volatile unsigned int*) 0x40003008))		
#define IWDG_SR		(*((volatile unsigned int*) 0x4000300C))
*/

void moje_iwdg_setup(void);
void moje_iwdg_reset(void);






#endif
