#ifndef STRATEGIEH
#define STRATEGIEH

enum{clRED,clGREEN};

//------------------------------------------------------------------------------
/*
Appelé au démarrage du programme, color est la couleur du robot (clRED ou clGREEN)
Lance la reconnaissance vidéo de la configuration
*/
void strat_init();

//------------------------------------------------------------------------------
/*
Règle/Renvoie la couleur
*/
void strat_set_color(int _color);
int strat_get_color();

//------------------------------------------------------------------------------
/*
Appelé lorsque le Jack est retiré
*/
void strat_lets_go();

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
Renvoie/regle les config, -1 sur inconnues
*/
int strat_config_distrib();
int strat_config_palets();
void strat_set_config_distrib(int c);
void strat_set_config_palets(int c);

//------------------------------------------------------------------------------
/*
Appelé à la fin de 1min30
*/
void strat_game_over();

//------------------------------------------------------------------------------
/*
Boucle principale de la strategie
*/
void stratMainLoop();

//------------------------------------------------------------------------------
/*
Collecte les palets
Renvoie -1 si le boulot n'est pas fini, 0 sinon
*/
int strat_collecte();

//------------------------------------------------------------------------------
/*
Dépose les palets
Renvoie -1 si le boulot n'est pas fini, 0 sinon
*/
int strat_depose();

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
