#include <LPC21xx.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "adc.h"
#include "lm35.h"
#include "rtc.h"
#include "uart0.h"
#include "eint0.h"
#include "myfunctions.h"
#include "esp01.h"
#include "pin_connect_block.h"
#include "i2c_eeprom.h"
#include "i2c_peripheral.h"
#include "system.h"
extern u32 flag;
u32 lastCloudMinute = 60;
u32 lastMinute=60;
extern s32 hour,minute,second;

s8 setPt[6];
s8 rsetPt[6];
s8 newSPt[6];
s8 *lmVal;

f32 temp;

u32 setTemp;

s32 ctemp;

s8 msg[36] = "CLOUD ENABLED IOT THERMAL SYS";
u32 display_flag = 0;

//Initialize all the required  peripherals
void System_Init(void)
{
    Init_I2C();
    InitLCD();
    Init_KPM();
    Init_ADC();
    Init_RTC();
    Init_eint1();
    Init_eint2();
    Init_UART0();
    
    // Configure LED pin as output  

    IODIR0 |= (1<<OLED_PIN);
   
    // Configure Buzzer pin as output

    IODIR0 |= (1<<OBUZ_PIN);

    MsgScroll(msg,36);
}

// Load the temperature setpoint from EEPROM

void Load_SetPoint_From_EEPROM(void)
{	
    // Read last saved setpoint from EEPROM 

     i2c_eeprom_seqread(0x50, 0x0000, setPt, 6);

     StrLCD("load set point");

    // First time EEPROM is empty 

      if(setPt[0] == 0xFF)
       {
        // Assign default setpoint 

        strcpy((char *)setPt, "40");

        // Store default setpoint in EEPROM 

        i2c_eeprom_pagewrite(0x50, 0x0000, setPt, 6);

        delay_ms(10);
       }

    // Load setpoint into RAM 

    setTemp=atoi((char *)setPt);

    // Display current setpoint 

    ClearLCD();
    StrLCD("SET POINT:");
    SetCursor(2,7);
    StrLCD(setPt);
    delay_s(2);

  // Set next cloud update time 

    lastMinute=MIN;
    lastCloudMinute=MIN;
}


//Display the time and temperature

void Display_System_Status(void)
{

    //Read the Current Temperature from LM35 sensor

    Read_Temperature('C', &temp);

    //clear the LCD before updating the display

        ClearLCD();

    //Check if the Temperature exceeds the Setpoint

    if(temp >= setTemp)
    {
        // Display alert message

        SetCursor(1,5);
        StrLCD("ALERT!");

        // Display the current Temperature

        SetCursor(2,0);
        StrLCD("TEMP:");

        SetCursor(2,6);
        F32LCD(temp,1);
        CharLCD(0xDF);
	CharLCD('C');
    }
    else
    {
       // Display the normal system status

        SetCursor(1,2);
        StrLCD("TIME    TEMP");

      // Displays the RTC time

        SetCursor(2,0);
        DisplayRTCTime(hour,minute,second);

      // Display the current temperature

        SetCursor(2,10);
        F32LCD(temp,1);
        CharLCD(0xDF);   
        CharLCD('C');  
    }
    delay_ms(500);
}

void Monitor_Alarm(void)
{
    if(temp >= setTemp)
    {
        IOCLR0 = 1 << OLED_PIN;   // LED ON
        IOSET0 = 1 << OBUZ_PIN;   // Buzzer ON
    }
    else
    {
        IOSET0 = 1 << OLED_PIN;   // LED OFF
        IOCLR0 = 1 << OBUZ_PIN;   // Buzzer OFF
    }
}

// Update the temperature setpoint entered by the user

void Update_Local_SetPoint(void)
{
    CmdLCD(0x01);
    StrLCD("ENTER SETPOINT:");
    SetCursor(2,7);
    StrLCD("(0-150)");
    SetCursor(2,1);

    // Read the set point from keypad

    ReadValue(setPt);      
    
    // Convert entered string to onteger

    ctemp = atoi(setPt);

    // Validate the entered set point

    if((ctemp >= 0) && (ctemp <= 150))
    {
         // Check if the new set point is same as current set point

        if(setTemp == ctemp)
        {
            ClearLCD();
            StrLCD("SAME SETPOINT");
            delay_ms(1000);
            ClearLCD();
       
        }
        else
        {
          // Upload new setpoint to ThingSpeak
     
            esp01_sendSPToThingspeak(setPt);
           
            // Store the new setpoint in EEPROM

            i2c_eeprom_pagewrite(0x50,0x0000,setPt,6);
    
            //Update current setpoint

            setTemp = ctemp;

            ClearLCD();
        }
    }
    else
    {
       // Displays the invalid range message

        CmdLCD(0x01);
        StrLCD("INVALID RANGE");
        SetCursor(2,1);
        StrLCD("USE 0-150");
        delay_ms(1000);
	ClearLCD();
        Update_Local_SetPoint();
        return;
    }
}
void Upload_Temperature_To_Cloud(void)
{
    // Check if 3-minutes interval is reached

    if((MIN %3==0)&&(MIN !=lastMinute))
    {
        // Save current minute to avoid repeated uploads

        lastMinute=MIN;
        
        // Convert temperature to string
    
        lmVal = floatToStr(temp);

        ClearLCD();

        delay_ms(500);

       // Send  Temperature to Thingspeak 

        esp01_sendToThingspeak(lmVal);
    }
}
// Update setpoint from ThingSpeak every 2 minutes
          
void Update_SetPoint_From_Cloud(void)
{
    // Check if 2-minute interval is reached

    if((MIN % 2 == 0)&&(MIN != lastCloudMinute))
    {
      // Store current minute to avoid repeated cloud reads

      lastCloudMinute=MIN;

      // Read latest Set Point from ThingSpeak

        esp01_readFromThingspeak(newSPt);
       
      // Convert received string to integer

        ctemp = atoi((char *)newSPt);

     // Validate received set point

        if((ctemp >= 0) && (ctemp <= 150))
        {

     // Read current Set Point from EEPROM

          i2c_eeprom_seqread(0x50, 0x0000, rsetPt, 6);

     // Compare Cloud value with EEPROM value

            if(strcmp((char *)newSPt, (char *)rsetPt) != 0)
            {
                // Save new Set Point into EEPROM

                i2c_eeprom_pagewrite(0x50, 0x0000, newSPt, 6);

                // Update RAM copy

                strcpy((char *)setPt, (char *)newSPt);

                setTemp = ctemp;

                // Display Message

                ClearLCD();
                StrLCD("SETPOINT");
                SetCursor(2,1);
                StrLCD("UPDATED");

                delay_ms(1000);

                display_flag = 1;
            }
            else
            {
            ClearLCD();
            StrLCD("SAME SETPOINT");
            SetCursor(2,1);
            StrLCD("NO CHANGE");
            delay_ms(1000);
            display_flag=1;
            } 
        }
        else
        {
           // Display invalid setpoint received from cloud

            ClearLCD();
            StrLCD("INVALID RANGE");
            SetCursor(2,1);
            StrLCD("USE 0-150");
            delay_ms(1000);

        }
    }
}
	
