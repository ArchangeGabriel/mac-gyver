// moteurs.c //

#include <pic18fregs.h>
#include "common_types.h"
#include "ep2.h"
#include "moteurs.h"
#include "pining.h"



void init_motors(void)
{
    TRISMOT11 = 0;
    TRISMOT12 = 0;
    TRISMOT21 = 0;
    TRISMOT22 = 0;
    TRISMOT31 = 0;
    TRISMOT32 = 0;
    TRISMOT41 = 0;
    TRISMOT42 = 0;
}

void set_motors(unsigned char values)
{
    PINMOT11 = values & 0x01;
    values>>=1;
    PINMOT12 = values & 0x01;
    values>>=1;
    PINMOT21 = values & 0x01;
    values>>=1;
    PINMOT22 = values & 0x01;
    values>>=1;
    PINMOT31 = values & 0x01;
    values>>=1;
    PINMOT32 = values & 0x01;
    values>>=1;
    PINMOT41 = values & 0x01;
    values>>=1;
    PINMOT42 = values & 0x01;
}

/*void init_DC_motor(unsigned char i)
{
    if(i==1) { TRISMOT11 = 0; TRISMOT12 = 0; }
    if(i==2) { TRISMOT21 = 0; TRISMOT22 = 0; }
    if(i==3) { TRISMOT31 = 0; TRISMOT32 = 0; }
    if(i==4) { TRISMOT41 = 0; TRISMOT42 = 0; }
}*/

/*void init_step_motor(void)
{
    T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // timer off au debut
//    INTCON |= 0x38;                    // INT0, TMR0, RB on...
//    INTCON &= 0xf8;                    // ...
//    INTCON2 = 0x85;                    // Falling edge for INT0,1,2. High priority for RB.
 //   INTCON3 = 0xD8;                    // INT1,2 on, High priority.
    TRISD &= 0xF0;                     // RD0..3 en sorties
    PORTD &= 0xF0;
    TRISC &= 0x3C;                     // RC0, 1, 6, 7 en sorties
    PORTC &= 0x3C;
}*/

/*void moteursetjack_ints_jack()
{
    mydata[0] = JACK;
    ep2_num_bytes_to_send = 4;
    ep2_source_data = mydata;
    my_prepare_ep2_in();
}*/

/*void moteursetjack_ints_tmr0(void)
{
    if(depart) depart++;
    else
    {
	sending(PRINTF, 7);
	if(mot1_activite)
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
		sending(PRINTF,mot1_position); 

	        if((mot1_position == mot1_pos_visee)&& mot1_pos_visee);
	        {
                    T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF

                    mot1_phase.b0 = PIN_MOT1_1;
       	            mot1_phase.b1 = PIN_MOT1_3;
	            PIN_MOT1_1 = 0;
	            PIN_MOT1_2 = 0;
	            PIN_MOT1_3 = 0;
	            PIN_MOT1_4 = 0;
	            mot1_activite=0;
		    sending(VPOS, mot1_position);
	        }
	        compteur_mot = 0;
	    }
        }
    }
}*/

