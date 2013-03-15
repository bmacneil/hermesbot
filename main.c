#include <stdio.h>
#include <p18f4525.h>
#include <timers.h>
#include "sumovore.h"
#include "motor_control.h"
#include "interrupts.h"
#include "robot_test.h"

void Robot_track_end(void);

// main acts as a cyclical task sequencer
void main(void)
{
           OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_64);
    initialization(); // function from sumovore.c
                      // it sets up pwm (using timer2),
                      // IO pins, the ADC, the 
                      // USART and the default
                      // threshold
    printf("\n\rKwantlen APSC1299 simple curve follower -- with error codes\n\r"); 
    ClrWdt();         // defined in <p18f4525.h>

  threshold = 650u; // to change from default value
                     // uncomment and change to any unsigned int <1024u -- most usually <512u

    while(1)
    {

       check_sensors();    // from sumovore.c
        set_leds();         // function from sumovore.c
        switch(SeeLine.B) // this is a switch based on the sensor values, SeeLine.B allows us to input sensor states as a 5 bit binary value
							// it is already built into sumovore.c , so 0b11111 is all the sensors are on and 0b00000 is all sensors off
        {
            case 0b00100: motor_control();	
           	   break;
            case 0b01110: motor_control();	
           	   break; 
            case 0:	Robot_track_end();
               break; 
 
        } 
      motor_control();    // function from motor_control.c 
       ClrWdt();           // defined in <p18f4525.h>
		if(SeeLine.B==0)
			{

			}
      if(lvd_flag_set())  LVtrap();
    }
}




void Robot_track_end(void)
{
	WriteTimer0(0);
		while((ReadTimer0()<40000) && (SeeLine.B==0))
			{
			  set_motor_speed(left, medium, 0); 
			  set_motor_speed(right, medium, 0); 
			}
		if(ReadTimer0()>40000  )
			{	
				while(SeeLine.B==0)
				{	
				  set_motor_speed(left, rev_slow, 0); 
				  set_motor_speed(right, rev_slow, 0); 
				}
			}
}

