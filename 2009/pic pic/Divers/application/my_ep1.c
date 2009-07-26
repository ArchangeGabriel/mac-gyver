// my_ep1.c /// j'y fais ce que je veux ! Na !
// OUT buffer (pc vers pic)


#include "my_ep1.h"
#include "my_ep2.h"

#include "servos.h"
#include "analogs.h"
#include "moteursetjack.h"

#include "../../../io/commandes.h"

volatile uchar __at(0x500) ep1_OutBuffer[MY_EP1_BUFFER_SIZE];

unsigned char checksum, i;
unsigned char tetedelecture;

#define ftoggle_A4() { PORTAbits.RA4 = !PORTAbits.RA4; }

void my_ep1_init(void)
{
    EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
    EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
    EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
}

void my_ep1_out(void)
{
    if(EP_OUT_BD(1).Cnt >= 1)
    {
	ep2_num_bytes_to_send = 1;
	ep2_source_data = mydata;
	mydata[0] =  RECU;
        tetedelecture = 1;
        
	    if(ep1_OutBuffer[0] & ANALOGS)
            {
                ep2_num_bytes_to_send += 2 * NB_ANALOGS;
	        ep2_source_data = an_data;
	        ep2_source_data[0] = ANALOGS | RECU;
	    }
	    if(ep1_OutBuffer[0] & DIGITALS)
	    {
                ep2_source_data[ep2_num_bytes_to_send++] = PORTB & 0xEF;
	        ep2_source_data[0] |= DIGITALS;
	    }
	    if(ep1_OutBuffer[0] & SERVO)
	    {
	        if(set_servo(0, ep1_OutBuffer[tetedelecture]))
	            ep2_source_data[0] |= SERVO;
		else 
	            ep2_source_data[0] |= ERREUR_SERVO;
	        tetedelecture ++;
	    }
	    if(ep1_OutBuffer[0] & MOTEURS)
	    {
	        ordre_moteur(ep1_OutBuffer[tetedelecture],ep1_OutBuffer[tetedelecture+1]);
		tetedelecture+=2;
		ep2_source_data[0] |= MOTEURS;
		ep2_source_data[ep2_num_bytes_to_send++] = etat_moteur[0];
		ep2_source_data[ep2_num_bytes_to_send++] = etat_moteur[1];
	    }
        

        my_prepare_ep2_in(); // Envoi

        EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
        if(EP_OUT_BD(1).Stat.DTS == 0)
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
        else
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
    }
    else // Raise an error
    {
        EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
        EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
        EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
    }
}


