/*-------------------------------------------------------------------------
  usbdevice.h - docker USB device driver
-------------------------------------------------------------------------*/


/* $Id: usbdevice.h,v 1.3 2006/08/24 21:19:21 gaufille Exp $ */

#ifndef MY_USBDEVICEINOUT_H
#define MY_USBDEVICEINOUT_H

//using namespace std;


//#include "../../Divers/application/analogs.h"

#define NB_ANALOGS 4

class USBDeviceInOut
{
    public:
        USBDeviceInOut(struct usb_device *dev);
        ~USBDeviceInOut();
        void stop_application();
        void servo_order(unsigned char index, unsigned char position);
        void motors_order(unsigned char order);
        unsigned char get_digital_in();
        unsigned char get_analog_in(unsigned short *result, unsigned char number);
        unsigned char init_analog_in(unsigned char number);
        unsigned char motors_last_order;
        int repare();
    private:
        struct usb_dev_handle* dh;
        int repare_epout();
        int repare_eps();
};
    
#endif // USBDEVICEINOUT_H
