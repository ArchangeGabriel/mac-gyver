#ifndef PATH_PLANNINGH
#define PATH_PLANNINGH

#include <pthread.h>
#include "types.h"

//------------------------------------------------------------------------------
/*
Initialise le path planner avec la configuration du terrain
*/
void pp_init(int config_terrain);

//------------------------------------------------------------------------------
/*
Ordonne au robot d'aller à la position spécifiée.
Renvoie un mutex vérouillé qui sera dévérouillé à l'arrivée
Si le robot n'a pas finit le chemin courant, celui-ci est annulé
et remplacé par le nouveau.
*/
enum{tpDEST,tpAPPROACH,tpLEAVE, tpWAYPOINT};
pthread_mutex_t* pp_go_to(const position_t &pos, int type = tpDEST, bool append = false);

//------------------------------------------------------------------------------
/*
Ajoute une étape au chemin en cours (de la destination actuelle à pos)
*/
pthread_mutex_t* pp_add_step(const position_t &pos);

//------------------------------------------------------------------------------
/*
Renvoie la destination courante du robot
*/
position_t pp_get_dest();

//------------------------------------------------------------------------------
/*
Efface le chemin
*/
void pp_clear_path();

//------------------------------------------------------------------------------
// Stop le robot 
void pp_stop(int id);

//------------------------------------------------------------------------------
/*
Boucle principale, appelée par pc_main
*/
void ppMainLoop();


#endif
