/*-------------------------------------------------------------------------
  main.c - Application main function
-------------------------------------------------------------------------*/

#include <pic18fregs.h>
#include "common_types.h"
#include "boot_iface.h"
#include "servos.h"
#include "analogs.h"
#include "digitals.h"
#include "ep2.h"
#include "moteurs.h"
#include "findecourse.h"
#include "usb_status.h"
#include "pindebug.h"
#include "suspend.h"
#include "debugs.h"

// beware : this is not a C main function, but the application
// entry point called from the boot.

void application_main(void) 
{
    unsigned char toto[2][4]={{0x05,0x01,0x00,0xf0}, // 00000101, 00000001, 00000000, 11110000
                              {0x0A,0x02,0x00,0xf0}};// 00001010, 00000010, 00000000, 11110000

    // Reset the Timer0 value
    TMR0H = 0;
    TMR0L = 1;

    setalim();

    init_motors();
    setup_adconversion();
    init_servos();
    init_digitals_in();
    init_debug();
    init_fdc();

    set_fdc((unsigned char *)toto,2);

    SET_DEVICE_STATUS(REMOTE_WAKEUP_DIS);
    
    // Interruptions
    PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!

    INTCONbits.PEIE = 1; // Interrupts peripheriques (usb, timer 1...)
    INTCONbits.GIE = 1; // Interrupts global

    while(usb_active_cfg > 2)
    {
        usb_sleep();
        dispatch_usb_event();
    }
    cutalim();
    PINDEBUG = 0;
//    clear_device_remote_wakeup();
}

/* Interrupt vectors */
#pragma code high_priority_isr 0x2030
void high_priority_isr(void) interrupt
{
    ////////////////////////// Servos ////////////////
    if(PIR1bits.TMR1IF)
    {
	//gestionservos_tmr1();
	gestionservos();
	PIR1bits.TMR1IF = 0;
    }
/*    if(PIR1bits.TMR2IF)
    {
	gestionservos_tmr2();
	PIR1bits.TMR2IF = 0;
    }*/
    ///////////////////////// AD conversion /////////
    if(PIR1bits.ADIF)
    {
        maj_result();
	PIR1bits.ADIF = 0;
    }
    if(INTCONbits.INT0IF || INTCON3bits.INT1IF)
    {
        gestionfdc();
        INTCONbits.INT0IF = 0;
        INTCON3bits.INT1IF = 0;
    }
}

#pragma code low_priority_isr 0x4000
void low_priority_isr(void) interrupt
{
}

