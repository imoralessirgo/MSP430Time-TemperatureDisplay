/*
 * time.c
 *
 *  Created on: Jun 27, 2018
 *      Author: Owner
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <msp430.h>
#include "peripherals.h"


long int days;
long int rdays;
long int hours;
long int rhours;
long int minutes;
long int seconds;



void print_time(long int h,long int m,long int s){
    int t;
    int u;
    char time[9];
    t = h/10;
    u = h%10;
    time[0]= '0' + t;
    time[1]= '0' + u;
    time[2]= ':';
    t = m/10;
    u = m%10;
    time[3]= '0' + t;
    time[4]= '0' + u;
    time[5]= ':';
    t = s/10;
    u = s%10;
    time[6]= '0' + t;
    time[7]= '0' + u;
    time[8]= '\0';
    Graphics_drawStringCentered(&g_sContext, time , AUTO_STRING_LENGTH, 48,
                                    30, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void ini_time(volatile long int timer){
    days    =   timer / 86400;
    rdays   =   timer % 86400;
    hours   =   rdays / 3600;
    rhours  =   rdays % 3600;
    minutes =   rhours / 60;
    seconds =   rhours % 60;
    print_time(hours,minutes,seconds);
}





