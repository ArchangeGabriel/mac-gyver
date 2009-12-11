#ifndef PICAPI
#define PICAPI

#include <pthread.h>
#include "../common/simul.h"


/*
Boucle infinie gérant les connections
*/
void picMainLoop();


/***************** PC --> PIC *****************/

// Lance le PIC
int picBegin();

/*
Envoie la puissance des moteurs pour le mouvement
Comprise entre -1 (recule) et 1 (avance)
Renvoie 0 en cas de succès, un nombre non nul sinon
*/
int picMotorsPower(float pwleft, float pwright);


/*
Demande la valeur des capteurs de distance indiqués par Flag (cf common/comm.h)
Renvoie 0 en cas de succès, un nombre non nul sinon 
*/
int picDistCaptors();


/***************** PIC --> PC *****************/
// Fonction de callback
// Indiquer NULL désactive le callback

/*
Appelé lorsque le jack est retiré
*/
void picOnRecvJack(void (*fun)(void));

/*
Réception de la valeur des codeuses
*/
void picOnRecvCoder(void (*fun)(int /*coder_left*/, int /*coder_right*/));

/*
Réception de la valeur des capteurs de distance
*/
void picOnRecvDistCaptors(void (*fun)(int /*n*/, float* /*dist*/));

/*
Fonction à appeler en cas de reconnection intempestive avec le pic
*/
void picOnReconnectPIC1(void (*fun)());
void picOnReconnectPIC2(void (*fun)());

//---------------------------------------------------------------------------//








/*
Utile seulement pour la simulation
*/
#ifdef SIMULATION

/*
Pour visualiser la position supposée et la destination
*/
int picSendInfo(float posX, float posY, float posA,
                float destX, float destY, float destA);

/*
Pour obtenir une image de la webcam
*/
pthread_mutex_t* picWebcam(int id, void *data);

/*
Fonction appelé en cas de "re" dans le simulateur
*/
void picRecvReset(void (*fun)(void));

#endif

#endif
