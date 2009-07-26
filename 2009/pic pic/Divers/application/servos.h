// servos.h //

#include <pic18fregs.h>
#include "common_types.h"

// jusqu'a 4 servos...

#ifndef _SERVOS_H
#define _SERVOS_H

#define NB_SERVO 1

// Commenter les lignes inutiles !!!!!

#define POS_DEFAULT0 64 // 1.6ms (1 ~ 0.2us)
//#define POS_DEFAULT1 8000 // 1.6ms (1 ~ 0.2us)
//#define POS_DEFAULT2 8000 // 1.6ms (1 ~ 0.2us)
//#define POS_DEFAULT3 8000 // 1.6ms (1 ~ 0.2us)

//////////////// PINS //////////////////
#define PIN_SVO0 PORTDbits.RD7
#ifdef POS_DEFAULT1
    #define PIN_SVO1 PORTDbits.RD6
#endif
#ifdef POS_DEFAULT2
    #define PIN_SVO2 PORTDbits.RD5
#endif
#ifdef POS_DEFAULT3
    #define PIN_SVO3 PORTDbits.RD4
#endif

//////////////// TRIS /////////////////
#define TRIS_SVO0 TRISDbits.TRISD7
#ifdef POS_DEFAULT1
    #define TRIS_SVO1 TRISDbits.TRISD6
#endif
#ifdef POS_DEFAULT2
    #define TRIS_SVO2 TRISDbits.TRISD5
#endif
#ifdef POS_DEFAULT3
    #define TRIS_SVO3 TRISDbits.TRISD4
#endif

void init_servos(void);
unsigned char set_servo(unsigned char servo_id, unsigned /*int*/ char position);
void gestionservos_tmr1(void);
void gestionservos_tmr2(void);

#endif // _SERVOS_H
