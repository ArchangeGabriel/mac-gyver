// analogs.c // gestion des entrées analogiques

#include <pic18fregs.h>
#include "analogs.h"
#include "ep2.h"
#include "proto.h"
#include "pining.h"
#include "debugs.h"

#include "app_usb.h"

#define NB_DIG (16-NB_ANALOGS) // Pour les registres de conf
#define SIZE_AN (2*NB_ANALOGS)

unsigned char compteur_an;
unsigned char an[SIZE_AN];
unsigned char an_data[SIZE_AN + 2];
unsigned char number_an;

unsigned char configured = 0;

void send_an_data(unsigned char number)
{
    unsigned char n,i,m;
    while(EP_IN_BD(2).Stat.uc & BDS_USIE); // wait the Uown bit to be cleared
    n = ((number > number_an) ? number_an : number);
    m = n<<1;
    ep2_num_bytes_to_send = m+2;
    ep2_source_data = &an_data[2];
    for(i=0;i<m;i++) ep2_source_data[i] = an[i];
    ep2_source_data = an_data;
    ep2_source_data[1] = n;
    ep2_source_data[0] = ANALOG;
    write_debug(0,ep2_source_data[1]);
    write_debug(1,ep2_source_data[2]);
    write_debug(2,ep2_source_data[3]);
    write_debug(3,ep2_source_data[4]);
    write_debug(4,ep2_source_data[5]);
    
    my_prepare_ep2_in();
}


void setup_adconversion(void) // Configure AD...
{
    set_antris();
    for(compteur_an=0;compteur_an<SIZE_AN;compteur_an++)
    {
        an_data[compteur_an]=0;
        an[compteur_an]=0;
    }
    an_data[SIZE_AN]=0;
    an_data[SIZE_AN + 1]=0;
    compteur_an = 0;
//    if(!configured)
//    {
        OSCCONbits.IDLEN = 1; //|= 0x80; // 10000000 : IDLEN = 1 : pas de SLEEP (pour AD conversion)
        OSCCONbits.SCS = 0; // &= 0xfc; // 11111100 : SCS = 0 : primary clock source (on ne sait jamais !)
        ADCON1 = NB_DIG; // 00 0 0 xxxx : nU, Vref- = VSS, Vref+ = VDD, nb d'analogs
        ADCON0 = 0x00; // 00 0000 0 0 : nU, ANinput = AN0, idle, ADOFF
        ADCON2 = 0x2E; // 0 0 101 110 : left justified, nU, 12 Tac d'acquisition, freq de Fosc/64
        ADCON0 |= 0x01; // xx xxxx x 1 : ADON
/*    }    
    else
    {
        init_adconversion(number_an);
    }*/
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
// il faut attendre un init_adconversion pour lancer la convertion.
}

void init_adconversion(unsigned char number)
{
  if(number){
    number_an = number;
    ADCON0bits.GO = 1;  // Lance la convertion
  //  configured = 1;
    write_debug(0,17);
  }
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
