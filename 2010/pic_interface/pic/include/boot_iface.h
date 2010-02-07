/*-------------------------------------------------------------------------
  boot_iface.h - Boot API
-------------------------------------------------------------------------*/

#ifndef BOOT_H_
#define BOOT_H_

#include "common_types.h"

void boot_goto_table(void);

void dispatch_usb_event(void);
void reset_usb(void);
void usb_sleep(void);
void fill_in_buffer(uchar EPnum,
                    uchar **source,
                    uint buffer_size, 
                    uint *nb_byte);
                    
void ep0_init(void);
void ep0_in(void);
void ep0_setup(void);

void ep1_init(void);
void ep1_out(void);

void ep2_init(void);
void ep2_in(void);

extern uchar __at(0x005f) usb_device_state;
extern uchar __at(0x005e) usb_active_cfg;
extern uchar __at(0x005d) usb_active_alt_setting;



#endif /*APPLICATION_H_*/
