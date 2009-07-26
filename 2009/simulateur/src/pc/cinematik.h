#ifndef CINEMATIKH
#define CINEMATIKH

#include "types.h"

/*
Appelée pour initialisation
color est la couleur du robot (cl RED ou clGREEN)
*/
void cine_init(int color);

/*
Donne/règle la position
*/
position_t cine_get_position();
void cine_set_position(position_t &_pos);

/*
Donne la vitesse (du robot et de la roue 0 gauche, 1 droite)
*/
vector_t cine_get_speed();
float cine_get_speed_wheel(int wheel);

/*
Fonction de callback: appellé lorsque le pc recoit les valeurs des codeuses
*/
void cine_OnCoderRecv(int left, int right);

/*
Fonction anti-dérapage: ajuste la puissance des moteurs pour éviter les dérapages
et transmet au pic.

PAS TOUCHE A newA et newB
*/
void cine_motors(float left, float right);

#endif
