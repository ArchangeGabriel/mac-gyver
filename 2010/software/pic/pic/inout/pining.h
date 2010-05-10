// pining.h //



#ifndef PINING_H_
#define PINING_H_

#define PINALIM PORTDbits.RD1
#define TRISALIM TRISDbits.TRISD1

///////////// MOTORS ///////////////////////////

#define PINMOT11 PORTDbits.RD2
#define PINMOT12 PORTDbits.RD3
#define PINMOT21 PORTCbits.RC6
#define PINMOT22 PORTCbits.RC7
#define PINMOT31 PORTDbits.RD7
#define PINMOT32 PORTDbits.RD6
#define PINMOT41 PORTDbits.RD4
#define PINMOT42 PORTDbits.RD5

#define TRISMOT11 TRISDbits.TRISD2
#define TRISMOT12 TRISDbits.TRISD3
#define TRISMOT21 TRISCbits.TRISC6
#define TRISMOT22 TRISCbits.TRISC7
#define TRISMOT31 TRISDbits.TRISD7
#define TRISMOT32 TRISDbits.TRISD6
#define TRISMOT41 TRISDbits.TRISD4
#define TRISMOT42 TRISDbits.TRISD5

/////////////// SERVOS /////////////////////////

#define NB_SERVO 4

#define PINSVO1 PORTCbits.RC1
#define PINSVO2 PORTCbits.RC2
#define PINSVO3 PORTCbits.RC0
#define PINSVO4 PORTDbits.RD0

#define TRISSVO1 TRISCbits.TRISC1
#define TRISSVO2 TRISCbits.TRISC2
#define TRISSVO3 TRISCbits.TRISC0
#define TRISSVO4 TRISDbits.TRISD0

#include "servos_low_functions.h" // fichier a modifier si on modifie le nombre de servos

/////////////// ANALOGS ////////////////////////

//#define NB_ANALOGS 8 // AN0...7

#define set_antris() {\
TRISAbits.TRISA0 = 1;\
TRISAbits.TRISA1 = 1;\
TRISAbits.TRISA2 = 1;\
TRISAbits.TRISA3 = 1;\
/*TRISAbits.TRISA5 = 1;\
TRISEbits.TRISE0 = 1;\
TRISEbits.TRISE1 = 1;\
TRISEbits.TRISE2 = 1;*/\
}

////////////////////////////////////////////////

#endif // PINING_H_
