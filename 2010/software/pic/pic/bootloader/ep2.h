/*-------------------------------------------------------------------------
  ep2.h - USB endpoint 2 callbacks
-------------------------------------------------------------------------*/

#ifndef EP2_H_
#define EP2_H_

#include "config.h"

void ep2_init(void);
void ep2_in(void);
void prepare_ep2_in(void);

//extern volatile uchar ep2_InBuffer[EP2_BUFFER_SIZE];

#endif /*EP2_H_*/
