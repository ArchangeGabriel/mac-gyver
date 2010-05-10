/*-------------------------------------------------------------------------
  main.c - Application main function
-------------------------------------------------------------------------*/

#include <pic18fregs.h>
#include "common_types.h"
#include "boot_iface.h"
#include "usb_status.h"

#include "codeuses.h"
#include "suspend.h"
#include "pining.h"
#include "pindebug.h"

#include "ep2.h"
#include "app_usb.h"

// beware : this is not a C main function, but the application
// entry point called from the boot.

void init_pwm(void) // Configure CCP1, CCP2 et Timer2
{
    PR2 = 0xFF; //periode Timer2 = 256
    CCPR1L = 0x80; // Rapport cyclique 1 = 1/2 (pour les bits de poids faibles, voir CCP1CON bits 4 et 5)
    CCPR2L = 0x80; // Rapport cyclique 2 = 1/2
    TRISC &= 0xF9; // 11111001 : CCP1, CCP2 = sorties
    T2CON = 0x7D; // timer2 conf = 0 1111 1 01 : nU, postscaler 1:16, on, prescaler 1:4
    CCP1CON = 0x0C; // CCP1 conf = 00 00 1100 : nU, bits poids faibles, PWM mode
    CCP2CON = 0x0C; // CCP2 conf...
}

void application_main(void) 
{
//    TRISDEBUG = 0;
//    PINDEBUG = 1;
    
    setalim();
    init_pwm();
    init_codeuses();
    
    SET_DEVICE_STATUS(REMOTE_WAKEUP_DIS);

    // Interruptions
    
    PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!

    INTCONbits.GIE = 1; // Interrupts global

    while(usb_active_cfg > 2)
    {
        usb_sleep();
        if(INTCONbits.TMR0IF)
        {
            sendcodeuses();
            INTCONbits.TMR0IF = 0;
        }
        dispatch_usb_event();
    }
    cutalim();
    TRISC |= 0x06;
    PINDEBUG = 0;
}


/* Interrupt vectors */
#pragma code high_priority_isr 0x2030
void high_priority_isr(void) interrupt
{
    ////////////////////////////////// Codeuses ////////////////////
    if(INTCON3bits.INT1IF)
    {
        if(PINSSCOD1) codeusecnt[1]++; else codeusecnt[1]--;
        INTCON3bits.INT1IF = 0;
    }
    if(INTCONbits.INT0IF)
    {
        if(PINSSCOD0) codeusecnt[0]++; else codeusecnt[0]--;
        INTCONbits.INT0IF = 0;
    }
}

#pragma code low_priority_isr 0x4000
void low_priority_isr(void) interrupt
{
}
