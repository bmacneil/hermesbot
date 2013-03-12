//-------------------------------------------------------------------------------------------------
// Geoff Roeder
// Filename: Assignment4.c
// Date: 11 March 2013
// Course: APSC 1299, R10
// Purpose: Use PIC18F4525 timer0 and ADC module to determine the maximum voltage, minimum voltage,
// and period of a sine wave signal, known to be between 0V and 5V.
//
// Note on implementation: in order to analyze slower sinusoidal signals (a few hundred Hz), the 
// oscillator speed is 1MHz. Since 1 TOSC is 1 microsecond, the A/D acquisition timeselect is set to 
// n=0 in order to sample the continuous waveform quickly. The minimum amount of time 
// needed for 1 TAD (hardware limit) is 0.7 microseconds. This is covered by the A/D clock
// source set at the minimum 2 (2 microseconds).
//
//-------------------------------------------------------------------------------------------------

#include <p18F4525.h>
#include <stdio.h>
#include <adc.h>
#include <delays.h>
#include <timers.h>
#include "..\functions\osc.h"
#include "..\functions\configureUSART.h"
#include "..\functions\serLCD.h"
#include "..\functions\configuration_bits.h"
#include "..\functions\PrintFloat_nFS.h"

unsigned int firstADCval, secondADCval, 
		   compareVal, firstTempADC, secondTempADC; // comparison variables to find target wave features

float minADCval, maxADCval, wavePeriodTCY; // store target values

const float ADC_TO_V = 0.0048828125,  // conversion factor from ADC to voltage = (5V-0V / 2^10)
			TCY_AMT = 0.000004; 	  // 1 TCY = 4 TOSC = 4 / (1*10^6 Hz)

unsigned char foundMin = 0,
			  foundMax = 0,
			  foundPeriod = 0; // flags for logic flow

// function prototypes, see definitions for descriptions
void FindMinVal (void);
void FindMaxVal (void);
void FindPeriod(void);
void ProcessResults(void);
void DisplayResults(void);
void ClearFlags(void);

void main (void)
{
	set_osc_1MHz(); // 1TOSC = 1microsecond, 1TCY = 4 microseconds

    configureUSART(9600ul, 1); // configure MCU serial communication module to run at 9600 baud 
                                 // with MCU operating at 1 MHz. Defined in configureUSART.c
        		                 // 9600 bits/s is default communication rate  

    Delay10TCYx(1);   // small delay to wait for splash screen to appear  
	
	printf("%c",0x12);  // reset LCD to default 9600 bps

	Delay10KTCYx(25); // 25*10000*4/(1*10^6) = 1.0s
	Delay10KTCYx(25); // Two second delay for LCD to initalize

	TRISAbits.TRISA0 = 1; // pin 2 AN0 set to input for ADC

	// Configure Timer0 for prescale 1 and 16bit operation. Maximum timer length: 0.262144 s

	OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);

	// Configure pin AN0 for ADC operation using VDD and VSS as
  	// the reference. The digital value is right justified.
 
  	OpenADC(ADC_RIGHT_JUST & ADC_FOSC_2 & ADC_0_TAD, ADC_CH0 & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 14);
  	// 1 TAD = 2 TOSC; each sample and conversion takes 11TAD + 2TAD before next read
  	Delay10TCYx(5);              // Delay for 50TCY to stabilize
  	SetChanADC(ADC_CH0);         // Read from pin AN0

  while(1)
  {
	// Take baseline reading to determine which waveform feature to search for first.
	// Optimizes search algorithm for speed.

	ConvertADC();                // Start ADC operation
	while( BusyADC() );          // Wait for completion
	firstADCval = ReadADC();   // Read result     
	ConvertADC();                // Start ADC operation
	while( BusyADC() );          // Wait for completion
	secondADCval = ReadADC();  // Read result

	// test values: give contrl to max or min locator function depending
	// on rising or falling trend of two points taken

	if ((firstADCval > secondADCval) && !foundMin) // slope is falling, look for minimum
	{
		  FindMinVal();
	} 

	if ((firstADCval < secondADCval) && !foundMax) // slope is rising, look for maximum 
	{
		  FindMaxVal();
		  
	}

	if (foundMax && foundMin) // execute only once max and min values found
	{
		FindPeriod();
		ProcessResults();
		DisplayResults();
		ClearFlags();
	}
	// sample and display loop continues until device is powered off
  }
}

//*******************************************************************************
// FindMinVal locates the smallest value of the osciallating waveform in ADC	*
// by successive comparisons until the slope of the two sampled points on the	*
// unknown waveform switches from falling to rising.							*
// 																				*
// The function has no return type. The value of the minimum ADC measured is 	*																				*
// stored in a global variable.													*
//																				*
// The function uses a test flag to return control to the main function once	*
// the value has been found.													*
//*******************************************************************************

void FindMinVal (void)
{
	minADCval = secondADCval;
	// repeat loop until the minimum value is found
	while (!foundMin)
	{
			// take another reading from ADC
			ConvertADC();                // Start ADC operation
			while( BusyADC() );          // Wait for completion
			compareVal = ReadADC();   // Read result   
			if (compareVal < minADCval)
				minADCval = compareVal; // update the lowest known value
			else if (compareVal > minADCval)
				foundMin = 1; // minimum value found, set flag to true
	}
}

