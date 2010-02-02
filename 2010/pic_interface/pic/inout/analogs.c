// analogs.c // gestion des entrées analogiques

#include <pic18fregs.h>
#include "analogs.h"
#include "ep2.h"
#include "proto.h"
#include "pining.h"

#define NB_DIG (16-NB_ANALOGS) // Pour les registres de conf
#define SIZE_AN_DATA ((2*NB_ANALOGS) + 2)

//unsigned char send_an[128];//2*NB_ANALOGS + 1];
unsigned char* an;
unsigned char compteur_an;
unsigned char an_data[SIZE_AN_DATA];
unsigned char number_an;

unsigned char configured = 0;

void send_an_data(unsigned char number)
{
    ep2_source_data = an_data;
    if(number>number_an)
    {
        ep2_num_bytes_to_send = 2 + (number_an<<1);
        ep2_source_data[1] = number_an;
    }
    else
    {
        ep2_num_bytes_to_send = 2 + (number<<1);
        ep2_source_data[1] = number;
    }
    ep2_source_data[0] = ANALOG;
    my_prepare_ep2_in();
}


void setup_adconversion(void) // Configure AD...
{
    set_antris();
    for(compteur_an=0;compteur_an<SIZE_AN_DATA;compteur_an++) an_data[compteur_an]=0;
    compteur_an = 0;
    if(configured)
    {
        OSCCONbits.IDLEN = 1; //|= 0x80; // 10000000 : IDLEN = 1 : pas de SLEEP (pour AD conversion)
        OSCCONbits.SCS = 0; // &= 0xfc; // 11111100 : SCS = 0 : primary clock source (on ne sait jamais !)
        ADCON1 = NB_DIG; // 00 0 0 xxxx : nU, Vref- = VSS, Vref+ = VDD, nb d'analogs
        ADCON0 = 0x00; // 00 0000 0 0 : nU, ANinput = AN0, idle, ADOFF
        ADCON2 = 0x2E; // 0 0 101 110 : left justified, nU, 12 Tac d'acquisition, freq de Fosc/64
        ADCON0 |= 0x01; // xx xxxx x 1 : ADON
        an = &(an_data[2]);
        PIE1bits.ADIE = 1;
    }    
    else
    {
        init_adconversion(number_an);
    }
// il faut attendre un init_adconversion pour lancer la convertion.
}

void init_adconversion(unsigned char number)
{
    number_an = number;
    ADCON0bits.GO = 1;  // Lance la convertion
    configured = 1;
}

void maj_result(void)
{
    unsigned char i;
    i = compteur_an << 1;
    an[i] = ADRESL;
    an[i + 1] = ADRESH;
    compteur_an++;
    compteur_an %= number_an;
    ADCON0 = (compteur_an<<2) + 1;
    ADCON0bits.GO = 1;
}
