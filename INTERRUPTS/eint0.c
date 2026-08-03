#include <LPC21xx.h>
#include <stdlib.h>
#include "eint0.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "eint0_defines.h"
#include "types.h"
#include "lcd.h"
#include "kpm.h"
#include "esp01.h"
#include "i2c_eeprom.h"
#include "delay.h"

extern u32 flag;
extern s8 setPt[6];
extern u32 setTemp;
extern u32 display_flag;


void Init_eint1(void)
{
        //cfg p0.14 pin as EINT1 input pin
        CfgPortPinFunc(0,14,PINFUNC3);

        //cfg VIC peripheral
        //cfg EINT1 as IRQ interrupt
        VICIntSelect = 0;
        //enable EINT1 via VIC
        VICIntEnable|=1<<EINT1_VIC_CHNO;
        //cfg  EINT1 as v.irq with highest priority
        VICVectCntl1 = (1<<5)|EINT1_VIC_CHNO;
        //load EINT1 isr addr
        VICVectAddr1 =(u32)eint1_isr;

        //cfg External Interrupt Peripheral
        //allow/enable EINT1,default all are enabled
        //EXTINT = 1<<2;
        //cfg EINT1 as edge triggered
        EXTMODE |= (1<<1);
        EXTPOLAR &= ~(1<<1);
        //cfg EINT1 as falling edge triggerd,
        //def all are falling edge
        //cfg EINT1 status LED pin as gpio out
}

//interrupt service routine for EINT1
void eint1_isr(void) __irq
{
// Set interrupt flag
        flag=1;
	     
        //clear EINT1 status in External Interrupt Peripheral
        EXTINT = 1<<1;
        //Acknowledge interrupt to VIC
        VICVectAddr=0;
}

//Initialize the External Interrupt 2
void Init_eint2(void)
{
        //cfg p0.15 pin as EINT2 input pin
        CfgPortPinFunc(0,15,PINFUNC3);

        //cfg VIC peripheral
        //cfg EINT2 as irq types,default all are irq type
          VICIntSelect = 0;
        //enable EINT2 via VIC
        VICIntEnable|=1<<EINT2_VIC_CHNO;
        //cfg  EINT2 as v.irq with highest priority
        VICVectCntl2 = (1<<5)|EINT2_VIC_CHNO;
        //load EINT2 isr addr
        VICVectAddr2 =(u32)eint2_isr;
        //cfg External Interrupt Peripheral
        //cfg EINT2 as edge triggered
        EXTMODE |= (1<<2);
        //cfg EINT2 as falling edge triggerd,
      
        EXTPOLAR &= ~(1<<2);

        
}
//External interrupt 2 Service Routine
void eint2_isr(void) __irq
{
//Set display flag
    display_flag = 1;
//Clear the EINT2 interrupt
    EXTINT = 1<<2;
//Acknowledge interrupt to vic
    VICVectAddr = 0;
}

