// Alim.c / Gestion de la consommation //

#include <pic18fregs.h>
#include "common_types.h"
#include "pindebug.h"
#include "pining.h"
#include "moteurs.h"

uchar memory = 0;

void application_suspend(void)
{
    OSCCONbits.IDLEN = 0;
    INTCON2bits.RBPU = 1; // Disable port B pull-ups
    memory = PINDEBUG + (PINALIM<<1) + (OSCCONbits.IDLEN<<2);
    PINDEBUG = 0;
    PINALIM = 0;
    set_motors(0);
  //  CCPR1L = 0x00; // Rapport cyclique 1 = 0
  //  CCPR2L = 0x00; // Rapport cyclique 2 = 0
}

void application_wakeup(void)
{
    INTCON2bits.RBPU = 0; // enable port B pull-ups
    PINDEBUG = memory & 0x01;
    memory >>= 1;
    PINALIM = memory & 0x01;
    memory >>= 1;
    OSCCONbits.IDLEN = memory & 0x01;
    memory = 0;
 //   CCPR1L = 0x80; // Rapport cyclique 1 = 1/2
 //   CCPR2L = 0x80; // Rapport cyclique 2 = 1/2
}
