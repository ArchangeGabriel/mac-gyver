#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <utility>
#include <list>
#include <signal.h>

#include "../common/comm.h"

#include "server.h"

using namespace std;


typedef list< pair< pair<int,int> ,int> > connect_list_t;
typedef list< pair< pair<int,int> ,int*> > robot_list_t;

connect_list_t connect_list;
robot_list_t robot_list;

pthread_mutex_t server_mutex;
int socket_id;

//------------------------------------------------------------------------------
void start_server(int port)
{
  int conn_id;
  struct sockaddr_in serv_addr;
  connect_list_t :: iterator iter;
  robot_list_t :: iterator iter_robot;
  connec_info info;
  int flags;
  pthread_mutex_init(&server_mutex,NULL);

  if((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
  {
    printf("\nERREUR: Impossible d'initialiser le socket. Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    exit(1);      
    return;  }

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // on accepte les connection sur toutes les interfaces
  serv_addr.sin_port = htons(port);

  //on lie le port du serveur au socket
  if (bind(socket_id, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nERREUR: Impossible de lier le socket au port d'écoute\n\
            Utiliser './simulateur -p port' pour changer le port.\n\
            Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    exit(1);    
    return;
  }

  if (listen(socket_id, 5) < 0)
  {
    printf("\nERREUR: Impossible d'écouter le port.\n\
            Utiliser './simulateur -p port' pour changer le port.\n\
            Connection au pic en émulation USB impossible.\n");
    fflush(stdout);  
    exit(1);    
    return;
  }
  
  // On ignore le signal SIGPIPE en cas de rupture de pipe
  struct sigaction act;
  act.sa_handler = SIG_IGN;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;
  sigaction (SIGPIPE, &act, NULL);

  printf("Serveur launched on port %d...\n",port);
  fflush(stdout); 
  while (1)
  {   
    pthread_mutex_unlock(&server_mutex);
    if((conn_id = accept(socket_id, (struct sockaddr *) NULL, NULL)) < 0)
    {
      printf("Un client a tenté de se connecter mais la connection a échouée (code 1)\n>>> ");
      fflush(stdout);  
      continue; 
    }
    pthread_mutex_lock(&server_mutex);
    if (read(conn_id, &info, sizeof(info)) < 0)
    {
      printf("Un client a tenté de se connecter mais la connection a échouée (code 2)\n>>> ");
      fflush(stdout);  
      continue; 
    }
    flags = fcntl (conn_id, F_GETFL);
	  if(fcntl(conn_id, F_SETFL, flags | O_NONBLOCK) < 0)
    {
      printf("Un client a tenté de se connecter mais la connection a échouée (code 3)\n>>> ");
      fflush(stdout);  
      continue; 
    }
    for(iter = connect_list.begin(); iter != connect_list.end(); iter++)
      if((*iter).first.first==info.robot_id && (*iter).first.second==info.pic_id)
      {
        pthread_mutex_unlock(&server_mutex);
        close_connection(&(*iter).second);        
        pthread_mutex_lock(&server_mutex);
        printf("La connection au pic %d du robot %d a changé...\n>>> ",info.pic_id, info.robot_id);        
        fflush(stdout);        
        break;        
      }
    connect_list.push_back(pair< pair<int,int> ,int>(pair<int,int>(info.robot_id,info.pic_id),conn_id));   
  }
}
//------------------------------------------------------------------------------
void get_connection_id(int robot_id, int pic_id, int* comm_id)
{
  connect_list_t :: iterator iter;
  robot_list_t :: iterator iter_robot;  
  pthread_mutex_lock(&server_mutex);
  for(iter_robot = robot_list.begin(); iter_robot != robot_list.end(); iter_robot++)
    if((*iter_robot).first.first==robot_id && (*iter_robot).first.second==pic_id)
    {
      robot_list.erase(iter_robot);
      break;
    }         
  for(iter = connect_list.begin(); iter != connect_list.end(); iter++) 
    if((*iter).first.first==robot_id && (*iter).first.second==pic_id)   
    {
      int msg=MSG_EMPTY;
      write((*iter).second,&msg,sizeof(int));    
      *comm_id=(*iter).second;
      printf("PC connecté au pic n°%d, robot %d...\n>>> ",pic_id, robot_id);
      fflush(stdout);          
      break;
    }
  robot_list.push_back(pair< pair<int,int> ,int*>(pair<int,int>(robot_id,pic_id),comm_id));
  pthread_mutex_unlock(&server_mutex);  
}
//------------------------------------------------------------------------------
void close_connection(int *comm_id)
{
  connect_list_t :: iterator iter;
  pthread_mutex_lock(&server_mutex);
  for(iter = connect_list.begin(); iter != connect_list.end(); iter++)  
    if((*iter).second==*comm_id)
    {      
      printf("Connection au pic %d du robot %d fermée...\n>>> ",(*iter).first.second, (*iter).first.first);        
      fflush(stdout);
      connect_list.erase(iter);      
      break;
    }
  close(*comm_id);  
  *comm_id=-1; 
  pthread_mutex_unlock(&server_mutex);  
}
//------------------------------------------------------------------------------
void close_connections()
{
  connect_list_t :: iterator iter;
  pthread_mutex_lock(&server_mutex);
  for(iter = connect_list.begin(); iter != connect_list.end(); iter++)  
    close((*iter).second);
  close(socket_id);
}
//------------------------------------------------------------------------------
