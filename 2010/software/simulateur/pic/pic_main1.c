#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#include "../../common/const.h"
#include "../../common/comm.h"
#include "../../common/simul.h"
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
  int last_iter_send_coder = 0;
  int msg_id;  
  #endif
  int comm_id=-1;   // pour les communications avec le pc  
  
  char *_msg;
  
    float dt;
  float w[2];                // vitesse de rotation
  int codeuse[2]; 
  int gauche, dgauche;
  int droite, ddroite;
  
  struct timeval last_send_coder,act_time,last_time;
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
      #ifdef SIMULATION
      if(Simul->robot->simul_info->dt * ((double)Simul->robot->simul_info->Iterations - last_iter_send_coder) >(((double)TIMER_CODER) / 1000.))
      {
        last_iter_send_coder = Simul->robot->simul_info->Iterations;
        gettimeofday(&last_send_coder,NULL);  
        MSG_INT2_t msg;
        msg.type = CODER;
        msg.msg_id = msg_id--;    
        msg.value1 = get_input1(SIMULARG3(Simul,CODER,0));
        msg.value2 = get_input1(SIMULARG3(Simul,CODER,1));        
        if(write_usb(SIMULARG3(comm_id,&msg,sizeof(msg)))<0)
        {    
          set_output1(SIMULARG4(Simul,MOTORS,0,0));    
          set_output1(SIMULARG4(Simul,MOTORS,1,0));             
          close_connection(&comm_id);  
        }
      }
      #endif      
      int type = read_usb(SIMULARG2(comm_id, &_msg));
      if(type<0)
      {     
        set_output1(SIMULARG4(Simul,MOTORS,0,0));    
        set_output1(SIMULARG4(Simul,MOTORS,1,0));                
        close_connection(&comm_id);
      }
      else switch(type)
      {
        case EMPTY_MSG: break;
        case MOTORS:
        {
          MSG_INT2_t *msg = (MSG_INT2_t *)_msg;
          set_output1(SIMULARG4(Simul,MOTORS,0,msg->value1));    
          set_output1(SIMULARG4(Simul,MOTORS,1,msg->value2));    
        }
        break;
        default:
        #ifdef SIMULATION         
        printf("<pic_main1.c> PIC1 Unknown message type %d.\n",type);
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
