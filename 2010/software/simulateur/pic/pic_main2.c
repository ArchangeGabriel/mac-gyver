#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h> 


#include "../../common/comm.h"
#include "../../common/simul.h"
#include "usb.h"
#include "io2.h"

#include "simul.h"

#include "pic_main2.h"

int query_webcam = -1;
int msg_webcam;
uint16_t *pixels;

//------------------------------------------------------------------------------
void* pic_main2(void *Args)
{
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  PIC_ARG *Simul=(PIC_ARG*)Args;
  int comm_id=-1;             // pour les communications avec le pc  
  char *_msg;
         
  while(1)
  {  
    //Communication avec le pc    
    if(comm_id<0)
      connect_to_pc(Simul->my_id,Simul->my_nbr,&comm_id);   
    else
    {
      #ifdef SIMULATION
      if(query_webcam != -1)
      {
        int size = Simul->robot->webcams[query_webcam].getPicture(&pixels);  
        if(size != -1)
        {           
          char f[100];
          sprintf(f, "webcam%d.dat", query_webcam);
          FILE *file = fopen(f, "wb+");
          fwrite(&Simul->robot->webcams[query_webcam].W, sizeof(int), 1, file);
          fwrite(&Simul->robot->webcams[query_webcam].H, sizeof(int), 1, file);
          fwrite(pixels, size, 1, file);
          fclose(file);
          free(pixels);
          
          int len = sizeof(MSG_INT1_t) + strlen(f) + 1;
          char *buffer = (char*) malloc(len);
          MSG_INT1_t *msg = (MSG_INT1_t*)buffer;
          char *data = &buffer[sizeof(MSG_INT1_t)];
          
          msg->type = MSG_WEBCAM;
          msg->msg_id = msg_webcam;
          msg->value = strlen(f) + 1;
          strcpy(data, f);
          
          if(write_usb(comm_id,buffer,len)<0)
            close_connection(&comm_id);
          else
            query_webcam=-1;
          free(buffer);
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
        case MSG_EMPTY: break;
        #ifdef SIMULATION
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
        case MSG_WEBCAM_QUERY:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg; 
          query_webcam = msg->value;
          msg_webcam = msg->msg_id;
        }
        break;  
        #endif               
        case MSG_QUERY:
        {
          MSG_INT1_t *msg = (MSG_INT1_t *)_msg;
          switch(msg->value)
          {
            case MSG_DIGIT:
            {
              MSG_INT1_t answer;
              answer.type   = MSG_DIGIT;
              answer.msg_id = msg->msg_id;
              answer.value  = get_input2(SIMULARG3(Simul,MSG_DIGIT,0));
              if(write_usb(comm_id,&answer,sizeof(MSG_INT1_t))<0)
                close_connection(&comm_id);  
            }
            break;
            case MSG_ANALOG:
            {
              MSG_INT2_t *msg = (MSG_INT2_t *)_msg;
              MSG_INT1_t answer;
              answer.type   = MSG_ANALOG;
              answer.msg_id = msg->msg_id;
              answer.value  = get_input2(SIMULARG3(Simul,MSG_ANALOG,msg->value2));
              if(write_usb(comm_id,&answer,sizeof(MSG_INT1_t))<0)
                close_connection(&comm_id);  
            }            
            break;
/*          case DIST:
            {
              MSG_INTn_t *msg2;
              int *values;
              char *buff = (char*) malloc(sizeof(MSG_INTn_t) + sizeof(int)*4);
              msg2 = (MSG_INTn_t*) buff;
              values = (int*) &buff[sizeof(MSG_INTn_t)];
              
              for(int i=0;i<4;i++)  
                values[i] = get_input2(SIMULARG3(Simul,DIST,(i==1?2:(i==2?1:i)) ));
              
              msg2->type = DIST;
              msg2->msg_id = msg_id--;
              msg2->n = 4;
              
              if(write_usb(SIMULARG3(comm_id,buff,sizeof(MSG_INTn_t) + sizeof(int)*4))<0)
                close_connection(&comm_id);
              free(buff);
            }
            break;*/
            default:
            printf("<pic_main2.c> PIC2 Unknown MSG_QUERY type %d.\n",type);
            fflush(stdout);     
            break;                  
          }
        }
        break;
        case MSG_DCMOTOR:
        case MSG_SERVOMOTOR:
        {
          MSG_INT2_t *msg = (MSG_INT2_t *)_msg;
          set_output2(SIMULARG4(Simul,type,msg->value1,msg->value2));
        }
        break;
        default:
        printf("<pic_main2.c> Unknown message type %d.\n",type);
        fflush(stdout);            
        break;             
      }      
      if(type>0)
        free(_msg);
    }
    
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
