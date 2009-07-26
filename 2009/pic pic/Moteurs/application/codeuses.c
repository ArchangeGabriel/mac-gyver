// codeuses.c //

#include "codeuses.h"

long codeusecnt[2];

void init_codeuses(void)
{
    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.INTEDG1 = 0;
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    codeusecnt[0] = 0;
    codeusecnt[1] = 0;
}


