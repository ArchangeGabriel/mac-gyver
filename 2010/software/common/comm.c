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
    case MSG_WEBCAM:
      return 2*sizeof(int);
    break;
    case MSG_WEBCAM_QUERY:
      return sizeof(int);
    break;    
    case MSG_INFO:
    case MSG_POS_INFO:
      return sizeof(int);    
    break;
    #endif
    case MSG_EMPTY:
      return 0;
    break;
    case MSG_QUERY:    
    case MSG_MOTORS:
    case MSG_CODER:    
    case MSG_PRINTF:
    case MSG_DIGIT:
    case MSG_DCMOTOR:
    case MSG_SERVOMOTOR:
      return sizeof(int);
    break;
    case MSG_ANALOG:
      return 2*sizeof(int);    
    break;
    default:
    fprintf(stderr,"<comm.c> get_msg_header_size() : Unknown message type. %d\n",get_msg_type(msg));
    fflush(stdout);
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
    case MSG_WEBCAM:
      return ((MSG_INT1_t*)msg)->value;
    case MSG_WEBCAM_QUERY:
      return sizeof(MSG_INT1_t) - 2*sizeof(int);
    break;
    case MSG_INFO:
    case MSG_POS_INFO:
      return sizeof(picInfo_t) - 2*sizeof(int);
    break;
    #endif  
    case MSG_EMPTY:
      return 0;
    break;    
    case MSG_QUERY:
    case MSG_PRINTF:
    case MSG_DIGIT:    
      return sizeof(MSG_INT1_t) - 2*sizeof(int);
    break;
    case MSG_MOTORS:
    case MSG_CODER: 
    case MSG_DCMOTOR:
    case MSG_SERVOMOTOR:
      return sizeof(MSG_INT2_t) - 2*sizeof(int);
    break;
    case MSG_ANALOG:
      return sizeof(int)*((MSG_INTn_t*)msg)->n;
    break;
    default:
    fprintf(stderr,"<comm.c> get_msg_data_size() : Unknown message type. %d\n",get_msg_type(msg));
    fflush(stdout);
    break;  
  }
  return 0;
}
//------------------------------------------------------------------------------
