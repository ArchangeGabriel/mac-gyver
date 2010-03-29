#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//------------------------------------------------------------------------------
int get_msg_type(char *msg)
{
  return ((int*)msg)[0];
}
//------------------------------------------------------------------------------
int get_msg_header_size(char *msg)
{
 switch(get_msg_type(msg))
  {
    #ifdef SIMULATION
    case WEBCAM:
      return 2*sizeof(int);
    break;
    case WEBCAM_QUERY:
      return sizeof(int);
    break;    
    case MSG_INFO:
      return sizeof(int);    
    break;
    #endif
    case BEGIN:
    case JACK:
    case EMPTY_MSG:
      return 0;
    break;
    case QUERY:    
    case ACK:
    case MOTORS:
    case CODER:    
    case PRINTF:
      return sizeof(int);
    break;
    case DIST:     
      return 2*sizeof(int);
    break;  
    default:
    //fprintf(stderr,"<comm.c> get_msg_header_size() : Unknown message type. %d\n",get_msg_type(msg));
    //fflush(stdout);
    break;  
  }
  return 0;
}
//------------------------------------------------------------------------------
int get_msg_data_size(char *msg)
{
  switch(get_msg_type(msg))
  {
    #ifdef SIMULATION
    case WEBCAM:
      return ((MSG_INT1_t*)msg)->value;
    case WEBCAM_QUERY:
      return sizeof(MSG_INT1_t) - 2*sizeof(int);
    break;
    case MSG_INFO:
      return sizeof(picInfo_t) - 2*sizeof(int);
    break;
    #endif  
    case BEGIN:
    case JACK:
    case EMPTY_MSG:
      return 0;
    break;    
    case QUERY:
    case ACK:
    case PRINTF:
      return sizeof(MSG_INT1_t) - 2*sizeof(int);
    break;
    case MOTORS:
    case CODER: 
      return sizeof(MSG_INT2_t) - 2*sizeof(int);
    break;
    case DIST:   
      return sizeof(int)*((MSG_INTn_t*)msg)->n;
    break;  
    default:
    //fprintf(stderr,"<comm.c> get_msg_data_size() : Unknown message type. %d\n",get_msg_type(msg));
    //fflush(stdout);
    break;  
  }
  return 0;
}
//------------------------------------------------------------------------------
