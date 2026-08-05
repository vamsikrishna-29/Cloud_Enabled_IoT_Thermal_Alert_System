//lcd.c
#include "types.h"
#include "delay.h"
#include "lcd_defines.h"
#include "defines.h"
#include <LPC21xx.h>

//write one byte to the LCD
void writeLCD(u8 byte)
{
#if LCD_MODE==8
//select Write operation(RW=0)
IOCLR0=1<<LCD_RW;
//send 8-bit data to LCD data pins
WRITEBYTE(IOPIN0,LCD_DATA,byte);
//generate enable pulse
IOSET0=1<<LCD_EN;
delay_us(1);
IOCLR0=1<<LCD_EN;
//wait for LCD to process the byte
delay_ms(2);
#elif LCD_MODE==4
#endif
}

//send a command to the LCD
void CmdLCD(u8 cmdbyte)
{
//select the Register(RS=0)
IOCLR0=1<<LCD_RS;
//write command byte to LCD
writeLCD(cmdbyte);
}

//Initialize the LCD in 8 bit mode
void InitLCD(void)
{
//configure the LCD data and control pins as output
IODIR0|=((0xff<<LCD_DATA)|(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
//wait for LCD power up
delay_ms(15);
//8-bit mode .1-line display
CmdLCD(MODE_8BIT_1LINE);
//required initialization delay
delay_ms(4);
delay_us(100);
//repeat function set command
CmdLCD(MODE_8BIT_1LINE);
delay_us(100);
CmdLCD(MODE_8BIT_1LINE);
//configure LCD for 8-bit,2nd line
CmdLCD(MODE_8BIT_2LINE);
//turn on display with blinking cursor
CmdLCD(DSP_ON_CUR_BLINK);
//clear the LCD display
CmdLCD(CLEAR_LCD);
//Move cursor to the beginning
CmdLCD(SHIFT_CUR_RIGHT);
}
//send a single character to the LCD
void CharLCD(u8 asciival)
{
//select data register(RS=1)
IOSET0=1<<LCD_RS;
//write character  to LCD
writeLCD(asciival);
}

//Display a string on the LCD
void StrLCD(s8 *s)
{
//send a characters until end of the string
while(*s)
CharLCD(*s++);
}

//display unsigned 32 bit integer on the LCD
void U32LCD(u32 n)
{
s32 i=0;//digit counter
u8 a[10];//buffer to store digits
if(n==0)//handle zero seperately
CharLCD('0');
else
{
//convert digits into display them
while(n>0)
{
a[i++]=(n%10)+48;
n/=10;
}
for(--i;i>=0;i--)
CharLCD(a[i]);
}
}


//display a signed 32 bit integer on the LCD
void S32LCD(s32 n)
{
//check if no is negitive
if(n<0)
{
//display the negitive sign
CharLCD('-');
//convert positive
n=-n;
}

U32LCD(n);
}


//display floating point number on the LCD
void F32LCD(f32 fn,u8 ndp)
{
u32 n,i;
//check if the number is negitive
if(fn<0.0)
{
CharLCD('-');
//convert to positive
fn=-fn;
}
//display the integer part
n=fn;
U32LCD(n);
//display decimal point
CharLCD('.');
//display the fractional part to the specified decimal places
for(i=0;i<ndp;i++)
{
//extract the next digit
fn=(fn-n)*10;
n=fn;
CharLCD(n+48);
}
}

//create CUSTOM characters on LCD CGRAM
void buildCGRAM(u8 *p,u8 nbytes)
{
u32 i;
//set CGRAM address
CmdLCD(GOTO_CGRAM_START);
//select data register
IOSET0=1<<LCD_RS;
//write custom character pattern
for(i=0;i<nbytes;i++)
{
writeLCD(p[i]);
}
//return the cursor beginning of the Line 1
CmdLCD(GOTO_LINE1_POS0);
}

void ClearLCD(void)
{
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
}



//display the "CHANGED SUCCESSFULLY" message on LCD
void changemsg(void)
{

		CmdLCD(CLEAR_LCD);

		CmdLCD(GOTO_LINE1_POS0+4);

		StrLCD("CHANGED");

		CmdLCD(GOTO_LINE2_POS0+1);

		StrLCD("SUCCESSFULLY");

		delay_ms(800);

}

//display the "UPDATED SUCCESSFULLY" message on LCD
void updatemsg(void)
{

		CmdLCD(CLEAR_LCD);

		CmdLCD(GOTO_LINE1_POS0+4);

		StrLCD("UPDATED");

		CmdLCD(GOTO_LINE2_POS0+1);

		StrLCD("SUCCESSFULLY");

		delay_ms(600);

}
//Scrolls a msg across the LCD
void  MsgScroll(s8 *s,s32 size)
{

	u32 i=0;
//display characters until end of the string
	while(*s)
	{
//move to the second line after 13 characters
		if(i==13)
		{
			CmdLCD(GOTO_LINE2_POS0);
		}
//display the current character
		CharLCD(*s);
//delay for scrolling effect
		delay_ms(200);
//move to the next character
		s++;
		i++;
	
	}
}

//sets the LCD cursor to the specified line and position
void SetCursor(u8 line,u8 pos)
{
//position cursor on Line 1
	if(line==1)
		CmdLCD(GOTO_LINE1_POS0+pos);
//position  cursor on Line 2
	else if(line==2)
		CmdLCD(GOTO_LINE2_POS0+pos);
}
