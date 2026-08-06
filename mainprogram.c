#include "system.h"
#include "types.h"
#include "lcd.h"
#include "delay.h"
#include "esp01.h"
#include "rtc.h"

u32 flag = 0;
s32 hour,minute,second;

extern u32 display_flag;
extern u32 setTemp;

int main(void)
{
    // Initialize Hardware Modules 
    System_Init();
    // Connect ESP01 to Wi-Fi  
    esp01_connectAP();

    // Load Set Point from EEPROM 
    Load_SetPoint_From_EEPROM();

    while(1)
    {
		GetRTCTimeInfo(&hour,&minute,&second);
        // Display RTC Time and Temperature 
        Display_System_Status();

        // Check Temperature Alarm 
        Monitor_Alarm();

        // Local Set Point Update 
        if(flag)
        {
            flag = 0;

            CmdLCD(0x01);
            StrLCD("INT OCCURED");

            delay_ms(1000);

            Update_Local_SetPoint();
        }

        // Display Updated Set Point 
        if(display_flag)
        {
            display_flag = 0;

            ClearLCD();

            StrLCD("SET POINT:");
            SetCursor(2,7);
            U32LCD(setTemp);

            delay_ms(2000);
        }

        // Upload Temperature to ThingSpeak (Every 3 Minute) 
        Upload_Temperature_To_Cloud();

        // Upload Set Point from Cloud (Every 2 Minutes) 
        Update_SetPoint_From_Cloud();
    }  
}

