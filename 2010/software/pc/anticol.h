#ifndef ANTICOL
#define ANTICOL

#include "../common/lin_alg.h"

/*
Boucle principale
*/
void anticolMainLoop();

/*
CallBack pour les capteurs
*/
void anticolOnRecvCaptors(int n, float *values);


#endif
