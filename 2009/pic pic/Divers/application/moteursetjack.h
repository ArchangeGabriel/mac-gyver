// moteursetjack.h //

#ifndef _MOTEURSETJACK_H
#define _MOTEURSETJACK_H

#define CONFIG_TMR0_MOTEUR 0x86 // 1 0 0 0 0 110 : TMR0on, 16bits, CLKO, low to high, PSA on, 1:128 
// On peut faire toute les puissances negatives de 2 de 46875 Hz (qui equivaut a 0xBE)

#define TEMPS_ATTENTE 20 // Temps d'attente (en interruptions de TMR0) pour changer de sens

#define PIN_JACK PORTBbits.RB7

#define PIN_MOT1_1 PORTDbits.RD2
#define PIN_MOT1_2 PORTDbits.RD3
#define PIN_MOT1_3 PORTCbits.RC6
#define PIN_MOT1_4 PORTCbits.RC7

#define PIN_MOT2_1 PORTDbits.RD1
#define PIN_MOT2_2 PORTDbits.RD0

#define PIN_MOT3_1 PORTCbits.RC0
#define PIN_MOT3_2 PORTCbits.RC1

#define FIN_COURSE1_BAS PORTBbits.RB3 
#define FIN_COURSE1_HAUT PORTBbits.RB4

#define FIN_COURSE2_ARRIERE PORTBbits.RB0 
#define FIN_COURSE2_MILIEU PORTBbits.RB1 
#define FIN_COURSE2_AVANT PORTBbits.RB2 

#define PAS_PAR_ETAGE 50 // TODO

#define POSITION_MAX_1 63 // entre 1 et 14

void init_moteursetjack(void);
void moteursetjack_ints_tmr0(void);
//void moteursetjack_ints(void);

void moteursetjack_ints_RB_bas(void);
void moteursetjack_ints_RB_haut(void);
void moteursetjack_ints_INT0(void);
void moteursetjack_ints_INT1(void);
void moteursetjack_ints_INT2(void);

void ordre_moteur(unsigned char ordre1, unsigned char ordre2);

extern unsigned char etat_moteur[2];

#endif // _MOTEURSETJACK_H