//*******************************************************************************
// FindMaxVal locates the largestvalue of the osciallating waveform in ADC		*
// by successive comparisons until the slope of the two sampled points on the	*
// unknown waveform switches from rising to falling.							*
// 																				*
// The function has no return type. The value of the maximum ADC measured is 	*																				*
// stored in the global variable maxADCval.										*
//																				*
// The function uses a test flag to return control to the main function once	*
// the value has been found.													*
//*******************************************************************************

void FindMaxVal (void)
{
	maxADCval = secondADCval;
	// repeat loop until the maximum value is found
	while (!foundMax)
	{
			// take another reading from ADC
			ConvertADC();                // Start ADC operation
			while( BusyADC() );          // Wait for completion
			compareVal = ReadADC();   // Read result into temp variable

			if (compareVal > maxADCval)
				maxADCval = compareVal; // change the maximum value
			else if (compareVal < maxADCval)
				foundMax = 1; // maximum value found
	}
}

//*******************************************************************************
// FindPeriod accepts no arguments. The function has no return type.			*
// 																				*
// The function takes two readings from ADC and then begins Timer0. The 		*
// function enters a loop and takes successive readings until it locates two	*
// points, one full cycle away from the original points, by comparison in term 	*
// of the two points slopes and their ADC read value. When both the slope 		*
// and the two values match, the value of the timer is read into a global		*
// variable. The foundPeriod flag is set and the function returns to main.		*
//																				*
//*******************************************************************************
				
void FindPeriod (void)
{
	INTCONbits.TMR0IF = 0; // Reset timer bit	

	// take two fresh readings so that timing function is accurate
	ConvertADC();                // Start ADC operation
	while( BusyADC() );          // Wait for completion
	firstADCval = ReadADC();   // Read result     
	ConvertADC();                // Start ADC operation
	while( BusyADC() );          // Wait for completion
	secondADCval = ReadADC();  // Read result
	WriteTimer0(0); // begin timer at t= 11 + 2 (refresh delay) + 11 TAD = 48 microseconds
					// timer offset is +48 microseconds

	while (!foundPeriod)
	{
		// sample approx. 24TAD = 48 microseconds + comparison time until 
		// the same rising slope point and value found after one full cycle

		ConvertADC();                // Start ADC operation
		while( BusyADC() );          // Wait for completion
		firstTempADC = ReadADC();   // Read result     
		ConvertADC();                // Start ADC operation
		while( BusyADC() );          // Wait for completion
		secondTempADC = ReadADC();  // Read result

		// if the first sample point is below the first ADC read value and the second
		// sample point is above the first ADC read value, the point has been found. The
		// if-condition permits one or the other value to be equal to the firstADCvalue
		// in case one or the other ADC values happen to be the same. Both can't be equal, though,
		// or the function would have misidentified the target point on the wave.

		if (((firstTempADC <= firstADCval) && (secondTempADC > firstADCval))
			|| ((firstTempADC < firstADCval) && (secondTempADC >= firstADCval)))
		{
			//one complete cycle found, capture time
			wavePeriodTCY = ReadTimer0(); // hold value for processing into period
			foundPeriod = 1; // exit while loop, return to main
		}
	}
}

//*******************************************************************************
// ProcessResults calculates the maximum and minimu voltages, and the period of	*
// the waveform. The function accepts no arguments and has no return type.		*
//																				*
// ProcessResults stores the results of these values in the variables  			*
// themselves rather than intermediate variables.								*
// 																				*
// Floating point data type is used to preserve the precision of the value.		*
//*******************************************************************************

void ProcessResults(void)
{
	 // Convert ADC to voltage (stored as float)
	minADCval = (minADCval + 0.5f)*(ADC_TO_V);
	maxADCval = (maxADCval + 0.5f)*(ADC_TO_V);

	//Convert wavePeriodTOSC ADC to seconds (stored as float)
	wavePeriodTCY = wavePeriodTCY*TCY_AMT + 0.000048f; // TOSC * TCY * Prescale (1) + timer offset
}

//*******************************************************************************
// DisplayResults takes no arguments and has no return type.					*
//																				*
// The function calls special floating point printing functions to correctly	*
// display the results of the calculations on a Hitachi HD44780U LCD screen		*
//																				*
//*******************************************************************************

void DisplayResults(void)
{
	LCD_ClearDisplay(); // Clear display

	LCD_SetRowCol(0,0); // print across top row the min and the max voltage
	PrintFloat4FS(minADCval); // printfloat s value to 4 decimal places
	printf("  "); // add spacing on LCD
	PrintFloat4FS(maxADCval);

	LCD_SetRowCol(1,0);
	PrintFloat4FS(wavePeriodTCY);
}


//*******************************************************************************
// ClearFlags takes no arguments and has no return type.						*
//																				*
// The function clears all flags so that the control structure in main will 	*
// restart, sampling the next waveform.											*
//*******************************************************************************

void ClearFlags(void)
{
	// Clear flags for next reading
	foundMin = 0;
	foundMax = 0;
	foundPeriod = 0;
	INTCONbits.TMR0IF = 0;
}






