#ifndef STRATEGIEH
#define STRATEGIEH

#include "types.h"

enum{clBLUE,clYELLOW};

//------------------------------------------------------------------------------
/*
Appelé au démarrage du programme, color est la couleur du robot (clRED ou clGREEN)
Lance la reconnaissance vidéo de la configuration
*/
void strat_init();
bool is_strat_ready();

//------------------------------------------------------------------------------
/*
Règle/Renvoie la couleur
*/
void strat_set_color(int _color);
int strat_get_color();

//------------------------------------------------------------------------------
/*
Renvoie la configuration du terrain (cf webcamAPI.h)
*/
void strat_set_config_terrain(int c);
int strat_get_config_terrain();

//------------------------------------------------------------------------------
/*
Appelé lorsque le Jack est retiré
*/
void strat_lets_go();

//------------------------------------------------------------------------------
/*
Prend une position pour le robot en haut à gauche et renvoit la position adaptée à la couleur
*/
position_t symetrize(position_t pos);

//------------------------------------------------------------------------------
/*
Attend qu'un mutex se libère et le détruit
*/
void wait_for_it(pthread_mutex_t *mutex);

//------------------------------------------------------------------------------
/*
Renvoie vrai ssi le jeu a commencé (= jack retiré)
*/
bool strat_is_started();

//------------------------------------------------------------------------------
/*
Renvoie le temps écoulé depuis le début (en secondes)
*/
float strat_elapsed_time();

//------------------------------------------------------------------------------
/*
Appelé à la fin de 1min30
*/
void strat_game_over();

//------------------------------------------------------------------------------
/*
Se recalle contre les bords
*/
void strat_recallage();

//------------------------------------------------------------------------------
/*
  Signal qu'un recallage est nécessaire
*/
void strat_signal_recall();

#endif
