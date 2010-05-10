// codeuses.c //

#include "codeuses.h"
#include "ep1.h"
#include "ep2.h"
#include "pining.h"

#include "proto.h"
#include "app_usb.h"

unsigned long codeusecnt[2];
unsigned long *codeusepointer;
char mydata[10];



void init_codeuses(void)
{
    // Reset the Timer0 value
    TMR0H = 0;
    TMR0L = 0;

    T0CON = 0x89; // 1 0 0 0 1 001 : TMR0ON, 16bits, CLKO, PSA on, 1:4
    INTCONbits.TMR0IE = 0; // Interrupt Timer 0
    INTCONbits.TMR0IF = 0; // Interrupt Timer 0
    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.INTEDG1 = 0;
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    mydata[0]=CODERS;
    mydata[1]=0;
    codeusepointer=(unsigned long*)(&mydata[2]);
    codeusecnt[0] = 0x00000000;
    codeusecnt[1] = 0x00000000;
}

void sendcodeuses()
{
    while(EP_IN_BD(2).Stat.uc & BDS_USIE); // wait the Uown bit to be cleared
    mydata[0]=CODERS | (PINSSCOD1<<1) | (PINSSCOD0);
    mydata[1]++;
    codeusepointer[0] = codeusecnt[0]; 
    codeusepointer[1] = codeusecnt[1]; 
    ep2_source_data = (unsigned char*)mydata;
    ep2_num_bytes_to_send = 10;
    my_prepare_ep2_in();
}
