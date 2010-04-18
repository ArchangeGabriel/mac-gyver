#ifndef CINEMATIKH
#define CINEMATIKH

#include "common.h"

/*
Appelée pour initialisation
*/
void cine_init();

/*
Donne/règle la position du centre du robot
*/
void cine_set_position(position_t &_pos);
position_t cine_get_position();
position_t cine_get_wheel_center_position();

/*
Donne la vitesse (du robot et de la roue 0 gauche, 1 droite)
*/
vector_t cine_get_speed();
double cine_get_speed_wheel(int wheel);

/*
Fonction de callback: appellé lorsque le pc recoit les valeurs des codeuses
*/
void cine_OnCoderRecv(double time, int left, int right);

/*
Règle la vitesse de consigne des moteurs (paramètre left et right: vitesses de consigne en m/s)
-> Gère l'asservissement vitesse
-> Gère l'antidérapage: ajuste la puissance des moteurs pour éviter les dérapages
-> Transmet au pic.
*/
void cine_motors(double left, double right);

#endif
