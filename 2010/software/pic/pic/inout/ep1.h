// my_ep1.h //


#ifndef MY_EP1_H_
#define MY_EP1_H_

#include "app_usb.h"
#include <pic18fregs.h>
#include <boot_iface.h>
#include "common_types.h"
#include "usbconf.h"

void my_ep1_init(void);
void my_ep1_out(void);

#endif /*MY_EP1_H_*/
