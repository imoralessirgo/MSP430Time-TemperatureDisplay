/************** ECE2049 DEMO CODE ******************/
/************** 14 May 2018   **********************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
#include "utils/test_runner.h"
#include "date.h"
#include "time.h"
#include "temp.h"
#include "console.h"
#include "log.h"


// Function Prototypes
void swDelay(char numLoops);
void startTimerA2(void);
void stopTimerA2(void);
unsigned int readScroll(void);

// timer
volatile long int timer = 5674839;
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void)
{
    timer++;
}


// Declare globals here

// Main
void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    // *** System initialization ***
    initLeds();
    initBtns();
    configDisplay();
    configKeypad();
    configADC();


    ConsoleInit();
    long int stime;
    _enable_interrupt();
    startTimerA2();
    ini_date(timer);
    ini_time(timer);
    ini_temps(timer);
    int state=0;
    int start = timer;
    while (timer<31536001)// Year long loop *365 days*
    {

        int pressed=readBtns();
        if ((pressed & BIT1) && ((timer-start)>30)){
            dispLog();
        }
        if (pressed & BIT3){
            BuzzerOn(readScroll());
            state=1;
        }
        if (state){
            BuzzerOn(readScroll());
        }
        if (pressed & BIT2){
            BuzzerOff();
            state=0;
        }

        while(stime != timer){ //enters loop once every second
            stime = timer;
            ini_date(timer);
            ini_temps(timer);
            ini_time(timer);
        }

    }
    stopTimerA2();
}


void startTimerA2(void)
{
    /*
     * This function configures and starts Timer A2
     * Timer is counting in intervals of ~0.005s
     *
     * smj, ECE2049, 17 September 2013
     */

    TA2CTL = TASSEL_1 | MC_1 | ID_0;
    TA2CCR0 = 32767; //  ACLK ticks 1 second
    TA2CCTL0 = CCIE; // Enable TA2CCR0 interrupts
}

void stopTimerA2(void)
{
    /*
     * This function stops Timer A2
     */
    TA2CTL = MC_0; // Stop timer
    TA2CCTL0 &= ~CCIE; // Disable TA2CCR0 interrupts
}


void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i,j;	// volatile to prevent removal in optimization
			                    // by compiler. Functionally this is useless code

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}


unsigned int readScroll(void){
    unsigned int in_value ;
    ADC12CTL0 &= ~ ADC12ENC ; // Disable ADC for configuration
    // Core configuration
    ADC12CTL0 = ADC12SHT0_9 | ADC12ON ;
    ADC12CTL1 = ADC12SHP ;
    // ADC channel configuration ( Use 3.3 V reference , input channel 0)
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_0 ;
    // Analog input A0 is shared with P6 .0 , so set pin to function mode
    P6SEL |= BIT0 ;
    ADC12CTL0 |= ADC12ENC ; // Configuration done , now enable ADC
    // ***** Now we can start the conversion *****
    // Note : in your lab , you may want to separate the configuration
    // and conversion parts into separate functions . Depending on how
    // you use it , you may only need to configure the ADC once .
    ADC12CTL0 &= ~ ADC12SC ; // Clear the start bit
    ADC12CTL0 |= ADC12SC ; // Start conversion
    // Poll busy bit waiting for conversion to complete
    while ( ADC12CTL1 & ADC12BUSY ) {
        }
        in_value = (ADC12MEM0 & 0x0FFF); // only keep low 12 bits
    return in_value;
}
