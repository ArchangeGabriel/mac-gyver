#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../common/comm.h"

int socket_id[2] = {-1, -1};
bool connected[2] = {false, false};

char *addr;
int port;
int robot_id;

typedef struct _connec_info
{
  int robot_id;
  int pic_id;
} connec_info;


//------------------------------------------------------------------------------
void connect_init()
{
}
//------------------------------------------------------------------------------
int connect_usb(int picID)
{
  struct sockaddr_in serv_addr;
  connec_info info;  

  printf(">>> Tentative de connection à %s:%d, pour le pic n°%d, robot %d\n",addr,port,picID,robot_id);
  fflush(stdout); 
      
  if((socket_id[picID] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\nERREUR: Impossible d'initialiser le socket. Connection au pic en émulation USB impossible.\n");
    fflush(stdout);     
    return -1;
  }

  bzero(&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0)
  {
    printf("\nERREUR: Impossible de résoudre l'adresse du serveur.\n\
            Utiliser './client -a adresse -p port' pour changer le port.\n\
            Connection au pic en émulation USB impossible.\n");
    fflush(stdout);             
    return -1;
  }

  if (connect(socket_id[picID], (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {    printf("\nERREUR: Impossible de se connecter au serveur\n\
            Utiliser './client -a adresse -p port' pour changer le port.\n\
            Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    return -1;
  }

  info.robot_id = robot_id;
  info.pic_id = picID;  
  if(write(socket_id[picID],&info,sizeof(info)) < 0)
  {
    printf("\nERREUR: Aucune réponse du serveur. Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    return -1;  
  }
  
  printf(">>> En attente d'une réponse du pic...\n");
  fflush(stdout);  
  
  int msg;
  if(read(socket_id[picID],&msg,sizeof(int)) < 0)
  {
    printf("ERREUR: Aucune réponse du serveur. Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    return -1;  
  }
  
  if(msg!=EMPTY_MSG)
  {
    printf("ERREUR: Protocole non respecté. Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    return -1;    
  }
  
  if(socket_id[picID] < 0)
    printf(">>> Echec de la connection...\n");
  else
  {
    printf(">>> Connecté au pic...\n>>> ");
    fflush(stdout);
  }
  
  connected[picID]=true;
  return socket_id[picID];
}
//------------------------------------------------------------------------------
bool is_connected(int picID)
{
  return connected[picID];
}
//------------------------------------------------------------------------------
char* read_usb(int picID)
{
  if(!connected[picID]) return NULL;

  char buff[100];
  int len;
  int len_type;
  int len_header;
  int len_data;
  len_type = sizeof(int);  
  if(read(socket_id[picID],buff,len_type)<=0)
    return NULL;
  len_header=get_msg_header_size(buff);
  if(len_header>0 && read(socket_id[picID],&buff[len_type],len_header)<0)
    return NULL;
  len_data=get_msg_data_size(buff);
  len=len_type+len_header+len_data;  
  char *msg=new char[len]; 
  memcpy(msg,buff,len_type+len_header);   
  int r = read(socket_id[picID],&msg[len_type+len_header],len_data);
  if(r > 100)
    printf("r = %d, l = %d\n",r, len_data);
  if(len_data>0 && r<0)
    return NULL;
  return msg;    
}
//------------------------------------------------------------------------------
int write_usb(int picID,void *msg,int size)
{
  if(!connected[picID]) return -1;
  if(write(socket_id[picID],msg,size)<0)
    return -1;  
  return 0;
}
//------------------------------------------------------------------------------
void close_connection(int picID)
{
  close(socket_id[picID]);
  socket_id[picID]=-1;
  connected[picID]=false;
}
//------------------------------------------------------------------------------
