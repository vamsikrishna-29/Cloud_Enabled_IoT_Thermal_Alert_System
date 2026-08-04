//i2c_peripheral.c

#include <LPC21xx.h>

#include "types.h"

#include "pin_function_defines.h"

#include "pin_connect_block.h"

#include "i2c_defines.h"


void Init_I2C(void) 
{ 

   //Cfg p0.2 for SCL functions 

   CfgPortPinFunc(0,2,SCL_PIN_0_2); 

   //Cfg p0.3 for SDA functions  

   CfgPortPinFunc(0,3,SDA_PIN_0_3); 

   //Cfg Speed for I2C Serial Communication 

   I2SCLL=I2C_DIVIDER; 

   I2SCLH=I2C_DIVIDER; 

   //I2C Peripheral Enable for Communication 

   I2CONSET=1<<I2EN_BIT;  

}


void i2c_start(void) 

{ 

  // start condition 

  I2CONSET=1<<STA_BIT; 

  //wait for start bit status 

  while(((I2CONSET>>SI_BIT)&1)==0); 

 // clear start condition 

  I2CONCLR=1<<STA_BIT; 

}	 


void i2c_restart(void) 

{ 

   // start condition 

   I2CONSET=1<<STA_BIT; 

   //clr prev SI_BIT 

   I2CONCLR=1<<SIC_BIT; 

    //wait for SI bit status 

   while(((I2CONSET>>SI_BIT)&1)==0); 

   // clear start condition 

   I2CONCLR=1<<STA_BIT; 

}	 


void i2c_write(u8 dat) 

{ 

   //put data into I2C0DAT 

    I2DAT=dat;	 

    //clr SI_BIT using I2CON 

    I2CONCLR=1<<SIC_BIT; 

    //wait for SI bit status 

    while(((I2CONSET>>SI_BIT)&1)==0); 

}	 

void i2c_stop(void) 

{ 

    // issue stop condition 

    I2CONSET=1<<STO_BIT; 

    // clr SI bit status	 

    I2CONCLR=1<<SIC_BIT; 

    //stop will be cleared automatically 

} 

//read one byte from I2C and send NACK
u8 i2c_nack(void) 
{	 
     //I2C0CONSET = 0x00; //Assert Not of Ack 

     I2CONCLR=1<<SIC_BIT;//Clear the SI flag to continue the I2C operation

     while(((I2CONSET>>SI_BIT)&1)==0);//wait until data is received(SI flag becomes 1)

     return I2DAT; //return the received data byte

} 

//read one byte from I2C and send ACK
u8 i2c_mack(void)

{	

    I2CONSET=1<<AA_BIT; //Assert Ack 

    I2CONCLR=1<<SIC_BIT; //Clear the SI flag to continue I2C operation

    while(((I2CONSET>>SI_BIT)&1)==0);//wait until data is received(SI flag becomes 1)

    I2CONCLR=1<<AAC_BIT; //Clear  ACK bit after acknowledge the received byte

    return I2DAT;//return the received byte

}
