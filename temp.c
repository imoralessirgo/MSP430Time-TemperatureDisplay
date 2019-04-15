/*
 * temp.c
 *
 *  Created on: Jun 27, 2018
 *      Author: Owner
 */


#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "temp.h"
#include "peripherals.h"
#include "log.h"

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)



void configADC(void){
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = ADC12SHT0_9 | ADC12REFON | ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;
    __delay_cycles(100);
    ADC12CTL0 |= ADC12ENC;
}

void tempStr(volatile float t,char type,int x, int y){
    char temp[7]={'0','0','0','0','0','0','\0'};
    int tn,u,d;
    int ttn;
    t *= 10;
    tn=((int)t/100);
    ttn=((int)t%100);
    u= ttn/10;
    d= ttn%10;

    temp[0]= '0'+tn;
    temp[1]= '0'+u;
    temp[3]= '0'+d;
    temp[2]='.';
    temp[4]=' ';
    temp[5]= type;

    Graphics_drawStringCentered(&g_sContext, temp , AUTO_STRING_LENGTH, x,
                                    y, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);

}

void ini_temps(volatile long int timer){
    configADC();
    ADC12CTL0 &= ~ADC12SC;  // clear the start bit
    ADC12CTL0 |= ADC12SC;
    volatile float temperatureDegC;
    volatile float temperatureDegF;
    volatile unsigned int bits30 = CALADC12_15V_30C;
    volatile unsigned int bits85 = CALADC12_15V_85C;
    volatile float degC_per_bit = ((float)(85.0 - 30.0))/((float)(bits85-bits30));
    while (ADC12CTL1 & ADC12BUSY)
        __no_operation();
    long in_temp = ADC12MEM0;
    temperatureDegC = (float)((long)in_temp - CALADC12_15V_30C) * degC_per_bit +30.0;
    tempStr(temperatureDegC,'C',48,50);
    temperatureDegF = temperatureDegC * 9.0/5.0 + 32.0;
    log(timer,temperatureDegC);
    tempStr(temperatureDegF,'F',48,60);
}

