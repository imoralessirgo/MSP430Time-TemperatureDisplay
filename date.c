/*
 * date.c
 *
 *  Created on: Jun 27, 2018
 *      Author: Owner
 */

#include <stdio.h>
#include <stdlib.h>
#include "date.h"
#include <string.h>
#include <msp430.h>
#include "peripherals.h"

char month[12][3]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
int size_month[12]={31,28,31,30,31,30,31,31,30,31,30,31};



void print_date(int mi, int day){
    char tdate[7]={0,0,0,0,0,0,'\0'};
    memcpy(tdate,month[mi],3);
    tdate[3]=',';
    int t;
    int u;
    t = day/10;
    u = day%10;
    tdate[4]= '0' + t;
    tdate[5]= '0' + u;
    Graphics_drawStringCentered(&g_sContext, tdate , AUTO_STRING_LENGTH, 48,
                                    20, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);

}


void ini_date(volatile long int timer){
    int rday,day;
    day    =   timer / 86400;
    rday   =   timer % 86400;
        day++;
    int i;
    for (i=0;day > size_month[i];i++){
        day -= size_month[i];
    }
    print_date(i,day);
}





