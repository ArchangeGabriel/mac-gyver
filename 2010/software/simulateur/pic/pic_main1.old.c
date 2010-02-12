#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#include "../common/const.h"
#include "../common/comm.h"
#include "../common/simul.h"
#include "usb.h"
#include "io1.h"

#ifdef SIMULATION
#include "simul.h"
#endif

#include "pic_main1.h"

//------------------------------------------------------------------------------
#ifdef SIMULATION
void* pic_main1(void *Args)
#else
void* pic_main1(void *)
#endif
{
  #ifdef SIMULATION
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  PIC_ARG *Simul=(PIC_ARG *)Args;
  #endif
  int comm_id=-1;   // pour les communications avec le pc  
  
  int msg_id=-1;
  char *_msg;
  
  float dt;
  float w[2];                // vitesse de rotation
  int codeuse[2]; 
  int gauche, dgauche;
  int droite, ddroite;
  int U_consigne[2];  // Tension demandée
  int U_safe[2];      // Tension antidérapage
  float U,Umin,Umax;  // Calcul intermédiaire
  U_consigne[0] = 0; 
  U_consigne[1] = 0;  
  U_safe[0] = 0;
  U_safe[1] = 0;
  const float ULim = 0.9 * // Marge de sécurité (mais augmente la distance de freinage/d'accélération)
                     (_MASSE / 3.) * _CONST_G * _ADHERE_ROUE * _RAYON_ROUE / (_CONST_TORQUE * _RAP_REDUC * _RENDEMNT * 0.98);
  
  struct timeval last_send_coder,act_time,last_time;
  int time_since_last_send_coder;
  gettimeofday(&last_send_coder,NULL);   
  gettimeofday(&last_time,NULL);       

  //Initialisation du calcul de la position    
  set_output1(SIMULARG4(Simul,MOTORS,0,0));
  set_output1(SIMULARG4(Simul,MOTORS,1,0));       
  codeuse[0]=get_input1(SIMULARG3(Simul,CODER,0));
  codeuse[1]=get_input1(SIMULARG3(Simul,CODER,1));  
  gauche = codeuse[0];
  droite = codeuse[1];
  
  while(1)
  {  
    gettimeofday(&act_time,NULL); 
    #ifdef SIMULATION
    dt = Simul->robot->simul_info->dt;
    #else
    dt = ((float)(act_time.tv_sec-last_time.tv_sec)) + ((float)(act_time.tv_usec-last_time.tv_usec))*0.000001;  
    #endif
    last_time = act_time;
  
    //Communication avec le pc    
    if(comm_id<0)
    {
      #ifdef SIMULATION      
      connect_to_pc(Simul->my_id,Simul->my_nbr,&comm_id);   
      #else
      connect_to_pc();         
      #endif
    }
    else
    {
      time_since_last_send_coder=(act_time.tv_sec-last_send_coder.tv_sec)*1000000+act_time.tv_usec-last_send_coder.tv_usec;
      if(time_since_last_send_coder>(TIMER_CODER * 1000))
      {
        gettimeofday(&last_send_coder,NULL);  
        MSG_INT2_t msg;
        msg.type = CODER;
        msg.msg_id = msg_id--;    
        msg.value1 = get_input1(SIMULARG3(Simul,CODER,0));
        msg.value2 = get_input1(SIMULARG3(Simul,CODER,1));        
        if(write_usb(SIMULARG3(comm_id,&msg,sizeof(msg)))<0)
        {    
          U_consigne[0] = 0;
          U_consigne[1] = 0;            
          close_connection(&comm_id);  
        }
      }
      int type = read_usb(SIMULARG2(comm_id, &_msg));
      if(type<0)
      {     
        U_consigne[0] = 0;
        U_consigne[1] = 0;               
        close_connection(&comm_id);
      }
      else switch(type)
      {
        case EMPTY_MSG: break;
        case MOTORS:
        {
          MSG_INT2_t *msg = (MSG_INT2_t *)_msg;   
          U_consigne[0] = msg->value1;
          U_consigne[1] = msg->value2;                                       
        }
        break;
        default:
        #ifdef SIMULATION         
        printf("<pic_main.c> PIC1 Unknown message type %d.\n",type);
        fflush(stdout);            
        #endif 
        break;             
      }
      if(type>0)
        free(_msg);
    }

   
    codeuse[0]=get_input1(SIMULARG3(Simul,CODER,0));
    codeuse[1]=get_input1(SIMULARG3(Simul,CODER,1));
    
    dgauche = codeuse[0] - gauche;
    ddroite = codeuse[1] - droite;
    
    gauche = codeuse[0];
    droite = codeuse[1];    
    
   
    if(dt != 0.)
    {  
      w[0] = ((float) dgauche) / (dt * _FREQ_CODER);     
      w[1] = ((float) ddroite) / (dt * _FREQ_CODER);
    }
       
    //Antidérapage
    for(int i=0;i<2;i++)  
    {    
      if(U_safe[i] != U_consigne[i])
      {
        U = ((float)U_consigne[i]) * _TENSION / ((float)RANGE_MOTOR);
        Umax = w[i] / _CONST_SPEED + ULim;
        Umin = w[i] / _CONST_SPEED - ULim;
        if(U > Umax)
          U = Umax;
        if(U < Umin)
          U = Umin;  
        if(U > _TENSION) U = _TENSION;
        else if(U < -_TENSION) U = -_TENSION;
        U_safe[i] = ((int) (U * ((float)RANGE_MOTOR) / _TENSION));

        set_output1(SIMULARG4(Simul,MOTORS,i,U_safe[i]));        
      }
    }
    
    #ifdef SIMULATION
    pthread_barrier_wait(Simul->barrier);
    pthread_barrier_wait(Simul->barrier);    
    if(*Simul->STOP)
    {
      *Simul->alive_thread = *Simul->alive_thread-1;
      break;
    }
    #endif
  }
  #ifdef SIMULATION
  free(Args);
  #endif
  return NULL;
}
//------------------------------------------------------------------------------
