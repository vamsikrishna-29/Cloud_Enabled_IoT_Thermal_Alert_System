//i2c_eeprom.c

#include "types.h"

#include "delay.h"

#include "i2c_peripheral.h"

void i2c_eeprom_bytewrite(u8 slaveAddr,u16 wBuffAddr,u8 dat)
{
    //Generate i2c start condition 

   i2c_start();	 

   // Send EEPROM slaveAddr + w,& take ack 

   i2c_write(slaveAddr<<1);  

   //Send higher byte of EEPROM memory address

    i2c_write(wBuffAddr>>8);

   // Send lower byte of EEPROM memory address

    i2c_write(wBuffAddr);    

   //Send data byte to EEPROM

   i2c_write(dat);    

   //issue i2c stop condition	 

   i2c_stop(); 

   //write cycle delay 

   delay_ms(10); 

}

// Read one byte of data from EEPROM using I2C
			 
u8 i2c_eeprom_randomread(u8 slaveAddr,u16 rBuffAddr) 
{

// variable to store the received data
    u8 dat;	 

    //Generate start condition 

    i2c_start();	 

    //Send EEPROM slaveAddr + w,option

    i2c_write(slaveAddr<<1); 

    //Send higher byte of EEPROM memory address

    i2c_write(rBuffAddr>>8);

   // Send lower byte of EEPROM memory address

    i2c_write(rBuffAddr);  

    //Generate I2C restart  condition

    i2c_restart();	 

    //Send EEPROM slaveAddr + r,option 

    i2c_write(slaveAddr<<1|1);  

   //read byte from slave & send NACK

    dat=i2c_nack();	 

    //stop condition 

    i2c_stop(); 
   
    //return received data    

    return dat; 

}

//Write multiple bytes to EEPROM using I2C 
void i2c_eeprom_pagewrite(u8 slaveAddr,u16 wBuffStartAddr,s8 *p,u8 nBytes)
{ 

//loop counter
  u8 i; 

//Generate I2C start condition

  i2c_start();	 

// slaveAddr + w 

//Send EEPROM slave address with write option

  i2c_write(slaveAddr<<1);     

//Send higher byte of EEPROM start address

  i2c_write(wBuffStartAddr>>8);

//Send Lower byte of EEPROM start address

  i2c_write(wBuffStartAddr);   

//write all bytes to EEPROM

  for(i=0;i<nBytes;i++) 

  { 

   //Send one data byte

    i2c_write(p[i]);              

  } 

   //issue stop condition

  i2c_stop(); 

  //internal write cycle delay

  delay_ms(10); 

}			  

//Read multiple bytes sequentially from  EEPROM using I2C
void i2c_eeprom_seqread(u8 slaveAddr,u16 rBuffStartAddr,s8 *p,u8 nBytes)
{

//Loop counter
   u8 i;

//Generate I2C start condition
   i2c_start();	

   //Send slaveAddr + w,option	

   i2c_write(slaveAddr<<1);  

  //Send higher byte of EEPROM Start addres

   i2c_write(rBuffStartAddr>>8);

 //Send lower byte of EEPROM start address

   i2c_write(rBuffStartAddr);  

   //restart condition	

   i2c_restart();	 

   //Send EEPROM slaveAddr + r ,& take ack 

   i2c_write(slaveAddr<<1|1); 

  //read all bytes expect the last one send ACK

    for(i=0;i<nBytes-1;i++) 
   {
    //read byte,give mack

      p[i]=i2c_mack();	 

   } 

   //read last byte,give nack

   p[i]=i2c_nack(); 

  //stop condition

   i2c_stop(); 

}
