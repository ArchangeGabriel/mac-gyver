#ifndef COMM_SERVERH
#define COMM_SERVERH

#include <stdio.h>
#include <errno.h>
#include "../server.h"
#include "../../common/comm.h"

// se connecte au pc
void connect_to_pc(int robot_id, int pic_id, int *comm_id);

// regarde s'il y a un message sur le port usb, opération non bloquante
// renvoit le type de message recu (voir common/comm.h) en cas de message, -1 sinon
// écrit le message dans msg
int read_usb(int comm_id, char **msg);

// écrit sur le port usb
int write_usb(int comm_id,void *msg,int size);
#endif
