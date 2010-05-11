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
enum{MSG_EMPTY,   // réservé pour dire qu'il n'y a pas de message
     MSG_QUERY,   // de type MSG_INT1_t, value  := MSG_DIGIT | MSG_ANALOG
     MSG_MOTORS,  // de type MSG_INT2_t, value1 := pow_left, value2 := pow_right, entre RANGE_MOTOR et -RANGE_MOTOR (cf ci-dessus)
     MSG_CODER,   // de type MSG_INT2_t, value1 := coder_left, value2 := coder_right
     MSG_DCMOTOR, // de type MSG_INT2_t, value1 := id moteur, value2 := consigne
     MSG_SERVOMOTOR, // de type MSG_INT2_t, value1 := id moteur, value2 := consigne     
     MSG_DIGIT,   // de type MSG_INT1_t, value  := flags on
     MSG_ANALOG,  // de type MSG_INTn_t, values := captors values
     MSG_PRINTF   // Pour débuggage du pic: affiche une valeur dans la console     
};

int get_msg_type(char *msg);
int get_msg_header_size(char *msg);
int get_msg_data_size(char *msg);


#ifdef SIMULATION

#define MSG_INFO 255               // QUERY MSG_INFO
#define MSG_POS_INFO 256           // QUERY MSG_INFO
#define MSG_WEBCAM   254           // QUERY WEBCAM
#define MSG_RESTART  253           // ACK RESTART
#define MSG_WEBCAM_QUERY   252     // QUERY WEBCAM

typedef struct _picInfo_t
{
  int type;
  int msg_id;
  
  float posX,posY,posA;
  float destX,destY,destA;
} picInfo_t;

#endif

#endif
