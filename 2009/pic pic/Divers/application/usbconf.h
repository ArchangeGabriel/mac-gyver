// usbconf.h //

#ifndef _USBCONF_H
#define _USBCONF_H

#include "common_types.h"

//#define SECTION_DESCRIPTOR_CMD 5
//extern const uchar section_descriptor [22];

// MY_EPs buffer size
#define MY_EP1_BUFFER_SIZE 64
#define MY_EP2_BUFFER_SIZE 64


/*#define FLASH_SECTION_READ     0x01
#define FLASH_SECTION_WRITE    0x02
extern const uchar section_descriptor [22] = {3,
                                     FLASH_SECTION_READ,
                                     0x00, 0x00, 0x00,
                                     0xff, 0x1f, 0x00,
                                     FLASH_SECTION_READ | FLASH_SECTION_WRITE,
                                     0x00, 0x20, 0x00,
                                     0xff, 0x7f, 0x00,
                                     FLASH_SECTION_READ,
                                     0x00, 0x00, 0x30,
                                     0x0d, 0x00, 0x30};*/


#endif // _USBCONF_H
