//delay.c
#include "types.h"

//Delay time in microseconds
void delay_us(u32 dlyus)
{
//convert microseconds into loop
dlyus*=12;
//Wait until loop count becomes zero
while(dlyus--);
}

//Delay time in milli seconds
void delay_ms(u32 dlyms)
{
dlyms*=12000;
while(dlyms--);
}

//Delay time in seconds
void delay_s(u32 dlys)
{
dlys*=12000000;
while(dlys--);
}

