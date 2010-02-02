/*-------------------------------------------------------------------------
  main.c - Application main function

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------*/

#include <pic18fregs.h>
#include "common_types.h"
#include "boot_iface.h"
#include "servos.h"
#include "analogs.h"
#include "digitals.h"
#include "ep2.h"
#include "moteurs.h"

#define ftoggle_A4() { PORTAbits.RA4 = !PORTAbits.RA4; }

// beware : this is not a C main function, but the application
// entry point called from the boot.

void application_main(void) 
{
    TRISA = 0xEF;
    PORTA = 0x10;
    
    // Reset the Timer0 value
    TMR0H = 0;
    TMR0L = 1;

    init_motors();
    setup_adconversion();
    init_servos();
    init_digitals_in();
    
    // Interruptions
    PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!

    INTCONbits.PEIE = 1; // Interrupts peripheriques (usb, timer 1...)
    INTCONbits.GIE = 1; // Interrupts global

    while(usb_active_cfg > 2)
    {
        usb_sleep();
        dispatch_usb_event();
    }
}

/* Interrupt vectors */
#pragma code high_priority_isr 0x2020
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

