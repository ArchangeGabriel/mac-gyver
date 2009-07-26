// my_ep1.h //


#ifndef MY_EP1_H_
#define MY_EP1_H_

//#include "config.h"

#include "usbconf.h"

extern uint ep2_num_bytes_to_send;
extern uchar *ep2_source_data;

void my_ep1_init(void);
void my_ep1_out(void);

//#define EP1_BUFFER_ADDRESSE 0x500

#endif /*MY_EP1_H_*/
