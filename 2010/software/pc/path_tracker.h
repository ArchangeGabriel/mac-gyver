#ifndef PATH_TRACKERH
#define PATH_TRACKERH

#include <pthread.h>
#include "common.h"

//------------------------------------------------------------------------------
/*
Boucle principale, appelée par pc_main
*/
void* pt_MainLoop(void*);

//------------------------------------------------------------------------------
/*
Initialise le path planner avec la configuration du terrain
*/
void pt_init(int config_terrain);

//------------------------------------------------------------------------------
/*
Ordonne au robot d'aller à la position spécifiée.
Renvoie un mutex vérouillé qui sera dévérouillé à l'arrivée
Si le robot n'a pas finit le chemin courant, celui-ci est annulé
et remplacé par le nouveau.
*/
enum{tpDEST,tpAPPROACH,tpLEAVE, tpWAYPOINT};
pthread_mutex_t* pt_go_to(const position_t &pos, int type = tpDEST, bool append = false);

//------------------------------------------------------------------------------
/*
Ajoute une étape au chemin en cours (de la destination actuelle à pos)
*/
pthread_mutex_t* pt_add_step(const position_t &pos);

//------------------------------------------------------------------------------
/*
Renvoie la destination courante du robot
*/
position_t pt_get_dest();

//------------------------------------------------------------------------------
/*
Efface le chemin
*/
void pt_clear_path();

//------------------------------------------------------------------------------
// Stop le robot 
void pt_stop(int id);


#endif
