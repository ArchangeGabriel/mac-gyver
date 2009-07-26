// moteursetjack.c //

#include <pic18fregs.h>
#include "common_types.h"
#include "my_ep2.h"
#include "../../../io/commandes.h"
#include "moteursetjack.h"

#define ftoggle_A4() { PORTAbits.RA4 = !PORTAbits.RA4; }

unsigned char depart=0;
unsigned char ticoutac = 0;
Char mot1_phase; // phase = dernier signal envoyé au moteur1
unsigned char mot1_sens=0;
unsigned char etat_moteur[2];
unsigned char mot1_position=0, mot2_position=0;
unsigned char mot1_pos_visee=0, mot2_pos_visee=0;
unsigned int compteur_mot=0;

void init_moteursetjack(void)
{
    T0CON = CONFIG_TMR0_MOTEUR & 0x7F;// TMR0 off...    //| 0x08; // PSA off
    etat_moteur[0] = INITIALISATION_1;
    etat_moteur[1] = INITIALISATION_2;
    INTCON |= 0x38;                    // INT0, TMR0, RB on...
    INTCON &= 0xf8;                    // ...
    INTCON2 = 0x85;                    // Rising edge for INT0,1,2. High priority for RB.
    INTCON3 = 0xD8;                    // INT1,2 on, High priority.
    TRISD &= 0xF0;                     // RD0..3 en sorties
    TRISC &= 0x3C;                     // RC0, 1, 6, 7 en sorties
    PORTD &= 0xF0;
    PORTC &= 0x3C;
}

void ordre_moteur(unsigned char ordre1, unsigned char ordre2)
{
    unsigned char dest;
    if(ordre1 & ACTIV_MOTEUR_1)
    {
	if(ordre1 & INITIALISATION_1) 
	{
	    dest = 0;
	    if(FIN_COURSE1_BAS)
		mot1_position = 0;
	    else
	        mot1_position = 255;
            etat_moteur[0] &= ~INITIALISATION_1;
	}
	else dest = (ordre1 & POSITION_MOTEUR_1);
        if((dest != mot1_position) || (etat_moteur[0] & ACTIV_MOTEUR_1))
        {
            if(etat_moteur[0] & ACTIV_MOTEUR_1)
            {
                if(((dest > mot1_position)&&(!mot1_sens))||((dest < mot1_position)&&(mot1_sens))) // ie si on change de sens
                {
                    compteur_mot = PAS_PAR_ETAGE - compteur_mot;
		    depart = -TEMPS_ATTENTE; // Attend TEMPS_ATTENTE interruptions du timer0 avant de repartir dans l'autre sens
                    if(mot1_sens) mot1_position++;
                    else mot1_position--;
                }
            }
            else
            {
		etat_moteur[0] |= ACTIV_MOTEUR_1;
                compteur_mot = 0;
                PIN_MOT1_1 = mot1_phase.b0;
                PIN_MOT1_2 = !mot1_phase.b0;
                PIN_MOT1_3 = mot1_phase.b1;
                PIN_MOT1_4 = !mot1_phase.b1;
            }
            T0CON = CONFIG_TMR0_MOTEUR;
            mot1_sens = (dest > mot1_position);
            if(PIN_MOT1_1 == PIN_MOT1_3) ticoutac = mot1_sens; // A modifier si mauvais sens
            else ticoutac = !mot1_sens;
        }
    }
    if(ordre2 & ACTIV_MOTEUR_2)
    {
	if(ordre2 & INITIALISATION_2)
	{
	    if(FIN_COURSE2_ARRIERE) mot2_position = 0; else mot2_position = 2;
	    dest = 0;
	}
	dest = (ordre2 & POSITION_MOTEUR_2);
	if(dest != mot2_position)
	{
            mot2_pos_visee = dest;
            if(dest > mot2_position)
	    {
		PIN_MOT2_1 = 1;
		PIN_MOT2_2 = 0;
	    }
	    else
	    {
		PIN_MOT2_1 = 0;
		PIN_MOT2_2 = 1;
	    }
	    etat_moteur[1] |= ACTIV_MOTEUR_2;
	}
    }
    if(ordre2 & ACTIV_MOTEUR_3)
    {
	ftoggle_A4();
        switch (ordre2 & MOTEUR_3)
        {
	    case 0x00: PIN_MOT3_1 = 0;
	    	   PIN_MOT3_2 = 0;
		   break;
	    case 0x10: PIN_MOT3_1 = 1;
		   PIN_MOT3_2 = 0;
		   break;
	    case 0x20: PIN_MOT3_1 = 0;
		   PIN_MOT3_2 = 1;
		   break;
        }
    }
}

