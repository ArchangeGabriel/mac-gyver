#ifndef PICAPI
#define PICAPI

#define TIMER_CODER   5   // Envoie des valeurs des codeuses toutes les TIMER_CODER ms

/*
Boucle infinie gérant les connections
*/
void pic_MainLoop();


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
