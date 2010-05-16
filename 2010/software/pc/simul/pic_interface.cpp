#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <map>

#define PC_INCLUDE
#include "../../common/comm.h"
#include "../common.h"
#include "../usb.h"
#include "../picAPI.h"
#include "../cinematik.h"
#include "../path_tracker.h"

using namespace std;

// Compteur de messages
int msg_id;

// La webcam est gérée par le serveur dans le cas de la simulation
pthread_mutex_t mut_webcam;
uint16_t *webcam_data;
unsigned int *webcam_W;
unsigned int *webcam_H;
#include "../common.h"
// Gère les moteurs
void* pic1_handle_msg(void *);
pthread_t picI_thread_pic1;  

// Gère les reste
void* pic2_handle_msg(void *);
pthread_t picI_thread_pic2;

// Pour visualiser la position supposée et la destination sur le simulateur
int SendInfo(int type = MSG_INFO, int config = -1);
                
// Mutex/valeur des messages
map<int, pthread_mutex_t *> picI_digit_mutex;
map<int, int> picI_digit_value;
map<int, pthread_mutex_t *> picI_analog_mutex;
map<int, int> picI_analog_value;

// Valeur des codeuses
int last_coder_L = 0;
int last_coder_R = 0;
int coder_L = 0;
int coder_R = 0;    
int coder_id = 0;         

bool pic1OK;
bool pic2OK;

//------------------------------------------------------------------------------

// Toutes ces fonctions retournent -1 en cas d'erreur, et un resultat >= 0 si tout s'est bien passe.
// get_codeuses, get_digital_in, init_analog_in, get_analog_in peuvent retourner des valeurs interessantes (genre leur resultat quoi !).

