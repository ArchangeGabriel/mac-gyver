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
#include "my_ep2.h"
#include "moteursetjack.h"

#define ftoggle_A4() { PORTAbits.RA4 = !PORTAbits.RA4; }

// beware : this is not a C main function, but the application
// entry point called from the boot.

void application_main(void) 
{
    TRISA = 0xEF;
    PORTA = 0x10;
    
    // Reset the Timer0 value
    TMR0H = 0;
    TMR0L = 0;

    init_moteursetjack();
    init_adconversion();
    init_servos();
    
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
    ///////////////////////// Moteurs et jack ////////
    if(INTCONbits.TMR0IF)
    {
        moteursetjack_ints_tmr0();
        INTCONbits.TMR0IF = 0;
    }
    //moteursetjack_ints();
    if(INTCONbits.RBIF)   // Fin de course moteur 1
    {
	if(FIN_COURSE1_BAS) moteursetjack_ints_RB_bas();
	if(FIN_COURSE1_HAUT) moteursetjack_ints_RB_haut();
        INTCONbits.RBIF = 0;
    }
    if(INTCONbits.INT0IF) // Fin de course moteur 2-0 / Derriere
    {
	moteursetjack_ints_INT0();
        INTCONbits.INT0IF = 0;
    }
    if(INTCON3bits.INT1IF) // Fin de course moteur 2-1 / Median
    {
	moteursetjack_ints_INT1();
        INTCON3bits.INT1IF = 0;
    }
    if(INTCON3bits.INT2IF) // Fin de course moteur 2-2 / Devant
    {
	moteursetjack_ints_INT2();
        INTCON3bits.INT2IF = 0;
    }
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

