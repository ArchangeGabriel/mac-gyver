/*-------------------------------------------------------------------------
  usbdevice.h - docker USB device driver
-------------------------------------------------------------------------*/


/* $Id: usbdevice.h,v 1.3 2006/08/24 21:19:21 gaufille Exp $ */

#ifndef MY_USBDEVICEMOTEURS_H
#define MY_USBDEVICEMOTEURS_H


class USBDeviceMoteur
{
    public:
        USBDeviceMoteur(struct usb_device *dev);
        ~USBDeviceMoteur();
        void stop_application();
        void sendorder(unsigned char vitesse_gauche, unsigned char vitesse_droite); // envoi les nouvelles vitesses :
// 1 > arriere toute, 128 > arret, 255 > avant toute / 0 EST INTERDIT !! (l'ordre ne sera pas compris par le pic)
	unsigned char getcodeuses(int *codeuse1, unsigned char *sens1, int *codeuse, unsigned char *sens2); //retourne l'indice de la transaction
        int repare();
    private:
        struct usb_dev_handle* dh;
        int repare_epout();
        int repare_eps();
};

    
#endif // MY_USBDEVICEMOTEURS_H
