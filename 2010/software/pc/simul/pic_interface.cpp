#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#define PC_INCLUDE
#include "../../common/comm.h"
#include "../common.h"
#include "../usb.h"
#include "../picAPI.h"
#include "../cinematik.h"
#include "../path_tracker.h"

// Compteur de messages
int msg_id;

// La webcam est gérée par le serveur dans le cas de la simulation
pthread_mutex_t *mut_webcam = NULL;
uint16_t *webcam_data;
int *webcam_W;
int *webcam_H;

// Gère les moteurs
void* pic1_handle_msg(void *);
pthread_t thread_pic1;  

// Gère les reste
void* pic2_handle_msg(void *);
pthread_t thread_pic2;

// Pour visualiser la position supposée et la destination sur le simulateur
int SendInfo();
                
// Compteur de messages
pthread_mutex_t *mut_hpos;
pthread_mutex_t *mut_vpos;
pthread_mutex_t *mut_clamp;
int msg_id_hpos;
int msg_id_vpos;
int msg_id_clamp;   

// Valeur des codeuses
int last_coder_L = 0;
int last_coder_R = 0;
int coder_L = 0;
int coder_R = 0;    
int coder_id = 0;         

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
  
  return write_usb(PIC1, &msg, sizeof(msg));
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
  // TODO
  return 0;
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
  // TODO
  return 0;
}
//------------------------------------------------------------------------------
// met le moteur number (1-4) dans le sens sens (-1, 0 arret, 1). Retourne 1 si ok, -1 sinon.
int set_DC_motor(unsigned char number, char sens)
{
  // TODO
  return 0;
}
//------------------------------------------------------------------------------
//met le servo number (1-4) en position position (1-255) (position = 0 rend le servo libre). Retourne 1 si ok, -1 sinon
int set_servo(unsigned char number, unsigned char position)
{
  // TODO
  return 0;
}
//------------------------------------------------------------------------------
// Initialise les connexions usb avec la carte. Retourne 1 si tout fonctionne, un resultat negatif sinon.
int setup_usb_connexions()
{
  msg_id = 1;
  mut_hpos = NULL;   
  mut_vpos = NULL;
  mut_clamp = NULL;    
  msg_id_hpos = 0;
  msg_id_vpos = 0;
  msg_id_clamp = 0;  

  pthread_create(&thread_pic1, NULL, &pic1_handle_msg, NULL);  
  pthread_create(&thread_pic2, NULL, &pic2_handle_msg, NULL);    
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
  // TODO
  return 1;
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
      sleep(1);
      continue;
    }     
    
    fprintf(stderr,"Listen to PIC1...\n");  fflush(stdout);  
    
    while((_msg=read_usb(PIC1)))
    {
      if(_msg==NULL)
        break;
      
      switch(get_msg_type(_msg))
      {
        case EMPTY_MSG:
        fprintf(stderr,"<picInterface.cpp> PIC1: Connexion réinitialisée.\n");
        fflush(stdout); 
        pic_Reset();
        break;      
        case CODER:
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
        case PRINTF:
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
      sleep(1);
      continue;
    }

    fprintf(stderr,"Listen to PIC2...\n");  fflush(stdout); 
            
    while((_msg=read_usb(PIC2)))
    {
      if(_msg==NULL)
        break;
      
      //printf("Recv PIC2: %d\n",get_msg_type(_msg));     
      switch(get_msg_type(_msg))
      {
        case EMPTY_MSG:       
        fprintf(stderr,"<picInterface.cpp> PIC2: Connexion réinitialisée.\n");
        fflush(stdout); 
        pic_Reset();
        break;
        case WEBCAM:
        {
          char *f = (char*)(_msg+sizeof(MSG_INT1_t));
          FILE *file = fopen(f, "rb");
          
          fread(webcam_W, sizeof(int), 1, file);
          fread(webcam_H, sizeof(int), 1, file);
          if(webcam_data)
          {
            fread(webcam_data, sizeof(uint16_t)*3*(*webcam_W)*(*webcam_H), 1, file);  
            // Convertit de RGB à YUV
            for(int i=0; i<3*(*webcam_W)*(*webcam_H); i+=3)
            {
              const double R = webcam_data[i+0]&255;
              const double G = webcam_data[i+1]&255;
              const double B = webcam_data[i+2]&255;  
              webcam_data[i+0] =  0.299  * R + 0.587  * G + 0.114 * B;          // Y 
              webcam_data[i+1] = -0.1687 * R - 0.3313 * G + 0.5   * B + 128.;   // Cb
              webcam_data[i+2] =  0.5   *  R - 0.4187 * G - 0.0813* B + 128.;   // Cr
            }
          }
          
          pthread_mutex_t *cpy = mut_webcam;
          mut_webcam = NULL;                       
          pthread_mutex_unlock(cpy);   
          //save_buff_to_bitmap("img.bmp", *webcam_W, *webcam_H, webcam_data);           
        }
        break;
        case JACK:
        {
          pic_Jack();
          int buf = JACK;
          if(write_usb(PIC2, &buf, sizeof(int))<0) break;
        }
        break;  
        case PRINTF:
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
pthread_mutex_t* picWebcam(int id, int *W, int *H, uint16_t *data)
{
  MSG_INT1_t msg;
  msg.type   = WEBCAM_QUERY;
  msg.msg_id = msg_id++;
  msg.value = id;
  
  if(write_usb(PIC2, &msg, sizeof(msg))<0)
    return NULL;
  else
  {
    if(mut_webcam != NULL)
    {
      fprintf(stderr,"<picInterface.cpp> Webcam mutex already locked.\n");
      fflush(stdout);
      pthread_mutex_unlock(mut_webcam);
    }
    mut_webcam = new pthread_mutex_t;
    pthread_mutex_init(mut_webcam,NULL);
    pthread_mutex_lock(mut_webcam);        
    webcam_data = data;
    webcam_W = W;
    webcam_H = H;
    return mut_webcam;
  }
}
//------------------------------------------------------------------------------
int SendInfo()
{
  picInfo_t info;
  info.type = MSG_INFO;
  info.msg_id = msg_id++;
  position_t pos = cine_get_position();
  position_t dest = pt_get_dest();  
  info.posX = pos.x;
  info.posY = pos.y;  
  info.posA = pos.a;    
  info.destX = dest.x;
  info.destY = dest.y; 
  info.destA = dest.a;  
  
  return write_usb(PIC2, &info, sizeof(info));  
}
//------------------------------------------------------------------------------

