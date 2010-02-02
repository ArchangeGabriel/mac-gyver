#ifndef USBH
#define USBH


#define WANT_PIC1   1
#define WANT_PIC2   1

// picID
enum {PIC1, PIC2};

// initialisation
void connect_init();

// se connecte au pic
int connect_usb(int picID);

// Vérifie que le pic est connecté
bool is_connected(int picID);

// attend un message sur le port usb, opération bloquante (mais on peut changer, suffit de demander)
// renvoie un pointeur sur le message recu (voir common/comm.h) en cas de message, NULL sinon
char* read_usb(int picID);

// ecrit un message sur le port usb  (messages définis dans common/comm.h)
int write_usb(int picID, void *msg, int size);

void close_connection(int picID);
#endif
