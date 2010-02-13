/*-------------------------------------------------------------------------
  ep0.h - USB endpoint 0 callbacks
-------------------------------------------------------------------------*/

#ifndef EP0_H_
#define EP0_H_

#include "config.h"
#include "usb_std_req.h"

void ep0_init(void);
void ep0_in(void);
void ep0_setup(void);

#endif /*EP0_H_*/
