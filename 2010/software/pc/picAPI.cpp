#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../common/simul.h"
#include "../common/comm.h"
#include "usb.h"

#include "path_tracker.h"
#include "picAPI.h"

// Gère les moteurs
void* pic1_handle_msg(void *);
// Gère les reste
void* pic2_handle_msg(void *);

// Fonction de callack
void (*callbackRecvCoder)(int,int) = NULL;
void (*callbackRecvDistCaptors)(int, float*) = NULL;
void (*callbackRecvCoulCaptors)(int, float*) = NULL;
void (*callbackOnJack)(void) = NULL;
void (*callbackOnReconnectPIC1)(void) = NULL;
void (*callbackOnReconnectPIC2)(void) = NULL;

// Compteur de messages
int msg_id;

// Compteur de messages
pthread_mutex_t *mut_hpos;
pthread_mutex_t *mut_vpos;
pthread_mutex_t *mut_clamp;
int msg_id_hpos;
int msg_id_vpos;
int msg_id_clamp;

#ifdef SIMULATION
int picSendInfo();
void (*callbackRecvReset)(void);
pthread_mutex_t *mut_webcam = NULL;
uint16_t *webcam_data;
int *webcam_W;
int *webcam_H;
#else
#define ANALOGS  0x80          // 10000000
#define DIGITALS 0x40          // 01000000
#define MOTEURS  0x20          // 00100000
#define SERVO    0x10          // 00010000

#define POSITION_MOTEUR_1 0x3f // 00111111
#define ACTIV_MOTEUR_1 0x80    // 10000000
#define INITIALISATION_1 0x40  // 01000000

//  nU [4 bits] / Atteinte (0) ou visee (1) (mot 2) / Initialisation / Position moteur 2 (visee ou atteinte) [2 bits]

#define POSITION_MOTEUR_2 0x03 // 00000011
#define INITIALISATION_2 0x04  // 00000100
#define ACTIV_MOTEUR_2 0x08    // 00001000
#define MOTEUR_3 0x30          // 00110000
#define ACTIV_MOTEUR_3 0x40    // 01000000
            *webcam_W = msg->value1;
            *webcam_H = msg->value2;          
            webcam_received = ceil(double((*webcam_W)*(*webcam_H))/double(webcam_buffsize));

#define CLAMP_OPEN_SERVO    0x45
#define CLAMP_LINTEL_SERVO  0x70
#define CLAMP_CLOSE_SERVO   0x55
#endif



