// ep2.h //


#ifndef EP2_H_
#define EP2_H_

//#include "config.h"

#include "usbconf.h"

extern uint ep2_num_bytes_to_send;
extern uchar *ep2_source_data;

void my_ep2_init(void);
void my_ep2_in(void);
void my_prepare_ep2_in(void);

#endif /*EP2_H_*/
