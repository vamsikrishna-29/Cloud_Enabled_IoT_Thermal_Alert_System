#include "types.h"
#include "adc.h"
//function to read the temperature in celcius,fahrenheit,or kelvin
void Read_Temperature(s8 TempType,f32 *temp)
{
	u32 adcDVal; //stores the raw ADC digital value(0-1023)
	f32 adcAR,degC,degF,degK;////ADC analog voltage ans temperature values
        //read ADC channel 1
        // adcval=raw adc val
        // adcAR=converted analog value
	Read_ADC(1,&adcDVal,&adcAR);
        //convert LM35 output voltage to celsius
        // LM35 gives 10mV/c,so temperature=voltage*100
	degC=adcAR*100;
        //convert celsius to fahrenheit
	degF=(degC*1.8)+32;
        // convert celsius to kelvin
	degK=degC+273.15;
        //retuyrn temp in celsius
	if(TempType=='C')
		*temp=degC;
        // return temp in fahrenheit
	else if(TempType=='F')
		*temp=degF;
        // return temp in kelvin
	else if(TempType=='K')
		*temp=degK;
}	
