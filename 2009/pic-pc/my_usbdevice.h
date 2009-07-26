/*-------------------------------------------------------------------------
  usbdevice.h - docker USB device driver

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------*/


/* $Id: usbdevice.h,v 1.3 2006/08/24 21:19:21 gaufille Exp $ */

#ifndef MY_USBDEVICE_H
#define MY_USBDEVICE_H

using namespace std;

#include <iostream>
#include <sstream>
#include <exception>
#include <usb.h>

#define NB_ANALOGS 4

struct EtatDivers
{
    unsigned short position_servo;
    unsigned char moteur1;
    unsigned char moteur2;
    unsigned char numerique;
    unsigned short analogique[NB_ANALOGS];
};

struct OrderDivers // pour carte divers
{
    unsigned char entete;
    unsigned short position_servo;
    unsigned char moteur1;
    unsigned char moteur2;
};

class USBDeviceDivers
{
    public:
        USBDeviceDivers(struct usb_device *dev);
        ~USBDeviceDivers();
	struct EtatDivers etat; // Contient les infos d'etat des capteurs, servo et moteurs.
	int get_digitals();
	int get_analogs();
	int set_servo(unsigned char position);
	int set_moteurs(unsigned char moteur1, unsigned char moteur2);
    private:
        struct usb_dev_handle* dh;
	int sendandreceive(char *data, int taille_data, char *buffer, int taille_buffer);
};

class USBDeviceMoteur
{
    public:
        USBDeviceMoteur(struct usb_device *dev);
        ~USBDeviceMoteur();
        int sendorder(unsigned char vitesse_gauche, unsigned char vitesse_droite); // envoi les nouvelles vitesses : 1 > arriere toute, 128 > arret, 255 > avant toute / 0 EST INTERDIT !! (l'ordre ne sera pas compris par le pic)
	int get_codeuses(); // Met a jour codeuse_gauche et codeuse_droite
	int codeuse_gauche; // valeurs des codeuses
	int codeuse_droite;
    private:
        struct usb_dev_handle* dh;
};

    
#endif // MY_USBDEVICE_H
