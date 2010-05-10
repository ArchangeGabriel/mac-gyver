// servos.h //

#include <pic18fregs.h>
#include "common_types.h"

// jusqu'a 4 servos...

#ifndef _SERVOS_H
#define _SERVOS_H

void init_servos(void);
unsigned char set_servo(unsigned char servo_id, unsigned /*int*/ char position);
//void gestionservos_tmr1(void);
//void gestionservos_tmr2(void);
void gestionservos(void);

#endif // _SERVOS_H
