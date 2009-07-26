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

#include "codeuses.h"

unsigned char totol, totoh;

#define ftoggle_A4() { PORTAbits.RA4 = !PORTAbits.RA4; }

/******************************************************************/

// beware : this is not a C main function, but the application
// entry point called from the boot.

void init_pwm(void) // Configure CCP1, CCP2 et Timer2
{
    PR2 = 0xFF; //periode Timer2 = 256
    CCPR1L = 0x80; // Rapport cyclique 1 = 1/2 (pour les bits de poids faibles, voir CCP1CON bits 4 et 5)
    CCPR2L = 0x80; // Rapport cyclique 2 = 1/2
    TRISC &= 0xF9; // 11111001 : CCP1, CCP2 = sorties
    T2CON = 0x7C; // timer2 conf = 0 1111 1 00 : nU, postscaler 1:16, on, prescaler 1:1
    CCP1CON = 0x0C; // CCP1 conf = 00 00 1100 : nU, bits poids faibles, PWM mode
    CCP2CON = 0x0C; // CCP2 conf...
}

void application_main(void) 
{
    TRISA = 0xEF;
    PORTA = 0x10;
    
    // Reset the Timer0 value
    TMR0H = 0;
    TMR0L = 0;

    // Configure the Timer0
    T0CON = 0x86; // TMR0ON, 16bits, CLKO, PSA on, 1:256

    init_pwm();
    init_codeuses();
    
    // Interruptions
    
    PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!

    INTCONbits.TMR0IE = 1; // Interrupt Timer 0
    INTCONbits.PEIE = 1; // Interrupts peripheriques (usb, timer 1...)
    INTCONbits.GIE = 1; // Interrupts global

    totol=0x80;
    totoh=0x80;

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
    if(INTCONbits.TMR0IF)
    {
        ftoggle_A4();
        TMR0H=totoh;
        TMR0L=totol;
        INTCONbits.TMR0IF = 0;
    }
    ////////////////////////////////// Codeuses ////////////////////
    if(INTCON3bits.INT1IF)
    {
        if(PIN_SSCOD1) codeusecnt[1]++; else codeusecnt[1]--;
	INTCON3bits.INT1IF = 0;
    }
    if(INTCONbits.INT0IF)
    {
        if(PIN_SSCOD0) codeusecnt[0]++; else codeusecnt[0]--;
	INTCONbits.INT0IF = 0;
    }
}

#pragma code low_priority_isr 0x4000
void low_priority_isr(void) interrupt
{
}

