// my_usbdevice.cxx //


using namespace std;

#include <iostream>
#include <exception>
#include <usb.h>
#include "my_usbdevice.h"

#include "commandes.h"

#define EP_OUT 1
#define EP_IN 2

/* In millisecond. Note that with libusb 0.11 on Linux, a timeout of 0 results
 * in a immediate return... */
#define USB_TIMEOUT 5000

USBDeviceDivers::USBDeviceDivers(struct usb_device *dev)
{
    int c;
    
    dh = usb_open(dev);
    if(!dh)
    {
        throw "Unable to open device.";
    }

    c = usb_claim_interface(dh, 0);
    if(c)
    {
        usb_close(dh);
        throw "Device interface 0 unavailable.";
    }
}

USBDeviceDivers::~USBDeviceDivers()
{
    usb_close(dh);
}

int USBDeviceDivers::sendandreceive(char *data_to_send, int taille_data, char *buffer, int taille_buffer)
{
    int c;
    c = usb_bulk_write(dh, EP_OUT, data_to_send, taille_data, USB_TIMEOUT); // Envoi de l'ordre
    if(c <= 0) 
    {
	return -1;
    }
   
    c = usb_bulk_read(dh, EP_IN, buffer, taille_buffer, USB_TIMEOUT); // Reception de la reponse
    if(c<=0)
    {
	return -1;
    }
    return c;
}

int USBDeviceDivers::get_analogs()
{
    unsigned char data[1] = {ANALOGS};
    unsigned char buffer[2*NB_ANALOGS+2];
    int tetedelecture;

    sendandreceive((char *)data, 1, (char *)buffer, sizeof(buffer));

    if(buffer[0] & ANALOGS)
    {
        for(tetedelecture=1;tetedelecture<=2*NB_ANALOGS;tetedelecture+=2)
        {
            etat.analogique[tetedelecture >> 1] = *((unsigned short*) &buffer[tetedelecture]);
        }
	return 1;
    }
    else return -1;
}

int USBDeviceDivers::set_servo(unsigned char position)
{
    unsigned char data[2]={SERVO,0x00};
    unsigned char buffer[2];

    data[1]=position;

    sendandreceive((char *)data, 2, (char *)buffer, 2);

    if(buffer[0] & SERVO)
    {
        etat.position_servo = position;
	return 1;
    }
    else return -1;
}

int USBDeviceDivers::set_moteurs(unsigned char moteur1, unsigned char moteur2)
{
    unsigned char data[3] = {MOTEURS, 0x00, 0x00};
    unsigned char buffer[3];
    data[1] = moteur1;
    data[2] = moteur2;

    sendandreceive((char *)data, 3, (char *)buffer, 3);

    if(buffer[0] & MOTEURS)
    {
        etat.moteur1 = buffer[1];
        etat.moteur2 = buffer[2];
	return 1;
    }
    else return -1;
}

int USBDeviceDivers::get_digitals()
{
    unsigned char data[1] = {DIGITALS};
    unsigned char buffer[2];

    sendandreceive((char*)data, 1, (char*)buffer, 2);

    if(buffer[0] & DIGITALS)
    {
        etat.numerique = buffer[1];
	return 1;
    }
    else return -1;
}

USBDeviceMoteur::USBDeviceMoteur(struct usb_device *dev)
{
    int c;
    
    dh = usb_open(dev);
    if(!dh)
    {
        throw "Unable to open device.";
    }

    c = usb_claim_interface(dh, 0);
    if(c)
    {
        usb_close(dh);
        throw "Device interface 0 unavailable.";
    }
}

USBDeviceMoteur::~USBDeviceMoteur()
{
    usb_close(dh);
}

int USBDeviceMoteur::sendorder(unsigned char vitesse_gauche, unsigned char vitesse_droite) // Attention, 1 > marche arriere toute, 1128 > arret, 255 avant toute. 0 EST INTERDIT !! (l'ordre ne sera pas compris comme tel)
{ 
    int c;
    unsigned char sent_bytes[2], buffer[8];

    sent_bytes[0] = vitesse_gauche;
    sent_bytes[1] = vitesse_droite;

    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 2, USB_TIMEOUT); // Envoyer !!
    if(c <= 0)
    {
        // Raise exception
        throw "I/O error on device erase command.";
    }
    //cout << "sent !" << endl;

    c=usb_bulk_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT); // Recevoir !!
    
    //cout << "recieved " << std::hex << (int)c << " octets :";
    //for(int i=0;i<c;i++) cout << " " << (int)buffer[i];
    //cout << endl;

    codeuse_gauche = ((int*)buffer)[0];
    codeuse_droite = ((int*)buffer)[1];

    return c;
}

int USBDeviceMoteur::get_codeuses()
{ 
    int c;
    unsigned char sent_bytes[1], buffer[8];

    sent_bytes[0] = 0;
    
    c = usb_bulk_write(dh, EP_OUT, (char*)sent_bytes, 1, USB_TIMEOUT); // Envoyer !!
    if(c <= 0)
    {
        // Raise exception
        throw "I/O error on device erase command.";
    }
    //cout << "sent !" << endl;

    c=usb_bulk_read(dh, EP_IN, (char*)buffer, sizeof(buffer), USB_TIMEOUT); // Recevoir !!
    
    //cout << "recieved " << std::hex << (int)c << " octets :";
    //for(int i=0;i<c;i++) cout << " " << (int)buffer[i];
    //cout << endl;

    codeuse_gauche = ((int*)buffer)[0];
    codeuse_droite = ((int*)buffer)[1];

    return c;
}
