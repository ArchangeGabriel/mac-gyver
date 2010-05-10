// my_usbdevice.cxx //


using namespace std;

#include <iostream>
#include <exception>
#include <usb.h>
#include "usbdeviceinout.h"

#include "../../pic/protocole/proto.h"

#define EP_OUT 1
#define EP_IN 2

/* In millisecond. Note that with libusb 0.11 on Linux, a timeout of 0 results
 * in a immediate return... */
#define USB_TIMEOUT 5000

USBDeviceInOut::USBDeviceInOut(struct usb_device *dev)
{
    int c;
    
    dh = usb_open(dev);
    if(!dh)
    {
        throw "Unable to open device InOut.";
    }

    c = usb_set_configuration(dh,3); //Lance l'application du pic
    if(c)
    {
        usb_close(dh);
        throw "Unable to set configuration on inout device.";
    }
    
    c = usb_claim_interface(dh, 0);
    if(c)
    {
        usb_close(dh);
        throw "Device InOut interface 0 unavailable.";
    }
    motors_last_order = 0;
}

USBDeviceInOut::~USBDeviceInOut()
{
    int c;
    c = usb_release_interface(dh,0);
    if(c) cout << "Unable to release interface on inout device." << endl;
    c = usb_set_configuration(dh,1);
    if(c) cout << "Unable to change configuration on inout device." << endl;
    usb_close(dh);
}


int USBDeviceInOut::repare_epout()
{
    int c;
    unsigned char sent_bytes[1];

    sent_bytes[0] = ISDEAD;
    
    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 1, USB_TIMEOUT);
    if(c <= 0)
    {
        c = usb_clear_halt(dh, EP_OUT);
        if(c != 0)
        {
            return -1;
        }
        c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 1, USB_TIMEOUT);
        if(c <= 0)
        {
            return -1;
        }
    }
    return 1;
}
    
int USBDeviceInOut::repare_eps()
{
    int c;
    unsigned char buffer[18];

    c = repare_epout();
    if(c <= 0)
    {
        return -1;
    }
    c = usb_bulk_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT);
    if(c <= 0)
    {
        c = usb_clear_halt(dh, EP_IN);
        if(c != 0)
        {
            return -1;
        }
        c = repare_epout();
        if(c <= 0)
        {
            return -1;
        }
        c = usb_bulk_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT);
        if(c <= 0)
        {
            return -1;
        }
    }
    if(buffer[0] == ISDEAD)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int USBDeviceInOut::repare()
{
    int c;
    c = repare_eps();
    if(c <= 0)
    {
        usb_reset(dh);
        return -1;
    }
    return 1;
}

void USBDeviceInOut::get_debug()
{
    int c;
    unsigned char buf[9];
    buf[0] = DEBUGS;

    c = usb_bulk_write(dh, EP_OUT, (char*)buf, 1, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Raise exception
        throw "I/O error on DEBUGS command.";
    }
    c = usb_bulk_read(dh, EP_IN, (char*)buf, sizeof(buf), USB_TIMEOUT);
    if(c <= 0)
    {
        throw "I/O error on DEBUGS command.";
    }
    if(buf[0] == DEBUGS)
    {
        cout << "Debugs : " << (int)buf[1];
        for(c=2;c<9;c++)
        {
            cout << " ; " << (int)buf[c];
        }
        cout << endl;
    }
    else
    {
        throw "I/O error on DEBUGS command.";
    }
}

void USBDeviceInOut::servo_order(unsigned char index, unsigned char position)
{ 
    int c;
    unsigned char sent_bytes[3];

    sent_bytes[0] = SERVOS;
    sent_bytes[1] = index;
    sent_bytes[2] = position;

    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 3, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Retry once
        c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 3, USB_TIMEOUT); 
        if(c <= 0)
        {
            // Raise exception
            throw "I/O error on SERVOS command.";
        }
    }
}

void USBDeviceInOut::motors_order(unsigned char order)
{ 
    int c;
    unsigned char sent_bytes[2];

    sent_bytes[0] = MOTORS;
    sent_bytes[1] = order;

    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Retry once
        c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); 
        if(c <= 0)
        {
            // Raise exception
            throw "I/O error on MOTORS command.";
        }
    }
    motors_last_order = order;
}

unsigned char USBDeviceInOut::get_digital_in()
{ 
    int c;
    unsigned char sent_bytes, buffer[18];

    sent_bytes = DIGITS;

    c = usb_bulk_write(dh, EP_OUT, (char*)&sent_bytes, 1, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Retry once
        c = usb_bulk_write(dh, EP_OUT, (char*)&sent_bytes, 1, USB_TIMEOUT); 
        if(c <= 0)
        {
            // Raise exception
            cout << "toto" << endl;
            throw "I/O error on DIGITS command.";
        }
    }
    c = usb_bulk_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT);
    if(c <= 0)
    {
        throw "I/O error on DIGITS command.";
    }
    if(buffer[0] == DIGITS)
    {
        return buffer[1];
    }
    else
    {
        throw "Result error on DIGITS command.";
    }
}

unsigned char USBDeviceInOut::get_analog_in(unsigned short *result, unsigned char number)
{ 
    int c;
    unsigned char sent_bytes[2], buffer[18];
    unsigned char *resul;

    sent_bytes[0] = ANALOG;
    sent_bytes[1] = number;

    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Retry once
        c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); 
        if(c <= 0)
        {
            // Raise exception
            throw "I/O error on ANALOG command.";
        }
    }
    c = usb_bulk_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT);
    if(c <= 0)
    {
        throw "I/O error on ANALOG command.";
    }
    if((buffer[0] == ANALOG)&&(buffer[1] <= number))
    {
        resul = (unsigned char *) result;
        for(c=0;c<2*buffer[1];c++)
        {
            resul[c] = buffer[c+2];
        }
        return buffer[1];
    }
    else
    {
        throw "Result error on ANALOG command.";
    }
}

unsigned char USBDeviceInOut::init_analog_in(unsigned char number)
{ 
    int c;
    unsigned char sent_bytes[2];

    sent_bytes[0] = INITAN;
    if(number > NB_ANALOGS)
    {
        sent_bytes[1] = NB_ANALOGS;
    }
    else
    {
        sent_bytes[1] = number;
    }

    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Retry once
        c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); 
        if(c <= 0)
        {
            // Raise exception
            throw "I/O error on INITAN command.";
        }
    }
    return sent_bytes[1];
}
