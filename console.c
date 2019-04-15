/*
 * console.c
 *
 *  Created on: Jun 14, 2017
 *      Author: deemer
 */

#include "console.h"

/**
 * Prototypes local to this function
 * console.h defines prototypes for other functions
 * in this file that we want to be available when
 * console.h is included in other code
 *
 */
void ConsoleSendChar(char x);

/*
 * Configure serial console for operation
 * NOTE:  This function currently assumes default clock frequencies.
 * If you need to change the clock frequencies, you will need to
 * change the baud rate computation here--see Section 36.3 of
 * the User's Guide (specifically Table 36-5) for details.
 */
void ConsoleInit(void)
{
	// Configure USCI Tx and Rx pins for function mode
	P4SEL |= (BIT4|BIT5);

	// Disable USCI for configuration
	UCA1CTL1 |= UCSWRST;

	// Configure USCI for UART, use SMCLK as clock source
	// Configure for 9600 baud, 8 bit mode, no parity bit, 1 stop bit
	UCA1CTL0 = UCMODE_0;
	UCA1CTL1 = UCSSEL__SMCLK | UCSWRST;

	// Configure baud rate (see Table 36-5 of User's guide)
	UCA1BR1 = 0;
	UCA1BR0 = 6;

	UCA1MCTL = UCBRF_13 | UCBRS_0 | UCOS16;

	// Configuration complete, so enable USCI
	UCA1CTL1 &= ~UCSWRST;
}

// Send a single character via the serial console
inline void ConsoleSendChar(char x)
{
	// Ensure the USCI is ready to send data
	while(!(UCA1IFG & UCTXIFG)) {
		__no_operation();
	}

	// Transmit character by loading it into the Tx buffer
	UCA1TXBUF = x;

	// Wait for the transmit operation to be done
	while(!(UCA1IFG & UCTXIFG)) {
		__no_operation();
	}
}


/**
 * Send size bytes starting at address data via the serial console
 * @param data Starting address of data to send
 * @param size  Number of bytes to send
 */
void ConsoleSendBytes(char *data, unsigned int size)
{
	unsigned int i;
	for(i = 0; i < size; i++) {
		ConsoleSendChar(data[i]);
	}
}


/**
 * Send a string of up to max_size bytes via the serial console
 * The string may be null-terminated, in which case only characters
 * before the null terminator will be sent.
 * @param str Starting address of data to send
 * @param size  Number of bytes to send
 */
void ConsoleSendString(char *str, unsigned int max_size)
{
	unsigned int i;

	for(i = 0; i < max_size; i++) {

		// Stop transmitting if we hit a null terminator
		if(str[i] == '\0') {
			break;
		}

		ConsoleSendChar(str[i]);
	}
}

/**
 * Send a string of up to max_size bytes via the serial console
 * The string may be null-terminated, in which case only characters
 * before the null terminator will be sent.
 * A newline character is added at the end of the string provided.
 * @param str Starting address of data to send
 * @param size  Number of bytes to send
 */
void ConsoleSendLine(char *str, unsigned int max_size)
{
	ConsoleSendString(str, max_size);
	ConsoleSendString("\r\n", 2);
}
