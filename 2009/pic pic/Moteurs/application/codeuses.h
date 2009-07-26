// codeuses.h //

#ifndef _CODEUSES_H
#define _CODEUSES_H

#include <pic18fregs.h>

#define PIN_SSCOD0 PORTBbits.RB2
#define PIN_SSCOD1 PORTBbits.RB3

extern long codeusecnt[2];

extern void init_codeuses(void);

#endif
