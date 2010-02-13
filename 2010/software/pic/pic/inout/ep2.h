// my_ep2.h //


#ifndef MY_EP2_H_
#define MY_EP2_H_

//#include "config.h"

extern unsigned int ep2_num_bytes_to_send;
extern unsigned char *ep2_source_data;
//extern unsigned char an_data[];
//extern unsigned char *mydata;

void my_ep2_init(void);
void my_ep2_in(void);
void my_prepare_ep2_in(void);

#endif /*MY_EP2_H_*/
