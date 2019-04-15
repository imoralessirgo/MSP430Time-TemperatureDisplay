/*
 * log.c
 *
 *  Created on: Jun 28, 2018
 *      Author: Owner
 */

#include "log.h"
#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "console.h"

#define log_size (120)

typedef struct logg{
    volatile long int timestamp;
    volatile float temp;
}RECORD;

RECORD LOG[log_size];

unsigned int indx=0;

void log(volatile long int timer,volatile float temp){
    LOG[indx].timestamp = timer;
    LOG[indx].temp = temp;
    indx++;
    if (indx == log_size){
            indx=0;
        }
}


void makestr(volatile long int timer,volatile float temp){
    int tt,tthou,thou,thun,hun,ten,uni;
    tt= timer%1000;
    thou = tt/1000;
    tthou = tt%1000;
    hun = tthou/100;
    thun = tthou%100;
    ten = thun/10;
    uni = thun%10;
    char logent[9]={0,0,0,0,0,0,0,0,'\0'};
    logent[0] = '0' + thou;
    logent[1] = '0' + hun;
    logent[2] = '0' + ten;
    logent[3] = '0' + uni;
    logent[4] = ',';

    int tn,u,d;
    int ttn;
    temp *= 10;
    tn=((int)temp/100);
    ttn=((int)temp%100);
    u= ttn/10;
    d= ttn%10;

    logent[5]= '0'+tn;
    logent[6]= '0'+u;
    logent[8]= '0'+d;
    logent[7]='.';
    ConsoleSendLine(logent, 9);

}
void dispLog(void){
    int i;
    for(i = 0; i < 30; i++) {
        int idx = ((indx - 1) - i);
        if (idx < 0) {
         idx += log_size;
        }

        makestr(LOG[idx].timestamp,LOG[idx].temp);
    }
}
