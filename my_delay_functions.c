/*********************************************
my_delay_function.c

Library of delay functions

**********************************************/

#include <delays.h>
#include "my_delay_functions.h"


//********************************************************

void delay_1S(unsigned char increment)
{
	unsigned char i;

	for(i=0;i<increment;i++)
	{
		Delay10KTCYx(200);
		Delay10KTCYx(200);
		Delay10KTCYx(200);
		Delay10KTCYx(200);
	
	}
	
}

//********************************************************


void delay_1DS(unsigned char increment)
{
	unsigned char i;

	for(i=0;i<increment;i++)
	{
		Delay10KTCYx(80);
	
	}
	
}

//********************************************************

void delay_1CS(unsigned char increment)
{
	unsigned char i;

	for(i=0;i<increment;i++)
	{
		Delay10KTCYx(8);
	
	}
	
}

//********************************************************


void delay_1MS(unsigned char increment)
{
	unsigned char i;

	for(i=0;i<increment;i++)
	{
		Delay1KTCYx(8);
	
	}
	
}