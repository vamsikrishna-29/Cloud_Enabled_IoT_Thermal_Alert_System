#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "lcd.h"

extern char buff[200];
extern unsigned char i;
extern u32 setTemp;


void esp01_connectAP()
{
    // Clear LCD display
    CmdLCD(0x01);

    // Move cursor to first row
    CmdLCD(0x80);

    // Display first line message
    StrLCD("CONNECTING TO");

    // Move cursor to second row
    CmdLCD(0xC0);

    // Display second line message
    StrLCD("WIFI...");

    // Wait for 1 second
    delay_ms(1000);

    // Send AT command to check ESP01 response
    UART0_TxStr("AT\r\n");

    // Reset receive index and clear receive buffer
    i = 0;
    memset(buff,'\0',200);

    // Wait until response is received
    while(i < 4);

    // Wait for complete response
    delay_ms(500);

    // Terminate received string
    buff[i] = '\0';

    // Wait for stability
    delay_ms(1000);

    // Check whether ESP01 responded with OK
    if(strstr(buff,"OK"))
    {
        // Wait for 1 second
        delay_ms(1000);
    }
    else
    {
        // Display error message
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);

        // Exit the function
        return;
    }

    // Disable command echo
    UART0_TxStr("ATE0\r\n");

    // Reset receive index and clear buffer
    i = 0;
    memset(buff,'\0',200);

    // Wait for response
    while(i < 4);

    // Wait for complete response
    delay_ms(500);

    // Terminate received string
    buff[i] = '\0';

    // Verify echo is disabled
    if(strstr(buff,"OK"))
    {
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    // Configure ESP01 for single TCP connection
    UART0_TxStr("AT+CIPMUX=0\r\n");

    // Reset receive index and clear buffer
    i = 0;
    memset(buff,'\0',200);

    // Wait for response
    while(i < 4);

    // Wait for complete response
    delay_ms(500);

    // Terminate received string
    buff[i] = '\0';

    // Additional delay
    delay_ms(2000);

    // Check command execution
    if(strstr(buff,"OK"))
    {
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    // Disconnect from previously connected Wi-Fi
    UART0_TxStr("AT+CWQAP\r\n");

    // Reset receive index and clear buffer
    i = 0;
    memset(buff,'\0',200);

    // Wait for response
    while(i < 4);

    // Wait for disconnect process
    delay_ms(1500);

    // Terminate received string
    buff[i] = '\0';

    // Additional delay
    delay_ms(2000);

    // Verify successful disconnection
    if(strstr(buff,"OK"))
    {
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    // Connect ESP01 to Wi-Fi Access Point
    UART0_TxStr("AT+CWJAP=\"vivo T3 Ultra\",\"vivot333\"\r\n");

    // Reset receive index and clear buffer
    i = 0;
    memset(buff,'\0',200);

    // Wait for connection response
    while(i < 4);

    // Allow time for Wi-Fi connection
    delay_ms(2500);

    // Terminate received string
    buff[i] = '\0';

    // Clear LCD
    CmdLCD(0x01);

    // Move cursor to first row
    CmdLCD(0x80);

    // Display ESP01 response
    StrLCD(buff);

    // Display response for 2 seconds
    delay_ms(2000);

    // Verify Wi-Fi connection
    if(strstr(buff,"WIFI CONNECTED"))
    {
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }
}


void esp01_sendToThingspeak(s8 *val)
{
    char cmd[40];
    int len;
    int temp;

    // Clear LCD display
    CmdLCD(0x01);

    // Move cursor to first row
    CmdLCD(0x80);

    // Display first line message
    StrLCD("SENDING DATA");

    // Move cursor to second row
    CmdLCD(0xC0);

    // Display second line message
    StrLCD("TO CLOUD...");

    // Start TCP connection with ThingSpeak server
    UART0_TxStr("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    // Reset receive index
    i = 0;

    // Clear receive buffer
    memset(buff,'\0',200);

    // Wait for connection
    delay_ms(2500);

    // Terminate received string
    buff[i] = '\0';

    // Check whether connection is successful
    if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
    {
        // Convert temperature string to integer
        temp = atoi(val);

        // Check if temperature exceeds setpoint
        if(temp >= setTemp)
        {
            // Calculate HTTP request length for OVERHEAT status
            len = strlen("GET /update?api_key=Q5IBBGMWGR5FGM7O&field2=")
                + strlen(val)
                + strlen("&field3=1&status=OVERHEAT\r\n\r\n");
        }
        else
        {
            // Calculate HTTP request length for NORMAL status
            len = strlen("GET /update?api_key=Q5IBBGMWGR5FGM7O&field2=")
                + strlen(val)
                + strlen("&field3=0&status=NORMAL\r\n\r\n");
        }

        // Prepare CIPSEND command
        sprintf(cmd,"AT+CIPSEND=%d\r\n",len);

        // Send CIPSEND command
        UART0_TxStr(cmd);

        // Wait for prompt
        delay_ms(1000);

        // Send first part of HTTP GET request
        UART0_TxStr("GET /update?api_key=Q5IBBGMWGR5FGM7O&field2=");

        // Send temperature value
        UART0_TxStr(val);

        // Send OVERHEAT status if temperature is above setpoint
        if(temp >= setTemp)
        {
            UART0_TxStr("&field3=1&status=OVERHEAT\r\n\r\n");
        }
        else
        {
            // Send NORMAL status if temperature is below setpoint
            UART0_TxStr("&field3=0&status=NORMAL\r\n\r\n");
        }

        // Reset receive index
        i = 0;

        // Clear receive buffer
        memset(buff,'\0',200);

        // Wait for server response
        delay_ms(5000);

        // Terminate received string
        buff[i] = '\0';

        // Check if data upload is successful
        if(strstr(buff,"SEND OK"))
        {
            // Clear LCD
            CmdLCD(0x01);

            // Display success message
            StrLCD("DATA UPDATED");

            // Wait for 1 second
            delay_ms(1000);
        }
        else
        {
            // Clear LCD
            CmdLCD(0x01);

            // Display failure message
            StrLCD("DATA NOT UPDATED");

            // Wait for 1 second
            delay_ms(1000);

            // Clear LCD
            CmdLCD(0x01);

            // Display ESP01 response
            StrLCD(buff);

            // Display response for 3 seconds
            delay_ms(3000);
        }
    }
    else
    {
        // Clear LCD
        CmdLCD(0x01);

        // Display connection error
        StrLCD("CONNECT ERROR");

        // Wait for 1 second
        delay_ms(1000);

        // Clear LCD
        CmdLCD(0x01);

        // Display ESP01 response
        StrLCD(buff);

        // Display response for 3 seconds
        delay_ms(3000);
    }
}


void esp01_sendSPToThingspeak(s8 *val)
{
      char req[100];
      char cmd[25];

    // Clear LCD display
    CmdLCD(0x01);

    // Move cursor to first row
    CmdLCD(0x80);

    // Display first line message
    StrLCD("SENDING SETPOINT");

    // Move cursor to second row
    CmdLCD(0xC0);

    // Display second line message
    StrLCD("TO CLOUD..");

    // Wait for 1 second
    delay_ms(1000);

    // Open TCP connection with ThingSpeak server
    UART0_TxStr("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    // Reset receive index
    i = 0;

    // Clear receive buffer
    memset(buff,'\0',200);

    // Wait for ESP01 response
    while(i < 5);

    // Wait for TCP connection to complete
    delay_ms(2500);

    // Terminate received string
    buff[i] = '\0';

    // Check whether TCP connection is established
    if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
    {
        // Create HTTP GET request with setpoint value
        sprintf(req,"GET /update?api_key=Q5IBBGMWGR5FGM7O&field1=%s\r\n\r\n",val);

        // Create CIPSEND command with request length
        sprintf(cmd,"AT+CIPSEND=%d\r\n",strlen(req));

        // Send CIPSEND command
        UART0_TxStr(cmd);

        // Reset receive index
        i = 0;

        // Clear receive buffer
        memset(buff,'\0',200);

        // Wait before sending HTTP request
        delay_ms(500);

        // Send HTTP GET request
        UART0_TxStr(req);

        // Wait for ThingSpeak response
        delay_ms(5000);

        // Additional waiting time
        delay_ms(5000);

        // Terminate received string
        buff[i] = '\0';

        // Wait for complete response
        delay_ms(2000);

        // Check whether data upload is successful
        if(strstr(buff,"SEND OK"))
        {
            // Clear LCD
            CmdLCD(0x01);

            // Display success message
            StrLCD("SETPOINT UPDATED");

            // Display message for 1 second
            delay_ms(1000);
        }
        else
        {
            // Clear LCD
            CmdLCD(0x01);

            // Display failure message
            StrLCD("SETPOINT NOT UPDATED");

            // Display message for 1 second
            delay_ms(1000);
        }
    }
    else
    {
        // Move cursor to second row
        CmdLCD(0xC0);

        // Display connection error
        StrLCD("ERROR");

        // Display message for 1 second
        delay_ms(1000);

        // Exit the function
        return;
    }
}


void esp01_readFromThingspeak(char *val)
{
    // Declare pointer to locate the received data in the response
    s8 *ptr;

    // Clear LCD display
    CmdLCD(0x01);

    // Move cursor to first row
    CmdLCD(0x80);

    // Display first line message
    StrLCD("READING FROM");

    // Move cursor to second row
    CmdLCD(0xC0);

    // Display second line message
    StrLCD("CLOUD...");

    // Wait for 1 second
    delay_ms(1000);

    // Open TCP connection with ThingSpeak server
    UART0_TxStr("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    // Reset receive index
    i = 0;

    // Clear receive buffer
    memset(buff,'\0',200);

    // Wait for TCP connection
    delay_ms(2500);

    // Terminate received string
    buff[i] = '\0';

    // Check whether TCP connection is successful
    if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
    {
        // Send CIPSEND command with request length
        UART0_TxStr("AT+CIPSEND=66\r\n");

        // Reset receive index
        i = 0;

        // Clear receive buffer
        memset(buff,'\0',200);

        // Wait before sending HTTP request
        delay_ms(500);

        // Request the latest value from ThingSpeak
        UART0_TxStr("GET /channels/3425798/fields/1/last.txt?api_key=JUKAGMOMLFT5BGHI\r\n");

        // Wait for server response
        delay_ms(5000);

        // Additional waiting time
        delay_ms(5000);

        // Terminate received string
        buff[i] = '\0';

        // Wait before processing the received data
        delay_ms(2000);

        // Search for ':' character in HTTP response
        if((ptr = strchr(buff,':')) != NULL)
        {
            // Move pointer to the first character after ':'
            ptr++;

            // Reset value index
            i = 0;

            // Copy numeric value from response
            while(((*ptr >= '0' && *ptr <= '9') || (*ptr == '.')) && (i < 5))
            {
                // Store received character
                val[i] = *ptr;

                // Move to next character
                ptr++;

                // Increment array index
                i++;
            }

            // Terminate extracted string
            val[i] = '\0';

            // Wait for 2 seconds
            delay_ms(2000);

            // Clear LCD display
            CmdLCD(0x01);
        }
        else
        {
            // Clear LCD display
            CmdLCD(0x01);

            // Move cursor to second row
            SetCursor(1,0);

            // Display error message
            StrLCD("data not read");

            // Display message for 3 seconds
            delay_ms(3000);

            // Clear LCD display
            CmdLCD(0x01);
        }
    }
    else
    {
        // Move cursor to second row
        CmdLCD(0xC0);

        // Display connection error
        StrLCD("ERROR");

        // Display message for 1 second
        delay_ms(1000);

        // Exit the function
        return;
    }
}