//------------------------------------------------------------------------------
void picMainLoop()
{    
  msg_id = 1;
  
  mut_hpos = NULL;   
  mut_vpos = NULL;
  mut_clamp = NULL;    
  msg_id_hpos = 0;
  msg_id_vpos = 0;
  msg_id_clamp = 0;  
  
  fprintf(stderr,"Init USB...                 ");  fflush(stdout);
  connect_init();
  fprintf(stderr,"ok\n");  fflush(stdout);

  pthread_t thread_pic1;  

  pthread_create(&thread_pic1, NULL, &pic1_handle_msg, NULL);  
  pic2_handle_msg(NULL);
  while(true)
    sleep(1);
}
//------------------------------------------------------------------------------
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
    if(callbackOnReconnectPIC1)
      callbackOnReconnectPIC1();
    
    fprintf(stderr,"Listen to PIC1...\n");  fflush(stdout);  
    
    while((_msg=read_usb(PIC1)))
    {
      if(_msg==NULL)
        break;
      
      //printf("Recv PIC1: %d\n",get_msg_type(_msg));
      switch(get_msg_type(_msg))
      {
        case EMPTY_MSG:
        #ifdef SIMULATION            
        fprintf(stderr,"<picInterface.cpp> PIC1: Connexion réinitialisée.\n");
        fflush(stdout); 
        if(callbackRecvReset)
          callbackRecvReset();
        #endif
        break;      
        case CODER:   
        #ifdef SIMULATION
        picSendInfo();
        #endif        
        if(callbackRecvCoder)
        {
          MSG_INT2_t *msg = (MSG_INT2_t *)_msg;
          #ifdef SIMULATION
          callbackRecvCoder(msg->value1, msg->value2);
          #else
          callbackRecvCoder(msg->value2, -msg->value1);
          #endif
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
//------------------------------------------------------------------------------
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
       
    picBegin();

    if(callbackOnReconnectPIC2)
      callbackOnReconnectPIC2();
    
    fprintf(stderr,"Listen to PIC2...\n");  fflush(stdout); 
            
    while((_msg=read_usb(PIC2)))
    {
      if(_msg==NULL)
        break;
      
      //printf("Recv PIC2: %d\n",get_msg_type(_msg));     
      switch(get_msg_type(_msg))
      {
        case EMPTY_MSG:
        #ifdef SIMULATION        
        
        fprintf(stderr,"<picInterface.cpp> PIC2: Connexion réinitialisée.\n");
        fflush(stdout); 
        if(callbackRecvReset)
          callbackRecvReset();
        #endif
        break;
        #ifdef SIMULATION
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
        #endif
        case BEGIN:
        break;
        case JACK:
        {
          callbackOnJack();
          #ifdef SIMULATION
          int buf = JACK;
          if(write_usb(PIC2, &buf, sizeof(int))<0) break;
          #else
          unsigned char buf = JACK;
          if(write_usb(PIC2, &buf, sizeof(buf))<0) break;
          #endif          
        }
        break;  
        case PRINTF:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;          
          printf("from pic %d: %d\n",2,msg->value);
        }
        break;
        case DIST:
        if(callbackRecvDistCaptors)
        {
          MSG_INTn_t *msg = (MSG_INTn_t *)_msg;
          float *tab;
          if(msg->n == 0)
            tab = NULL;
          else
            tab = new float[msg->n];
          int *values = &msg->n;
          values++;         
          for(int i=0; i<msg->n; i++)
          {
            tab[i] = COEFF_A_DIST * pow(COEFF_B_DIST, ((double)(values[i]>>6)));
            if(tab[i]<0.) tab[i] = 0.;        
          }
          
          #ifdef SIMULATION
          float tmp = tab[1];
          values[1] = tab[2];
          values[2] = tmp;  
          #endif
          
          callbackRecvDistCaptors(msg->n, tab);
          
          if(tab != NULL)
            delete tab;
        }
        break;
        case COUL:
        if(callbackRecvCoulCaptors)
        {
          MSG_INTn_t *msg = (MSG_INTn_t *)_msg;
          float *tab = new float[msg->n];
          int *values = &msg->n;
          values++;                      
          for(int i=0; i<msg->n; i++)
            tab[i] = ((float)values[i]);
          callbackRecvCoulCaptors(msg->n, tab);
          delete tab;
        }
        break;        
        
        default:
        //fprintf(stderr,"<picInterface.cpp> PIC2: Unknown message type %d.\n",get_msg_type(_msg));
        //fflush(stdout);            
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

//-----------------------------------------------------------------------------
int picBegin()
{
  #ifdef SIMULATION
  int buf;
  buf = BEGIN;
  
  return write_usb(PIC2,&buf,sizeof(buf));  
  #else
  unsigned char buf;
  buf = BEGIN;
  
  return write_usb(PIC2,&buf,sizeof(buf));
  #endif
}
//------------------------------------------------------------------------------
int picMotorsPower(float pwleft, float pwright)
{
  #ifdef SIMULATION
  if(pwleft>1.) pwleft = 1.;
  if(pwleft<-1.) pwleft = -1.;  
  if(pwright>1.) pwright = 1.;
  if(pwright<-1.) pwright = -1.;    
  MSG_INT2_t msg;
  msg.type   = MOTORS;
  msg.msg_id = msg_id++;
  msg.value1 = ((int)(pwleft*((float)RANGE_MOTOR)));
  msg.value2 = ((int)(pwright*((float)RANGE_MOTOR)));  
  
  return write_usb(PIC1, &msg, sizeof(msg));
  #else
  printf("send: %f,%f\n",pwleft,pwright); fflush(stdout);
  if(pwleft>1.) pwleft = 1.;
  if(pwleft<-1.) pwleft = -1.;  
  if(pwright>1.) pwright = 1.;
  if(pwright<-1.) pwright = -1.;
  pwleft = -pwleft;
  pwright = -pwright;  
  unsigned char buff[2];
  buff[0] = ((unsigned char)(pwleft*127.)) + 128;
  buff[1] = ((unsigned char)(pwright*127.)) + 128; 
  return write_usb(PIC1, buff, 2);  
  #endif
}
//------------------------------------------------------------------------------
int picDistCaptors()
{
  #ifdef SIMULATION
  MSG_INT1_t msg;
  msg.type   = QUERY;
  msg.msg_id = msg_id++;
  msg.value = DIST;
  
  return write_usb(PIC2, &msg, sizeof(MSG_INT1_t));
  #else
  unsigned msg = ANALOGS;
  return write_usb(PIC2, &msg, 1);  
  #endif
}
//------------------------------------------------------------------------------
void picOnRecvJack(void (*fun)(void))
{
  callbackOnJack = fun;
}
//------------------------------------------------------------------------------
void picOnRecvCoder(void (*fun)(int, int))
{
  callbackRecvCoder = fun;
}
//------------------------------------------------------------------------------
void picOnRecvDistCaptors(void (*fun)(int n, float *dist))
{
  callbackRecvDistCaptors = fun;
}
//------------------------------------------------------------------------------
void picOnRecvCoulCaptors(void (*fun)(int n, float *coul))
{
  callbackRecvCoulCaptors = fun;
}
//------------------------------------------------------------------------------
void picOnReconnectPIC1(void (*fun)())
{
  callbackOnReconnectPIC1 = fun;
}
//------------------------------------------------------------------------------
void picOnReconnectPIC2(void (*fun)())
{
  callbackOnReconnectPIC2 = fun;
}
//---------------------------------------------------------------------------//






#ifdef SIMULATION
#include "cinematik.h"
//------------------------------------------------------------------------------
void picRecvReset(void (*fun)(void))
{
  callbackRecvReset = fun;
}          // Convertit de RGB à YUV
          /*for(int i=0; i<size; i+=3)
          {
            const double R = pixels[i];
            const double G = pixels[i+1];
            const double B = pixels[i+2];  
            const double Y = 0.299*R + 0.587*G + 0.114*B;    // Y
            pixels[i+2] = 0.877*(R - Y);  // V
            pixels[i+1] = 0.492*(B - Y);  // U
            pixels[i+0] = Y;
          }*/
//------------------------------------------------------------------------------
int picSendInfo()
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
#endif
