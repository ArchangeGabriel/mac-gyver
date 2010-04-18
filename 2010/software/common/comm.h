#ifndef COMMH
#define COMMH

#include "../common/simul.h"

// Protocole
typedef struct MSG_INT1_t
{
  int type;
  int msg_id;
  
  int value;
}MSG_INT1_t;

typedef struct MSG_INT2_t
{
  int type;
  int msg_id;
  
  int value1;
  int value2;  
}MSG_INT2_t;

typedef struct MSG_INTn_t
{
  int type;
  int msg_id;
  
  int n;
}MSG_INTn_t;

// Type de message
// (ne pas oublier de les ajouter à get_size_msg ci-dessous)
enum{EMPTY_MSG, // réservé pour dire qu'il n'y a pas de message
     QUERY,   // de type MSG_INT1_t, value := DIST
     MSG_MOTORS,  // de type MSG_INT2_t, value1 := pow_left, value2 := pow_right, entre RANGE_MOTOR et -RANGE_MOTOR (cf ci-dessus)
     JACK,    // de type MSG_INT1_t, jack retiré     
     DIST,    // de type MSG_INTn_t, values := valeur des capteurs de distance, entre 0 et RANGE_CAPTOR
     COUL,    // de type MSG_INTn_t, values := valeur des capteurs de couleur
     CODER,   // de type MSG_INT2_t, value1 := coder_left, value2 := coder_right   
     PRINTF,  // Pour débuggage du pic: affiche une valeur dans la console     
     ACK      // Acknoledge de message
};


/* Protocole:

  PIC RECOIT        ->         PIC ENVOIE
  -------------------------------------------
  QUERY DIST (INT1)   |     DIST      (INTn)
  MOTORS     (INT2)   |     rien
  quand JACK retiré   |     ACK JACK  (INT1)  
  après TIMER_CODER µs|     CODER     (INT2)  
  
msg_id: indique le numéro du message
Pour le PC, msg_id commence à 1 et augmente à chaque message envoyé à l'initiative du PC
Pour le PIC, msg_id commence à -1 et diminue à chaque message envoyé à l'initiative du PIC

Un message de type ACK ou en réponse à un QUERY doit avoir le même numéro que le message original
*/

int get_msg_type(char *msg);
int get_msg_header_size(char *msg);
int get_msg_data_size(char *msg);


#ifdef SIMULATION

#define MSG_INFO 255           // QUERY MSG_INFO
#define WEBCAM   254           // QUERY WEBCAM
#define RESTART  253           // ACK RESTART
#define WEBCAM_QUERY   252     // QUERY WEBCAM

typedef struct _picInfo_t
{
  int type;
  int msg_id;
  
  float posX,posY,posA;
  float destX,destY,destA;
} picInfo_t;

#endif

#endif
