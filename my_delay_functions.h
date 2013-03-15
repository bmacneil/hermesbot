/******************************************
my_delay_functions.h

This is just a quick little library of delay functions 
that i might find useful at the time. 
All of my functions will be based off of the delays.h
library built into C18 but I would rather have a quick 
function which I can increment in seconds without 
calculations and that dosent top out at .25 seconds.
All of these functions assume that the MCU is configured 
to run at 32MHz.
Includes:

delay_1DS(increment); is a funtion that will produce a delay
in increments of 0.1 seconds based on the function call value.

*********************************************/


void delay_1S(unsigned char increment);
void delay_1DS(unsigned char increment);
void delay_1CS(unsigned char increment);
void delay_1MS(unsigned char increment);

