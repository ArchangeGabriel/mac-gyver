// servos.c //

#include <pic18fregs.h>
#include "pining.h"

#include "debugs.h"

//unsigned char positions[NB_SERVO];
unsigned int positions[NB_SERVO];
unsigned char change_svocmd;
unsigned char event_count, first_event;
//unsigned char svo_event[NB_SERVO];
unsigned char svo_eventL[NB_SERVO+1];
unsigned char svo_eventH[NB_SERVO+1];
unsigned char classements[NB_SERVO];


void init_servos(void)
{
    int i;
/*    PR2 = 0xFF; //periode overflow Timer2 = 256
    T2CON = 0x3F; //0x3F : timer2 conf = 0 0111 1 11 : nU, postscaler 1:8, on, prescaler 1:16
    T1CON = 0x91; //0xA1 : timer1 conf = 1 0 10 0 0 0 1 : 16bits, vers proc = off, prescaler 1:4, oscillator off, Extclk input nU, internal clock, on
    PIE1bits.TMR1IE = 1; //Interrupt Timer1
    PIE1bits.TMR2IE = 0; //Interrupt Timer2*/

    T1CON = 0xA1; // 21.8ms...
    TMR1H = 0;
    TMR1L = 0;
    PIE1bits.TMR1IE = 1; //Interrupt Timer1
    PIR1bits.TMR1IF = 0; //Interrupt Timer1

    for(i=0;i<NB_SERVO;i++)
    {
        positions[i]=0;                // Positions par defaut
        svo_eventH[i]=0;
        svo_eventL[i]=0;
        classements[i]=0;
    }
    svo_eventH[NB_SERVO]=0;
    svo_eventL[NB_SERVO]=0;
    first_event = 0;
    event_count = 0;
    set_servotris();
    clear_all_servo();
    change_svocmd = 1; // Flag de changement de position a 1
    //TMR2 = 0;
}

/*unsigned char set_servo(unsigned char servo_id, unsigned char position)
{
     if(servo_id<NB_SERVO)
     {
         positions[servo_id]=position; // changer position
         change_svocmd = 1; // mise du Flag a 1
         return 1;
     }
     return 0;
}*/

/*void sort_servo(void)
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
        svo_event[i] = positions[classements[i]] - positions[classements[i-1]];
    }
    i=0;
    while((i<NB_SERVO) && (svo_event[i]==0)) 
    {
        i++;
    }
    first_event = i;
    for(i=0;i<NB_SERVO;i++)
    {
        write_debug(i,classements[i]);
    }
    write_debug(4,first_event);
}*/

/*void gestionservos_tmr2(void)
{
//    clear_servo(1);
//    PIE1bits.TMR2IE = 0; //Interrupt Timer2

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

void gestionservos_tmr1(void)
{
//    set_usedservo(); // Mise a 1 des sorties de position non nulle
//    PR2 = positions[1];
//    PIR1bits.TMR2IF = 0;
//    PIE1bits.TMR2IE = 1;
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
        PIR1bits.TMR2IF = 0;
        PIE1bits.TMR2IE = 1;
    }
}*/

unsigned char set_servo(unsigned char servo_id, unsigned char position)
{
     if(servo_id<NB_SERVO)
     {
       if(position)
       {
         positions[servo_id]=1125+(25*(unsigned int)position); // changer position
       }
       else
       {
         positions[servo_id] = 0;
       }
         change_svocmd = 1; // mise du Flag a 1
         return 1;
     }
     return 0;
}

void sort_servo(void)
{
  unsigned char i, j, k;
  unsigned int temp;
  if(change_svocmd) // si il y a eu du changement
  {
    for(i=0;i<NB_SERVO;i++) // Trier les evenements
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
        classements[k] = i;
    }
    temp = -positions[classements[0]];
    svo_eventL[0] = (unsigned char)temp;
    temp >>= 8;
    svo_eventH[0] = (unsigned char)temp;
    for(i=1;i<NB_SERVO;i++)
    {
        temp = positions[classements[i-1]] - positions[classements[i]];
        svo_eventL[i] = (unsigned char)temp;
        temp >>= 8;
        svo_eventH[i] = (unsigned char)temp;
    }
    temp = positions[classements[NB_SERVO-1]];
    svo_eventL[NB_SERVO] = (unsigned char)temp;
    temp >>= 8;
    svo_eventH[NB_SERVO] = (unsigned char)temp;

    i=0;   // Trouver le premier evenement non nul
    while((i<NB_SERVO) && ((svo_eventH[i]==0)&&(svo_eventL[i]==0))) 
    {
        i++;
    }
    first_event = i;

   /* for(i=0;i<NB_SERVO;i++)
    {
        write_debug(2*i,svo_eventL[i+1]);
        write_debug(2*i+1,svo_eventH[i+1]);
    }*/
  }
  change_svocmd = 0; // flag a 0
}

void gestionservos(void)
{
  if(event_count == NB_SERVO)
  {
    sort_servo();
    event_count = first_event;
    set_usedservo();
  }
  else
  {
    clear_servo(classements[event_count]);
    event_count++;
    while((event_count < NB_SERVO)&&((svo_eventH[event_count]==0)&&(svo_eventL[event_count]==0)))
    {
      event_count++;
    }
  }
  TMR1H = svo_eventH[event_count];
  TMR1L = svo_eventL[event_count];
}