//------------------------------------------------------------------------------
// change la vitesse des moteurs en ces valeurs (0 ne change rien, 1 arriere toute, 128 arret, 255 avant toute). Retourne 1 si ok, -1 sinon
int set_speed(unsigned char vitesse1, unsigned char vitesse2)
{ 
  MSG_INT2_t msg;
  msg.type   = MSG_MOTORS;
  msg.msg_id = msg_id++;
  msg.value1 = vitesse1;
  msg.value2 = vitesse2;  
  
  return (write_usb(PIC1, &msg, sizeof(msg)) < 0)?-1:1;
}
//------------------------------------------------------------------------------
// met les valeurs courantes des codeuses dans codeuse1 et codeuse2, et les sens dans sens1 et sens2 / Retourne l'indice de l'echange usb (il s'incremente tous les pas de temps, au cas ou on a rate une trame usb) si ok, -1 sinon
// cette fonction est destinee a etre appelee en boucle dans un thread (cf testusb.cpp)
int get_codeuses(int *codeuse1, unsigned char *sens1, int *codeuse2, unsigned char *sens2)
{
  *codeuse1 = coder_L;
  *codeuse2 = coder_R;
  *sens1 = coder_L > last_coder_L;
  *sens2 = coder_R > last_coder_R;  
  return coder_id;
}
//------------------------------------------------------------------------------
// retourne de 0 à 127 (ie les 7 entrees numeriques) si ok, -1 sinon
int get_digital_in(void)
{
  /*int id = msg_id++;
  
  MSG_INT1_t msg;
  msg.type   = MSG_QUERY;
  msg.msg_id = id;
  msg.value  = MSG_DIGIT;  
  
  picI_digit_mutex[id] = new pthread_mutex_t;
  pthread_mutex_init(picI_digit_mutex[id],NULL);
  pthread_mutex_lock(picI_digit_mutex[id]);
  
  if(write_usb(PIC2, &msg, sizeof(msg)) < 0)
    return -1;
  else
  {
    wait_for_it(picI_digit_mutex[id]);
    int res = picI_digit_value[id];
    
    map<int, pthread_mutex_t*> :: iterator it1 = picI_digit_mutex.find(id);
    picI_digit_mutex.erase(it1);  
    map<int, int> :: iterator it2 = picI_digit_value.find(id);
    picI_digit_value.erase(it2);
    
    return res;
  }*/
  return 255;
}
//------------------------------------------------------------------------------
// defini le nombre NB_ANALOG d'entrees analogiques. Retourne min(number, NB max d'analogiques de la carte) si ok, -1 sinon. Doit etre appelee avant le premier get_analog_in.
int init_analog_in(unsigned char number)
{
  return number;
}
//------------------------------------------------------------------------------
// rempli result des length (ou NB_ANALOG si length trop grand) premieres entrees analogiques. Retourne ne nombre de valeurs effectivement ecrites (ie min(length, NB_ANALOG)) si ok, -1 sinon
int get_analog_in(unsigned short *result, unsigned char length)
{
  /*int id = msg_id++;
  
  MSG_INT1_t msg;
  msg.type   = MSG_QUERY;
  msg.msg_id = id;
  msg.value  = MSG_ANALOG;  
  
  picI_analog_mutex[id] = new pthread_mutex_t;
  pthread_mutex_init(picI_analog_mutex[id],NULL);
  pthread_mutex_lock(picI_analog_mutex[id]);
  
  if(write_usb(PIC2, &msg, sizeof(msg)) < 0)
    usleep(10000);
    
  wait_for_it(picI_analog_mutex[id]);
  *result = picI_analog_value[id];
  
  map<int, pthread_mutex_t*> :: iterator it1 = picI_analog_mutex.find(id);
  picI_analog_mutex.erase(it1);  
  map<int, int> :: iterator it2 = picI_analog_value.find(id);
  picI_analog_value.erase(it2);
  */
  
  for(int i=0; i<length; i++)
    result[i] = 0;
  
  return length;
}
//------------------------------------------------------------------------------
// met le moteur number (1-4) dans le sens sens (-1, 0 arret, 1). Retourne 1 si ok, -1 sinon.
int set_DC_motor(unsigned char number, char sens)
{
  MSG_INT2_t msg;
  msg.type   = MSG_DCMOTOR;
  msg.msg_id = msg_id++;
  msg.value1 = number;
  msg.value2 = sens;  
  
  return (write_usb(PIC2, &msg, sizeof(msg)) < 0)?-1:1;
}
//------------------------------------------------------------------------------
//met le servo number (1-4) en position position (1-255) (position = 0 rend le servo libre). Retourne 1 si ok, -1 sinon
int set_servo(unsigned char number, unsigned char position)
{
  MSG_INT2_t msg;
  msg.type   = MSG_SERVOMOTOR;
  msg.msg_id = msg_id++;
  msg.value1 = number;
  msg.value2 = position;  
  
  return (write_usb(PIC2, &msg, sizeof(msg)) < 0)?-1:1;
}
//------------------------------------------------------------------------------
// Initialise les connexions usb avec la carte. Retourne 1 si tout fonctionne, un resultat negatif sinon.
int setup_usb_connexions()
{
  msg_id = 1;
  pthread_mutex_init(&mut_webcam,NULL);
  pic1OK = false;
  pic2OK = false;  
  pthread_create(&picI_thread_pic1, NULL, &pic1_handle_msg, NULL);  
  pthread_create(&picI_thread_pic2, NULL, &pic2_handle_msg, NULL);    
  
  while(!pic1OK || !pic2OK)
    usleep(10000);
    
  return 1;
}
//------------------------------------------------------------------------------
// Ferme proprement les connexions usb.
void shut_usb()
{
  close_connection(PIC1);
  close_connection(PIC2);  
}
//------------------------------------------------------------------------------
// Tente de reparer les connexions usb cassees. Retourne 1 si tout fonctionne, un resultat negatif sinon.
int repare_usb()
{
  shut_usb();
  return setup_usb_connexions();
}
//-------------------------------------------------------------------------------
void* pic1_handle_msg(void *)
{
  if(!WANT_PIC1)
    while(true) usleep(100000);

  char *_msg;
  
  while(true)
  {
    if(connect_usb(PIC1)<0)
    {
      fprintf(stderr,"Connection failed PIC1, retrying...\n");
      exit(1);
    }     
    
    fprintf(stderr,"Listen to PIC1...\n");  fflush(stdout);  
    
    pic1OK = true;
    
    while((_msg=read_usb(PIC1)))
    {
      if(_msg==NULL)
        break;
        
      switch(get_msg_type(_msg))
      {
        case MSG_EMPTY:
        fprintf(stderr,"<picInterface.cpp> PIC1: Connexion réinitialisée.\n");
        fflush(stdout); 
        pic_Reset();
        return NULL;
        break;      
        case MSG_CODER:
        {   
          SendInfo();
          MSG_INT2_t *msg = (MSG_INT2_t *)_msg;          
          last_coder_L = coder_L;
          last_coder_R = coder_R;        
          coder_L = msg->value1;
          coder_R = msg->value2;
          coder_id = msg->msg_id;
        }
        break;
        case MSG_PRINTF:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;          
          printf("from pic %d: %d\n",1,msg->value);
        }
        break;        
        default:
        fprintf(stderr,"<picInterface.cpp> PIC1: Unknown message type %d.\n",get_msg_type(_msg));
        fflush(stdout);            
        break;      
      }   
      delete _msg;    
    }
    fprintf(stderr,"Connection closed PIC1...\n");
    fflush(stdout);
    usleep(10000);
    break;
  }
  return NULL;
}
//-------------------------------------------------------------------------------
void* pic2_handle_msg(void *)
{  
  if(!WANT_PIC2)
    while(true) usleep(100000);
  
  char *_msg; 

  while(true)
  {
    if(connect_usb(PIC2)<0)
    {
      fprintf(stderr,"Connection failed PIC2, retrying...\n");
      exit(1);
    }

    fprintf(stderr,"Listen to PIC2...\n");  fflush(stdout); 

    pic2OK = true;
                
    while((_msg=read_usb(PIC2)))
    {
      if(_msg==NULL)
        break;
      
      //printf("Recv PIC2: %d\n",get_msg_type(_msg));     
      switch(get_msg_type(_msg))
      {
        case MSG_EMPTY:       
        fprintf(stderr,"<picInterface.cpp> PIC2: Connexion réinitialisée.\n");
        fflush(stdout); 
        return NULL;        
        break;
        case MSG_WEBCAM:
        {
          char *f = (char*)(_msg+sizeof(MSG_INT1_t));
          FILE *file = fopen(f, "rb");
          
          fread(webcam_W, sizeof(int), 1, file);
          fread(webcam_H, sizeof(int), 1, file);
          if(webcam_data)
          {
            int size = 3*(*webcam_W)*(*webcam_H);
            uint16_t *tmp = new uint16_t[size];
            fread(tmp, sizeof(uint16_t)*size, 1, file);  
            // Convertit de RGB à YUV
            for(int i=0; i<size; i+=3)
            {
              const double R = tmp[i+0]&255;  // les webcams sont la tête en bas sur le robot
              const double G = tmp[i+1]&255;
              const double B = tmp[i+2]&255;  
              webcam_data[(size-i)+0] =  0.299  * R + 0.587  * G + 0.114 * B;          // Y 
              webcam_data[(size-i)+1] = -0.1687 * R - 0.3313 * G + 0.5   * B + 128.;   // Cb
              webcam_data[(size-i)+2] =  0.5   *  R - 0.4187 * G - 0.0813* B + 128.;   // Cr
            }
          }              
          pthread_mutex_unlock(&mut_webcam);   
        }
        break;
        case MSG_DIGIT:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;
          if(picI_digit_mutex[msg->msg_id])
          {
            picI_digit_value[msg->msg_id] = msg->value;
            pthread_mutex_unlock(picI_digit_mutex[msg->msg_id]);
          }
        }
        break;
        case MSG_ANALOG:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;
          if(picI_analog_mutex[msg->msg_id])
          {
            picI_analog_value[msg->msg_id] = msg->value;
            pthread_mutex_unlock(picI_analog_mutex[msg->msg_id]);
          }
        }
        break;        
        case MSG_PRINTF:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;          
          printf("from pic %d: %d\n",2,msg->value);
        }
        break;       
        default:
        fprintf(stderr,"<picInterface.cpp> PIC2: Unknown message type %d.\n",get_msg_type(_msg));
        fflush(stdout);            
        break;      
      }
      delete _msg;        
    }
    fprintf(stderr,"Connection closed PIC2...\n");
    fflush(stdout);
    usleep(10000);  
    break;  
  }
  return NULL;
}
//------------------------------------------------------------------------------
int picWebcam(int id, unsigned int *W, unsigned int *H, uint16_t *data)
{
  MSG_INT1_t msg;
  msg.type   = MSG_WEBCAM_QUERY;
  msg.msg_id = msg_id++;
  msg.value = id;
  
  if(write_usb(PIC2, &msg, sizeof(msg))<0)
    return 1;
  else
  {
    pthread_mutex_lock(&mut_webcam);       
    webcam_data = data;
    webcam_W = W;
    webcam_H = H;
    pthread_mutex_lock(&mut_webcam);
    pthread_mutex_unlock(&mut_webcam);       
    return 0;
  }
}
//------------------------------------------------------------------------------
int SendInfo(int type, int input_config)
{
  picInfo_t info;
  info.type = type;
  info.msg_id = msg_id++;
  position_t pos;
  if(type == MSG_POS_INFO)
  {
    pos = cine_get_wheel_center_position();
    //info.destX = input_config;
  }
  else
  {
    pos = cine_get_position();
    position_t dest = pt_get_dest();  
    info.destX = dest.x;
    info.destY = dest.y; 
    info.destA = dest.a;  
      
  }
  info.posX = pos.x;
  info.posY = pos.y;  
  info.posA = pos.a;    
  return write_usb(PIC2, &info, sizeof(info));  
}
//------------------------------------------------------------------------------

