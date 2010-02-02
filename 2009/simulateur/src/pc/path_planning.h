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
Ordonne au robot d'aller à la position spécifiée.
Renvoie un mutex vérouillé qui sera dévérouillé à l'arrivée
Si le robot n'a pas finit le chemin courant, celui-ci est annulé
et remplacé par le nouveau.
*/
position_t pp_get_dest();

//------------------------------------------------------------------------------
/*
Efface le chemin
*/
void pp_clear_path();

//------------------------------------------------------------------------------
/*
Go_to spécial pour l'approche d'un distributeur
*/
enum {distribGrandCote,       // distributeur du grand coté
      distribPetitCoteRobot,  // distributeur sur le petit coté, vers la zone de départ
      distribPetitCoteDepot}; // distributeur sur le petit coté, vers la zone de dépôt
pthread_mutex_t* pp_go_to_distrib(int distrib, int type = tpDEST, bool append = false, bool do_append = true, bool do_leave = true, pthread_mutex_t **approach = NULL, pthread_mutex_t **leave = NULL);

//------------------------------------------------------------------------------
/*
Go_to spécial pour le dépot
*/
enum {zone0,                  // zone de hauteur 0, id de 0 à 7 pour chaque bande noire (bande 0 du côté vert)
      zone1,                  // zone de hauteur 1, id de 0 à 3 pour chaque bande noire (bande 0 du côté vert)
      zone2};                 // zone de hauteur 2, id inutilisé, position = endroit de dépot des palets
pthread_mutex_t* pp_go_to_drop(int zone, int id_bande, vector_t pos = vector_t(0.,0.));


//------------------------------------------------------------------------------
/*
Ralentit / Reprend la vitesse normale le chemin.
*/
void pp_stop(int id);
void pp_slow_down();
void pp_no_danger();

void pp_lock_situ();
void pp_unlock_situ();

//------------------------------------------------------------------------------
/*
Boucle principale, appelée par pc_main
*/
void ppMainLoop();


#endif
