#ifndef ANTICOL
#define ANTICOL

#include "../common/lin_alg.h"

/*
Boucle principale
*/
void* ac_MainLoop(void*);

/*
CallBack pour les capteurs
*/
void ac_OnRecvCaptors(int n, float *values);


#endif
