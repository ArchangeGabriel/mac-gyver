#ifndef PICAPI
#define PICAPI

#define PIC_FREQ  (1./45.7763671875)   // Fréquence d'horloge du PIC
#define TIMER_CODER   20000            // Regarde les valeurs des codeuses toutes les TIMER_CODER µs (dans l'idéal un peu plus faible que PIC_FREQ)

#include "../pic/protocole/proto.h"

/*
Boucle infinie gérant les connections
*/
void pic_MainLoop();
bool pic_is_ready();


/***************** PC --> PIC *****************/
/*
Signale que le jack a été retiré
*/
void pic_Jack();

/*
Envoie la puissance des moteurs pour le mouvement
Comprise entre -1 (recule) et 1 (avance)
Renvoie 0 en cas de succès, un nombre non nul sinon
*/
int pic_MotorsPower(double pwleft, double pwright);

/*
Avance ou recule le poussoir (voir ../../pic/protocole/proto.h)
Renvoie 0 en cas de succès, un nombre non nul sinon
*/
int pic_move_pusher(int position);
int pic_move_door(int position);

/*
teste la position du poussoir
*/
bool pic_where_pusher(int position);

/*
réinitialise les PIC
*/
void pic_Reset();

/***************** PIC --> PC *****************/
// Fonction de callback
// Indiquer NULL désactive le callback

/*
Appelé lorsque le jack est retiré
*/
void pic_OnRecvJack(void (*fun)(void));

/*
Réception de la valeur des codeuses
*/
void pic_OnRecvCoder(void (*fun)(double /*time*/, int /*coder_left*/, int /*coder_right*/));

/*
Fonction appelée en cas de reset du robot
*/
void pic_RecvReset(void (*fun)(void));

#endif
