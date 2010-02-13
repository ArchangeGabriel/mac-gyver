// my_usbdevice.cxx //


//using namespace std;

//#include <iostream>
#include <exception>
#include <usb.h>
#include "usbdevicemoteurs.h"

#include "proto.h"

#define EP_OUT 1
#define EP_IN 2

/* In millisecond. Note that with libusb 0.11 on Linux, a timeout of 0 results
 * in a immediate return... */
#define USB_TIMEOUT 5000

USBDeviceMoteur::USBDeviceMoteur(struct usb_device *dev)
{
    int c;
    
    dh = usb_open(dev);
    if(!dh)
    {
        throw "Unable to open device motor.";
    }

    c = usb_claim_interface(dh, 0);
    if(c)
    {
        usb_close(dh);
        throw "Device motor interface 0 unavailable.";
    }
    
    c = usb_set_configuration(dh,2); //Lance l'application du pic
    if(c)
    {
        usb_close(dh);
        throw "Unable to set configuration on motors device.";
    }
}

USBDeviceMoteur::~USBDeviceMoteur()
{
    usb_close(dh);
}

int USBDeviceMoteur::repare_epout()
{
    int c;
    unsigned char sent_bytes[1];

    sent_bytes[0] = ISDEAD;
    
    c = usb_interrupt_write(dh, EP_OUT, (char*)sent_bytes, 1, USB_TIMEOUT);
    if(c <= 0)
    {
        c = usb_clear_halt(dh, EP_OUT);
        if(c != 0)
        {
            return -1;
        }
        c = usb_interrupt_write(dh, EP_OUT, (char*)sent_bytes, 1, USB_TIMEOUT);
        if(c <= 0)
        {
            return -1;
        }
    }
    return 1;
}
    
int USBDeviceMoteur::repare_eps()
{
    int c;
    unsigned char buffer[18];

    c = repare_epout();
    if(c <= 0)
    {
        return -1;
    }
    c = usb_interrupt_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT);
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
        c = usb_interrupt_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT);
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

int USBDeviceMoteur::repare()
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

void USBDeviceMoteur::sendorder(unsigned char vitesse_gauche, unsigned char vitesse_droite) // Attention, 1 > marche arriere toute, 128 > arret, 255 avant toute. 0 EST INTERDIT !! (l'ordre ne sera pas compris comme tel)
{ 
    int c;
    unsigned char sent_bytes[3], buffer[8];

    sent_bytes[0] = SETPWM;
    sent_bytes[1] = vitesse_gauche;
    sent_bytes[2] = vitesse_droite;

    c = usb_interrupt_write(dh, EP_OUT, (char*)sent_bytes, 3, USB_TIMEOUT); 
    if(c <= 0)
    {
        // Retry once
        c = usb_interrupt_write(dh, EP_OUT, (char*)sent_bytes, 3, USB_TIMEOUT); 
        if(c <= 0)
        {
            // Raise exception
            throw "I/O error on PWM command.";
        }
    }
}

unsigned char USBDeviceMoteur::getcodeuses(int *codeuse1, unsigned char *sens1, int *codeuse2, unsigned char *sens2)
{ 
    int c;
    unsigned char buffer[10];

    c=usb_interrupt_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT); // Recevoir !!
    
    //cout << "recieved " << std::hex << (int)c << " octets :";
    //for(int i=0;i<c;i++) cout << " " << (int)buffer[i];
    //cout << endl;
    if(c==10)
    {
        *codeuse1 = *((int*)(buffer+sizeof(unsigned char)));
        *codeuse2 = *((int*)(buffer+sizeof(unsigned char)+sizeof(int)));
        *sens1 = *buffer & MSKSENS0;
        *sens2 = *buffer & MSKSENS1;
        return buffer[9];
    }
    else 
    {
        throw "I/O error on getcodeuses command.";
    }
}
