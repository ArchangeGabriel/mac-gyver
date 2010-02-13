// servos.c //

#include <pic18fregs.h>
#include "pining.h"

unsigned char positions[NB_SERVO];
unsigned char change_svocmd;
unsigned char event_count, first_event;
unsigned char svo_event[NB_SERVO];
unsigned char classements[NB_SERVO];


void init_servos(void)
{
    int i;
    PR2 = 0xFF; //periode overflow Timer2 = 256
    T2CON = 0x3F; // timer2 conf = 0 0111 1 11 : nU, postscaler 1:8, on, prescaler 1:16
    T1CON = 0xA1; // timer1 conf = 1 0 10 0 0 0 1 : 16bits, vers proc = off, prescaler 1:4, oscillator off, Extclk input nU, internal clock, on
    PIE1bits.TMR1IE = 1; //Interrupt Timer1
    PIE1bits.TMR2IE = 1; //Interrupt Timer2
    for(i=0;i<NB_SERVO;i++) positions[i]=0;                // Positions par defaut
    set_servotris();
    clear_all_servo();
    change_svocmd = 1; // Flag de changement de position a 1
//    TMR2 = 0;
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

void sort_servo(void)
{
    unsigned char i, j, k;
    for(i=0;i<NB_SERVO;i++)
    {
        k=0;
        for(j=0;j<i;j++)
        {
            if(positions[i]>=positions[j]) k++;
        }
        for(j=i+1;j<NB_SERVO;j++)
        {
            if(positions[i]>positions[j]) k++;
        }
        //svo_event[k] = positions[i];
        classements[k] = i;
    }
    svo_event[0] = positions[classements[0]];
    for(i=1;i<NB_SERVO;i++)
    {
        svo_event[i] = positions[classements[i-1]] - positions[classements[i]];
    }
    i=0;
    while((i<NB_SERVO) && (svo_event[i]==0)) 
    {
        i++;
    }
    first_event = i;
}

void gestionservos_tmr2(void) // 1 avant
{
    clear_servo(classements[event_count]);

    event_count++;  // On passe au prochain evenement

    while((event_count<NB_SERVO) && (svo_event[event_count]==0)) 
    {
        clear_servo(classements[event_count]);
        event_count++;
    }
    if(event_count<NB_SERVO)
    {
	 PR2 = svo_event[event_count];
	// TMR2 = 0;
    }
    else
    {
        PIE1bits.TMR2IE = 0; //Interrupt Timer2
    } 
}

void gestionservos_tmr1(void) // 2 avant
{
    if(change_svocmd) // si Flag = 1 (ie si on a ordonne un changement de position)
    {
        sort_servo();
        change_svocmd = 0; // Flag a 0
    }
    event_count=first_event;
    set_usedservo(); // Mise a 1 des sorties de position non nulle
    //PIN_SVO0=1; // Mise a 1 des sorties
    if(event_count<NB_SERVO)
    {
        PR2 = svo_event[event_count];
        TMR2 = 0;
        PIE1bits.TMR2IE = 1;
    }
}

