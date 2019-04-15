/*
 * temp.h
 *
 *  Created on: Jun 27, 2018
 *      Author: Owner
 */

#ifndef TEMP_H_
#define TEMP_H_



void configADC(void);
void tempStr(volatile float t,char type,int x, int y);
void ini_temps(volatile long int timer);


#endif /* TEMP_H_ */
