// analogs.c // gestion des entrées analogiques

#include <pic18fregs.h>
#include "analogs.h"
#include "my_ep2.h"

//unsigned char send_an[128];//2*NB_ANALOGS + 1];
unsigned char* an;
unsigned char compteur_an;

void init_adconversion(void) // Configure AD...
{
    OSCCON |= 0x80; // 10000000 : IDLEN = 1 : pas de SLEEP (pour AD conversion)
    OSCCON &= 0xfc; // 11111100 : SCS = 0 : primary clock source (on ne sait jamais !)
    TRISA |= TRISA_CONF; // bons ports en input
    //TRISE |= TRISE_CONF; // idem
    ADCON1 = NB_DIG; // 00 0 0 xxxx : nU, Vref- = VSS, Vref+ = VDD, nb d'analogs
    ADCON0 = 0x00; // 00 0000 0 0 : nU, ANinput = AN0, idle, ADOFF
    ADCON2 = 0x2E; // 0 0 101 110 : left justified, nU, 12 Tac d'acquisition, freq de Fosc/64
    ADCON0 |= 0x01; // xx xxxx x 1 : ADON
    compteur_an = 0;
    PIE1bits.ADIE = 1;
    an = &(an_data[1]);
    ADCON0bits.GO = 1; //commence la conversion
}

void maj_result(void)
{
    an[compteur_an] = ADRESH;
    an[compteur_an + 1] = ADRESL;
    compteur_an += 2;
    compteur_an %= 2 * NB_ANALOGS;
    ADCON0 = (compteur_an << 1) + 1;
    ADCON0bits.GO = 1;
}
