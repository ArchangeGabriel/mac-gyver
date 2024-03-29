#ifndef COMMH
#define COMMH

#include "../common/simul.h"

//Pince
enum {stClampOpen, stClampClosed, stClampLintel};
enum {posClampBack, posClampMiddle, posClampFront};
enum {beltStop, beltInside, beltOutside};

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

#define RANGE_MOTOR          127   // Entrée des moteurs entre -RANGE_MOTOR et RANGE_MOTOR
#define RANGE_DIST_CAPT      400   // Sortie des capteurs de distance entre 0 et RANGE_DIST_CAPT
#define VALUE_FOR_0cm_DIST   620.
#define VALUE_FOR_15cm_DIST  182.
#define COEFF_A_DIST         0.48320478134
#define COEFF_B_DIST         0.994212066
#define MAX_DIST_CAPT        0.32  // Distance max des capteurs de distance
#define MIN_DIST_CAPT        0.004 // Distance min des capteurs de distance
#define TIMER_CODER          5.3   // Envoie des valeurs des codeuses toutes les TIMER_CODER ms

// Type de message
// (ne pas oublier de les ajouter à get_size_msg ci-dessous)
enum{EMPTY_MSG, // réservé pour dire qu'il n'y a pas de message
     QUERY,   // de type MSG_INT1_t, value := DIST
     BEGIN,   // DOIT ETRE EN 2
     MOTORS,  // de type MSG_INT2_t, value1 := pow_left, value2 := pow_right, entre RANGE_MOTOR et -RANGE_MOTOR (cf ci-dessus)
     JACK,    // de type MSG_INT1_t, jack retiré     
     BELT,    // de type MSG_INT1_t, value := pow, entre RANGE_BELT et -RANGE_BELT (cf ci-dessus)
     VPOS,    // de type MSG_INT1_t, value := hauteur en mm entre le sol et le bas du palet
     HPOS,    // de type MSG_INT1_t, value := posClampBack pour arrière, posClampMiddle pour milieu et posClampFront pour devant
     CLAMP,   // de type MSG_INT1_t, value := stClampClosed pour fermée, stClampOpen pour ouverte
     DIST,    // de type MSG_INTn_t, values := valeur des capteurs de distance, entre 0 et RANGE_CAPTOR
     COUL,    // de type MSG_INTn_t, values := valeur des capteurs de couleur
     CODER,   // de type MSG_INT2_t, value1 := coder_left, value2 := coder_right   
     PRINTF,
     ACK      // de type MSG_INT1_t, value := VPOS | HPOS       
};


/* Protocole:

  PIC RENCOIT        ->         PIC ENVOIE
  -------------------------------------------
  QUERY DIST (INT1)   |     DIST      (INTn)
  QUERY COUL (INT1)   |     COUL      (INTn)  //on n'a pas de capteur de couleurs...
  MOTORS     (INT2)   |     rien
  BELT       (INT1)   |     rien
  VPOS       (INT1)   |     ACK VPOS  (INT1)  (quand prêt)
  HPOS       (INT1)   |     ACK HPOS  (INT1)  (quand prêt) 
  CLAMP      (INT1)   |     ACK CLAMP (INT1)  (quand prêt) 
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

#define MSG_INFO 255     // QUERY MSG_INFO
#define WEBCAM   254     // QUERY WEBCAM
#define RESTART  253     // ACK RESTART
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
