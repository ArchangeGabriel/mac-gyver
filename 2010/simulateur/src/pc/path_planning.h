#ifndef PATH_PLANNINGH
#define PATH_PLANNINGH

#include <pthread.h>
#include "types.h"

#define RAYON_RECULE    0.05   // si la destination se trouve derrière à telle distance, on recule


//------------------------------------------------------------------------------
/*
Ordonne au robot d'aller à la position spécifiée.
Renvoie un mutex vérouillé qui sera dévérouillé à l'arrivée
Si le robot n'a pas finit le chemin courant, celui-ci est annulé
et remplacé par le nouveau.
*/
enum{tpDEST,tpAPPROACH,tpLEAVE, tpWAYPOINT};
pthread_mutex_t* pp_go_to(position_t pos, int type = tpDEST, bool append = false, bool do_append = true, bool do_leave = true, pthread_mutex_t **approach = NULL, pthread_mutex_t **leave = NULL);

//------------------------------------------------------------------------------

/*
Ajoute une étape au chemin en cours (de la destination actuelle à pos)
*/
pthread_mutex_t* pp_add_step(position_t pos);

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
// Reprend le chemin
void pp_no_danger();
// Demande de ralentir
void pp_slow_down();

void pp_lock_situ();
void pp_unlock_situ();

//------------------------------------------------------------------------------
/*
Boucle principale, appelée par pc_main
*/
void ppMainLoop();


#endif
