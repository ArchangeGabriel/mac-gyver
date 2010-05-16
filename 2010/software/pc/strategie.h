#ifndef STRATEGIEH
#define STRATEGIEH

#include "common.h"

//------------------------------------------------------------------------------
/*
Appelé au démarrage du programme, color est la couleur du robot (clRED ou clGREEN)
Lance la reconnaissance vidéo de la configuration
*/
void* strat_MainLoop(void*);
void strat_init();
bool strat_is_ready();

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
Renvoie vrai ssi le jeu a commencé (= jack retiré) (ou fini)
*/
bool strat_is_started();
bool strat_is_game_over();

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
