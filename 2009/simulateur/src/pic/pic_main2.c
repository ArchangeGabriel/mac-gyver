#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "../common/comm.h"
#include "../common/simul.h"
#include "usb.h"
#include "io2.h"

#ifdef SIMULATION
#include "simul.h"
#endif

#include "pic_main2.h"

int query_hpos  = -1;
int query_vpos  = -1;
int query_clamp = -1;
int msg_hpos;
int msg_vpos;
int msg_clamp;

#ifdef SIMULATION
int query_webcam = -1;
int msg_webcam;
unsigned char *pixels;
#endif

//------------------------------------------------------------------------------
#ifdef SIMULATION
void* pic_main2(void *Args)
#else
void* pic_main2(void *)
#endif
{
  #ifdef SIMULATION
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  PIC_ARG *Simul=(PIC_ARG*)Args;
  #endif
  int comm_id=-1;             // pour les communications avec le pc  
  
  int msg_id=-1;
  char *_msg;
  bool started = false;
         
  while(1)
  {  
    //Communication avec le pc    
    if(comm_id<0)
    {
      #ifdef SIMULATION      
      connect_to_pc(Simul->my_id,Simul->my_nbr,&comm_id);   
      started = false;      
      #else
      connect_to_pc();         
      #endif
    }
    else
    {
      if(query_hpos != -1)
      {    
        if(query_hpos == get_input2(SIMULARG2(Simul,HPOS)))
        {
          MSG_INT1_t msg;
          msg.type = ACK;
          msg.msg_id = msg_hpos;
          msg.value = HPOS;
          if(write_usb(SIMULARG3(comm_id,&msg,sizeof(MSG_INT1_t)))<0)
            close_connection(&comm_id);          
          else
            query_hpos=-1;
        }
      }
      if(query_vpos != -1)
      {  
        if(query_vpos == get_input2(SIMULARG2(Simul,VPOS)))
        {
          MSG_INT1_t msg;
          msg.type = ACK;
          msg.msg_id = msg_vpos;
          msg.value = VPOS;
          if(write_usb(SIMULARG3(comm_id,&msg,sizeof(MSG_INT1_t)))<0)
            close_connection(&comm_id);          
          else
            query_vpos=-1;
        }
      }
      if(query_clamp != -1)
      {  
        if(query_clamp == get_input2(SIMULARG2(Simul,CLAMP)))
        {
          MSG_INT1_t msg;
          msg.type = ACK;
          msg.msg_id = msg_clamp;
          msg.value = CLAMP;
          if(write_usb(SIMULARG3(comm_id,&msg,sizeof(MSG_INT1_t)))<0)
            close_connection(&comm_id);          
          else
            query_clamp=-1;
        }       
      }   
      #ifdef SIMULATION
      if(query_webcam != -1)
      {
        int size = Simul->robot->webcams[query_webcam].getPicture(&pixels);
        if(size != -1)
        {
          char *msg = (char*) malloc(sizeof(MSG_INT1_t) + size);
          ((MSG_INT1_t*)msg)->type = WEBCAM;
          ((MSG_INT1_t*)msg)->msg_id = msg_webcam;
          ((MSG_INT1_t*)msg)->value = size;
          int *ptr = (int*) &msg[sizeof(MSG_INT1_t)];
          ptr++;
          memcpy(ptr,pixels,size); 
          if(write_usb(comm_id,msg,sizeof(MSG_INT1_t)+size)<0)
            close_connection(&comm_id);  
          else
            query_webcam=-1;            
          free(pixels);
          free(msg);
        }
      }
      #endif         
      int type = read_usb(SIMULARG2(comm_id, &_msg));
      if(type<0)
      {        
        close_connection(&comm_id);
      }
      else switch(type)
      {
        case EMPTY_MSG: break;
        #ifdef SIMULATION
        case BEGIN:
        case JACK:
        break;
        case MSG_INFO:
        {
          picInfo_t *msg = (picInfo_t*)_msg;
          Simul->robot->data[Simul->my_nbr].destX=msg->destX;
          Simul->robot->data[Simul->my_nbr].destY=msg->destY;
          Simul->robot->data[Simul->my_nbr].destA=msg->destA;  
          Simul->robot->data[Simul->my_nbr].posX=msg->posX;
          Simul->robot->data[Simul->my_nbr].posY=msg->posY;
          Simul->robot->data[Simul->my_nbr].angle=msg->posA;
        }
        break;  
        case WEBCAM_QUERY:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg; 
          query_webcam = msg->value;
          msg_webcam = msg->msg_id;
        }
        break;         
        #endif     
        case QUERY:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;
          switch(msg->value)
          {
            case DIST:
            {
              MSG_INTn_t *msg2;
              int *values;
              char *buff = (char*) malloc(sizeof(MSG_INTn_t) + sizeof(int)*4);
              msg2 = (MSG_INTn_t*) buff;
              values = (int*) &buff[sizeof(MSG_INTn_t)];
              
              for(int i=0;i<4;i++)  
                values[i] = get_input2(SIMULARG3(Simul,DIST,i));
              
              msg2->type = DIST;
              msg2->msg_id = msg_id--;
              msg2->n = 4;
              
              if(write_usb(SIMULARG3(comm_id,buff,sizeof(MSG_INTn_t) + sizeof(int)*4))<0)
                close_connection(&comm_id);
              free(buff);
            }
            break;
            #ifdef SIMULATION              
            case COUL:
            break;                                
            #endif
            default:
            #ifdef SIMULATION               
            printf("<pic_main2.c> PIC2 Unknown query type %d.\n",type);
            fflush(stdout);     
            #endif       
            break;                  
          }
        }
        case BELT: 
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;   
          set_output2(SIMULARG4(Simul,msg->type,0,msg->value));
        }
        break;         
        case VPOS:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;
          set_output2(SIMULARG4(Simul,msg->type,0,msg->value));
          query_vpos = msg->value;
          msg_vpos = msg->msg_id;
        }
        break;         
        case HPOS:
        {      
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;   
          set_output2(SIMULARG4(Simul,msg->type,0,msg->value));
          query_hpos = msg->value;
          msg_hpos = msg->msg_id;          
        }
        break;         
        case CLAMP:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;
          set_output2(SIMULARG4(Simul,msg->type,0,msg->value));
          query_clamp = msg->value;
          msg_clamp = msg->msg_id;          
        }
        break;
        default:
        #ifdef SIMULATION           
        printf("<pic_main2.c> Unknown message type %d.\n",type);
        fflush(stdout);            
        #endif
        break;             
      }      
      if(!started && get_input2(SIMULARG3(Simul,JACK,0)))
      {
        started = true;
        int msg;
        msg = JACK;
        if(write_usb(SIMULARG3(comm_id,&msg,sizeof(int)))<0)
          close_connection(&comm_id);  
      }
      if(type>0)
        free(_msg);
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
