#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "usb_simul.h"

//------------------------------------------------------------------------------
void connect_to_pc(int robot_id, int pic_id, int *comm_id)
{
  if(*comm_id==-1)
  {
    printf("PIC %d (robot %d) en attente de connection avec le PC...\n>>> ",pic_id, robot_id);
    fflush(stdout); 
    *comm_id=-2;
  }
  get_connection_id(robot_id,pic_id,comm_id);
}
//------------------------------------------------------------------------------
int read_usb(int comm_id, char **msg)
{
  if(comm_id<0) return -1;
  
  char buff[100];
  int len;
  int len_type;
  int len_header;
  int len_data;
  
  len_type = sizeof(int);
  if(read(comm_id,buff,len_type)<=0)
  {
    if(errno!=EAGAIN) return -1;
    else return MSG_EMPTY;  
  }

  len_header = get_msg_header_size(buff);
  if(len_header>0 && read(comm_id,&buff[len_type],len_header)<0)
  {
    return -1;
  }
  len_data = get_msg_data_size(buff);
  if(len_data>0 && read(comm_id,&buff[len_type+len_header],len_data)<0)
  {
    return -1;  
  }
  
  len=len_type+len_header+len_data;  
  *msg=(char*)malloc(len*sizeof(char));
  if(*msg == NULL) return MSG_EMPTY;  
  memcpy(*msg,buff,len);
  return get_msg_type(*msg);   
}
//------------------------------------------------------------------------------
int write_usb(int comm_id,void *msg,int size)
{
  if(comm_id<0) return -1;
  
  int len;
  while(true)
  {
    len = write(comm_id,msg,size);
    if(len>=0 || errno != EAGAIN)
      break;
    else
      usleep(100);
  }
  return len;
}
//------------------------------------------------------------------------------
