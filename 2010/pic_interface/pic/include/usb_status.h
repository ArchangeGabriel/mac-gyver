#ifndef USB_STATUS_H_
#define USB_STATUS_H_

#include "common_types.h"

/* Current device status accessors */
#define SET_DEVICE_STATUS(x)  { usb_device_status = x; }
#define GET_DEVICE_STATUS()  (usb_device_status)

/* USB device statuses */
#define REMOTE_WAKEUP_DIS   0x00
#define REMOTE_WAKEUP_EN    0x02
#define DEVICE_BUS_POWERED  0x00
#define DEVICE_SELF_POWERED 0x01

extern uchar __at(0x005c) usb_device_status;

#endif // USB_STATUS_H_
