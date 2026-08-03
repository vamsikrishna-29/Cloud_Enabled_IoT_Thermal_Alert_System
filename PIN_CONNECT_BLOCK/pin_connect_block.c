//pin_connect_block.c

#include "types.h"

#include <LPC21xx.h>
//function to configure the alternate function of GPIO pin
void CfgPortPinFunc(u32 portNo,u32 pinNo,u32 pinFunc)
{
//Check if the selected port is port 0

	if(portNo==0)
	{
//configure pins p0.0 to p0.15 using PINSEL0

		if(pinNo<=15)
		{
// clear the existing function bits and set the new function

			PINSEL0=((PINSEL0&~(3<<(pinNo*2)))|

			         (pinFunc<<(pinNo*2)));

		}
// configure pins p0.16 to p0.31 using PINSEL1

		else if(pinNo>=16 && pinNo<=31)
                {
// clear the existing function bits and set the new function

			PINSEL1=((PINSEL1&~(3<<((pinNo-16)*2)))|

			         (pinFunc<<((pinNo-16)*2)));

		}

	}

}
