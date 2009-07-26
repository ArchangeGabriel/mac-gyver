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

// Renvoie la valeur du ième capteur
float captor_get_dist(int i);

// Renvoie de l'obstacle devant le ième capteur
vector_t captor_get_position(int i);

// Renvoie le status du ième capteur
// 0 - ok
// 1 - slower
// 2 - danger
// 3 - distrib
int captor_get_status(int i);


#endif
