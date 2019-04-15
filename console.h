/*
 * console.h
 *
 *  Created on: Jun 14, 2017
 *      Author: deemer
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <msp430.h>

void ConsoleInit(void);

/**
 * Send size bytes starting at address data via the serial console
 * @param data Starting address of data to send
 * @param size  Number of bytes to send
 */
void ConsoleSendBytes(char *data, unsigned int size);


/**
 * Send a string of up to max_size bytes via the serial console
 * The string may be null-terminated, in which case only characters
 * before the null terminator will be sent.
 * @param str Starting address of data to send
 * @param size  Number of bytes to send
 */
void ConsoleSendString(char *str, unsigned int max_size);


/**
 * Send a string of up to max_size bytes via the serial console
 * The string may be null-terminated, in which case only characters
 * before the null terminator will be sent.
 * A newline character is added at the end of the string provided.
 * @param str Starting address of data to send
 * @param size  Number of bytes to send
 */
void ConsoleSendLine(char *str, unsigned int max_size);


#endif /* CONSOLE_H_ */
