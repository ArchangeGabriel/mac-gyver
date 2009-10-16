#ifndef SERVERH
#define SERVERH

typedef struct _connec_info
{
  int robot_id;
  int pic_id;
} connec_info;

void start_server(int port);    // lance l'écoute sur le port indiqué
void get_connection_id(int robot_id, int pic_id, int* comm_id);
void close_connection(int *comm_id);
void close_connections();

#endif
