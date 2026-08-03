#include "types.h"

s8 str[5];
s8* floatToStr(f32 fvalue)
{
	
	s8 tem[5];
	s32 num,val,digit,i=0,j=0;

	num=fvalue;
	val=num;
	while(num)
	{
		digit=num%10;
		tem[i++]=digit+48;
		num/=10;
	}
	for(--i;i>=0;i--,j++)
	{
		str[j]=tem[i];
	}
	str[j++]='.';

	fvalue=fvalue-val;
	fvalue*=10;
	val=fvalue;
	str[j++]=val+48;
	str[j]='\0';

	return str;
}
