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
#include "usb_status.h"
#include "pindebug.h"

// beware : this is not a C main function, but the application
// entry point called from the boot.

void application_main(void) 
{
    TRISDEBUG = 0;
    PINDEBUG = 1;
    
    // Reset the Timer0 value
    TMR0H = 0;
    TMR0L = 1;

    init_motors();
    setup_adconversion();
    init_servos();
    init_digitals_in();

    SET_DEVICE_STATUS(DEVICE_BUS_POWERED | REMOTE_WAKEUP_DIS);
    
    // Interruptions
    PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!

    INTCONbits.PEIE = 1; // Interrupts peripheriques (usb, timer 1...)
    INTCONbits.GIE = 1; // Interrupts global

    while(usb_active_cfg > 2)
    {
        usb_sleep();
        dispatch_usb_event();
    }
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
	gestionservos_tmr1();
	PIR1bits.TMR1IF = 0;
    }
    if(PIR1bits.TMR2IF)
    {
	gestionservos_tmr2();
	PIR1bits.TMR2IF = 0;
    }
    ///////////////////////// AD conversion /////////
    if(PIR1bits.ADIF)
    {
        maj_result();
	PIR1bits.ADIF = 0;
    }
}

#pragma code low_priority_isr 0x4000
void low_priority_isr(void) interrupt
{
}

