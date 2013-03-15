/* file osc.h                                                */
/*                                                           */
/* adapted from July 2006 osc.h by Mike Coombes              */
/* function moved to osc.c  Nov. 7, 2006 (by Dan Peirce)     */
/* Set Clock speed to 32 MHz                                 */
/*                                                           */
// #ifndef __OSC_H
// #define __OSC_H

#define _OSC_SPEED  32000000

void set_osc_32MHz(void); 
void set_osc_16MHz(void); 
void set_osc_8MHz(void); 
void set_osc_4MHz(void); 
void set_osc_2MHz(void); 
void set_osc_1MHz(void); 

// #endif
