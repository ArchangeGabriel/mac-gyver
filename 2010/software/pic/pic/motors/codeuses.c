// codeuses.c //

#include "codeuses.h"
#include "ep1.h"
#include "ep2.h"
//#include "../../../io/comm.h"
#include "pining.h"

#include "proto.h"

long* codeusecnt;
//long mesdonnees[4];
char mydata[10];



void init_codeuses(void)
{
    unsigned char i;
    T0CON = 0x89; // TMR0ON, 16bits, CLKO, PSA on, 1:4
    INTCONbits.TMR0IE = 1; // Interrupt Timer 0
    INTCONbits.TMR0IF = 0; // Interrupt Timer 0
    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.INTEDG1 = 0;
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
//    for(i=1;i<10;i++) mydata[i]=i;//0;//mesdonnees[i]=0;
    mydata[0]=CODERS;
    mydata[1]=0;
    codeusecnt=(long*)(mydata+(2*sizeof(char)));
    codeusecnt[0] = 0x01020304;
    codeusecnt[1] = 0x05060708;
    //mesdonnees[0] = CODER; //mydata..
    //codeusecnt = (long*)(&mydata[8]);
    //codeusecnt = &(mesdonnees[2]);
}

void sendcodeuses()
{
    //char i;
    //mesdonnees[0] = CODER;
    mydata[0]=CODERS | (PINSSCOD1<<1) | (PINSSCOD0);//CODER;
    mydata[1]++;
    ep2_source_data = (unsigned char*)mydata;//mesdonnees;
    ep2_num_bytes_to_send = 10; //16;
    //for(i=0;i<8;i++) mydata[i+8]=((unsigned char*)codeusecnt)[i];
    my_prepare_ep2_in();
}
