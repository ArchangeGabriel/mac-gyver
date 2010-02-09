// Alim.c / Gestion de la consommation //

#include <pic18fregs.h>
#include "common_types.h"
#include "pindebug.h"
#include "pining.h"


uchar memory = 0;

void application_suspend(void)
{
    memory = PINDEBUG + (PINALIM<<1);
    PINDEBUG = 0;
    PINALIM = 0;
    CCPR1L = 0x00; // Rapport cyclique 1 = 0
    CCPR2L = 0x00; // Rapport cyclique 2 = 0
}

void application_wakeup(void)
{
    PINDEBUG = memory & 0x01;
    PINALIM = memory>>1;
    memory = 0;
    CCPR1L = 0x80; // Rapport cyclique 1 = 1/2
    CCPR2L = 0x80; // Rapport cyclique 2 = 1/2
}

char set_device_remote_wakeup(void)
{
    return TRUE; // Remote wakeup unavailable
}

char clear_device_remote_wakeup(void)
{
    return TRUE; // Remote wakeup unavailable
}
