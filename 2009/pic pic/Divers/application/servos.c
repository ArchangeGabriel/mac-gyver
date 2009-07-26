// servos.c //

#include "servos.h"

unsigned char positions[NB_SERVO];
unsigned char change_svocmd;
unsigned char event_count, next_event_svo;
unsigned char svo_event[NB_SERVO];

unsigned char svo_order;

void init_servos(void)
{
    PR2 = 0xFF; //periode overflow Timer2 = 256
    T2CON = 0x3F; // timer2 conf = 0 0111 1 11 : nU, postscaler 1:8, on, prescaler 1:16
    T1CON = 0xA1; // timer1 conf = 1 0 10 0 0 0 1 : 16bits, vers proc = off, prescaler 1:4, oscillator off, Extclk input nU, internal clock, on
    PIE1bits.TMR1IE = 1; //Interrupt Timer1
    PIE1bits.TMR2IE = 1; //Interrupt Timer2
    positions[0]=POS_DEFAULT0;                // Positions par defaut
    TRIS_SVO0 = 0;
    change_svocmd = 1; // Flag de changement de position a 1
    TMR2 = 0;
}

unsigned char set_servo(unsigned char servo_id, unsigned /*int*/ char position)
{
     if(servo_id<NB_SERVO)
     {
         positions[servo_id]=position; // changer position
         change_svocmd = 1; // mise du Flag a 1
         return 1;
     }
     return 0;
}

void gestionservos_tmr2(void) // 1 avant
{
     PIN_SVO0 = 0;

     event_count++;  // On passe au prochain evenement

     if(event_count<NB_SERVO)
     {
	 PR2 = svo_event[event_count];
	 TMR2 = 0;
     }
}

void gestionservos_tmr1(void) // 2 avant
{
     if(change_svocmd) // si Flag = 1 (ie si on a ordonne un changement de position)
     {
         svo_event[0] = positions[0];
         change_svocmd = 0; // Flag a 0
     }
     event_count=0;
     PIN_SVO0=1; // Mise a 1 des sorties
     
     PR2 = svo_event[0];
     TMR2 = 0;
}
