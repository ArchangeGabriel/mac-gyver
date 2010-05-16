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
#include "../../pc/picAPI.h"
#include "usb.h"
#include "io1.h"

#include "simul.h"

#include "pic_main1.h"

int send_coder = 0;

//------------------------------------------------------------------------------
void* pic_main1(void *Args)
{
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  PIC_ARG *Simul=(PIC_ARG *)Args;
  int last_iter_send_coder = 0;
  int comm_id=-1;   // pour les communications avec le pc  
  
  char *_msg;
  int codeuse[2]; 
  
  struct timeval act_time,last_time;
  gettimeofday(&last_time,NULL);       

  //Initialisation du calcul de la position    
  set_output1(SIMULARG4(Simul,MSG_MOTORS,0,128));
  set_output1(SIMULARG4(Simul,MSG_MOTORS,1,128));       
  codeuse[0]=get_input1(SIMULARG3(Simul,MSG_CODER,0));
  codeuse[1]=get_input1(SIMULARG3(Simul,MSG_CODER,1));  
    
  while(1)
  {  
    gettimeofday(&act_time,NULL); 
    last_time = act_time;
    
    //Communication avec le pc    
    if(comm_id<0)
    {
      connect_to_pc(Simul->my_id,Simul->my_nbr,&comm_id);   
    }
    else
    {
      if(Simul->robot->simul_info->dt * ((double)Simul->robot->simul_info->Iterations - last_iter_send_coder) > PIC_FREQ)
      {
        last_iter_send_coder = Simul->robot->simul_info->Iterations;
        last_time.tv_usec += PIC_FREQ*1000000.;
        if(last_time.tv_usec >= 1000000)
        {
          last_time.tv_sec++;
          last_time.tv_usec = 0;
        }
        send_coder++;
        MSG_INT2_t msg;
        msg.type = MSG_CODER;
        msg.msg_id = send_coder;            
        msg.value1 = -get_input1(SIMULARG3(Simul,MSG_CODER,0));
        msg.value2 = get_input1(SIMULARG3(Simul,MSG_CODER,1));        
        if(write_usb(SIMULARG3(comm_id,&msg,sizeof(msg)))<0)
        {    
          set_output1(SIMULARG4(Simul,MSG_MOTORS,0,128));    
          set_output1(SIMULARG4(Simul,MSG_MOTORS,1,128));             
          close_connection(&comm_id);  
        }
      }
      int type = read_usb(SIMULARG2(comm_id, &_msg));
      if(type<0)
      {     
        set_output1(SIMULARG4(Simul,MSG_MOTORS,0,128));    
        set_output1(SIMULARG4(Simul,MSG_MOTORS,1,128));                
        close_connection(&comm_id);
      }
      else switch(type)
      {
        case MSG_EMPTY: break;
        case MSG_MOTORS:
        {
          MSG_INT2_t *msg = (MSG_INT2_t *)_msg;
          if(msg->value1) set_output1(SIMULARG4(Simul,MSG_MOTORS,0,msg->value1));
          if(msg->value2) set_output1(SIMULARG4(Simul,MSG_MOTORS,1,msg->value2));    
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

   
    codeuse[0]=get_input1(SIMULARG3(Simul,MSG_CODER,0));
    codeuse[1]=get_input1(SIMULARG3(Simul,MSG_CODER,1));
    
    pthread_barrier_wait(Simul->barrier);
    pthread_barrier_wait(Simul->barrier);    
    if(*Simul->STOP)
    {
      *Simul->alive_thread = *Simul->alive_thread-1;
      break;
    }
  }
  free(Args);
  return NULL;
}
//------------------------------------------------------------------------------