void moteursetjack_ints_RB_bas(void)
{
	    //depart = -TEMPS_ATTENTE; // Attend TEMPS_ATTENTE interruptions du timer avant de partir dans l'autre sens
	    //ticoutac = !ticoutac;
	    //mot1_sens = 1;
            //compteur_mot = 0;
            mot1_pos_visee = 0;
	    mot1_position = 0;
	    etat_moteur[0] &= ~POSITION_MOTEUR_1; // Maj position
	    //etat_moteur |= PM1(0); // ..
  //////////////////////////////////////////////////////////////////////////// news 
   	    T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
            etat_moteur[0] &= ~ACTIV_MOTEUR_1; // Inactif

            mot1_phase.b0 = PIN_MOT1_1;
            mot1_phase.b1 = PIN_MOT1_3;
            PIN_MOT1_1 = 0;
            PIN_MOT1_2 = 0;
            PIN_MOT1_3 = 0;
            PIN_MOT1_4 = 0;
}
void moteursetjack_ints_RB_haut(void)
{
	    //depart = -TEMPS_ATTENTE; // Attend TEMPS_ATTENTE interruptions du timer avant de partir dans l'autre sens
            //ticoutac = !ticoutac;
	    //mot1_sens = 0;
            //compteur_mot = 0;
	    mot1_pos_visee = POSITION_MAX_1;
            mot1_position = POSITION_MAX_1;
	    etat_moteur[0] &= ~POSITION_MOTEUR_1; // Maj position
	    etat_moteur[0] |= POSITION_MAX_1;     // ..
  //////////////////////////////////////////////////////////////////////////// news 
   	    T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
            etat_moteur[0] &= ~ACTIV_MOTEUR_1; // Inactif

            mot1_phase.b0 = PIN_MOT1_1;
            mot1_phase.b1 = PIN_MOT1_3;
            PIN_MOT1_1 = 0;
            PIN_MOT1_2 = 0;
            PIN_MOT1_3 = 0;
            PIN_MOT1_4 = 0;
}
void moteursetjack_ints_INT0(void) // position arriere
{
        PIN_MOT2_1 = 0;
        PIN_MOT2_2 = 0;
        mot2_position = 0;
	etat_moteur[1] &= ~POSITION_MOTEUR_2; // Maj position
	//etat_moteur |= PM2(0);              // ..
	etat_moteur[1] &= ~ACTIV_MOTEUR_2;    // Inactif !
}
void moteursetjack_ints_INT1(void) // position mediane
{
	if(mot2_pos_visee == 1)
	{
            PIN_MOT2_1 = 0;
            PIN_MOT2_2 = 0;
	    etat_moteur[1] &= ~ACTIV_MOTEUR_2;// Inactif !
	}
        mot2_position = 1;
	etat_moteur[1] &= ~POSITION_MOTEUR_2; // Maj position
	etat_moteur[1] |= 0x01;               // ..
}
void moteursetjack_ints_INT2(void) // position avant
{
        PIN_MOT2_1 = 0;
        PIN_MOT2_2 = 0;
        mot2_position = 2;
	etat_moteur[1] &= ~POSITION_MOTEUR_2; // Maj position
	etat_moteur[1] |= 0x02;               // ..
	etat_moteur[1] &= ~ACTIV_MOTEUR_2;    // Inactif !
}

void moteursetjack_ints_tmr0(void)
{
    if((!depart)&&(etat_moteur[0] & ACTIV_MOTEUR_1))
    {
	if(ticoutac)
	{
            PIN_MOT1_1 = !PIN_MOT1_1;
            PIN_MOT1_2 = !PIN_MOT1_2;
	    ticoutac = 0;
	}
	else
	{
            PIN_MOT1_3 = !PIN_MOT1_3;
            PIN_MOT1_4 = !PIN_MOT1_4;
	    ticoutac = 1;
	}
	compteur_mot++;
	if(compteur_mot >= PAS_PAR_ETAGE)
	{
            if(mot1_sens) mot1_position++;
	    else mot1_position--;

	    etat_moteur[0] &= ~POSITION_MOTEUR_1;                // Maj position
	    etat_moteur[0] |= mot1_position & POSITION_MOTEUR_1; // ...

	    if(mot1_position == mot1_pos_visee)
	    {
                T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
                etat_moteur[0] &= ~ACTIV_MOTEUR_1; // Inactif

                mot1_phase.b0 = PIN_MOT1_1;
       	        mot1_phase.b1 = PIN_MOT1_3;
	        PIN_MOT1_1 = 0;
	        PIN_MOT1_2 = 0;
	        PIN_MOT1_3 = 0;
	        PIN_MOT1_4 = 0;
	    }
	    compteur_mot = 0;
	}
    }
    else
    {
	if(depart < 3)
	{
            if(PIN_JACK)
            {
            /*    if(depart == 1) depart = 2;
		else if(depart == 2) 
                {
                    T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
                    mydata[0] = GOGOGRINGO;
                    ep2_num_bytes_to_send = 1;
	            ep2_source_data = mydata;
	            my_prepare_ep2_in();
	            depart = 0;
	        }*/
            }
            else depart = 1;
        }
        else depart++; // Pour les tempos au changement de sens...
    }
}

