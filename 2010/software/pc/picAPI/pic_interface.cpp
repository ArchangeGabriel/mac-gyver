// pic_interface.cxx //
#include "../../common/simul.h"
#include "pic_interface.h"

#ifdef SIMULATION
#include "../simul/pic_interface.cpp"
#else

using namespace std;

#include <iostream>
#include <exception>
#include <usb.h>

#include "usbdevicemoteurs.h"
#include "usbdeviceinout.h"


#define DEVICEMOTEUR 0xf0
#define DEVICEINOUT  0x0f

#define catchandreturn(x) {\
try\
{\
    return (int)x;\
}\
catch(...)\
{\
    return -1;\
}}

#define catcherror(x) {\
try\
{\
    x;\
    return 1;\
}\
catch(...)\
{\
    return -1;\
}}

static USBDeviceMoteur *devicemoteur;
static USBDeviceInOut *deviceinout;

unsigned char find_usb(unsigned char but)
{
    unsigned char resultat;
    struct usb_bus *busses;
    resultat = 0;
    usb_init();
    usb_find_busses();
    usb_find_devices();
    busses = usb_get_busses();
    for (struct usb_bus *bus = busses; bus; bus = bus->next)
    {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next)
        {
            /* Look for matching devices */
            if (dev->descriptor.idVendor  == MACGYVERID)
            {
                if((but & DEVICEMOTEUR) && (dev->descriptor.idProduct == MOTORID))
                {
                    // Carte moteur trouvee !
	            cout << "Carte moteur trouvee a l'emplacement " << bus->dirname << ":" << dev->filename << endl;
                    try 
                    {
                        devicemoteur = new USBDeviceMoteur(dev);
                        resultat |= DEVICEMOTEUR;
                    }
                    catch (char const* msg)
                    {
                        cerr << msg << endl; 
                    }
                }
                else if((but & DEVICEINOUT) && (dev->descriptor.idProduct == INOUTID))
                {
                    // Carte InOut trouvee !
	            cout << "Carte InOut trouvee a l'emplacement " << bus->dirname << ":" << dev->filename << endl;
                    try 
                    {
                        deviceinout = new USBDeviceInOut(dev);
                        resultat |= DEVICEINOUT;
                    }
                    catch (char const* msg)
                    {
                        cerr << msg << endl; 
                    }
                }
                else if(dev->descriptor.idProduct == BOOTLOADERID)
                {
                    cout << "Carte non programmee trouvee a l'emplacement " << bus->dirname << ":" << dev->filename << endl;
                }
            }
        }
    }
    return resultat;
}

int setup_usb_connexions()
{
    unsigned char c;
    int res;
    res = 0;
    c = find_usb(DEVICEMOTEUR | DEVICEINOUT);
    if(c != (DEVICEMOTEUR | DEVICEINOUT))
    {
        //if(!(c & DEVICEMOTEUR)) cout << "Carte moteur absente" << endl;
      //  if(!(c & DEVICEINOUT)) cout << "Carte InOut absente" << endl;
        if(!(c & DEVICEMOTEUR)) res = -1;
        if(!(c & DEVICEINOUT)) res -= 2;
        return res;
    }
    return 1;
}

int repare_usb()
{
    unsigned char b,c;
    int res;
    res = 0;
    c = 0;
    if((devicemoteur == NULL)||(devicemoteur->repare() <= 0)) c = DEVICEMOTEUR;
    if((deviceinout == NULL)||(deviceinout->repare() <= 0)) c |= DEVICEINOUT;
    if(c != 0) 
    {
        b = find_usb(c);
        if(b != c)
        {
            if(!(b & DEVICEMOTEUR)) res = -1;
            if(!(b & DEVICEINOUT)) res -= 2;
            return res;
        //    if(!(b & DEVICEMOTEUR)) cout << "Carte moteur absente" << endl;
        //    if(!(b & DEVICEINOUT)) cout << "Carte InOut absente" << endl;
        //    return -1;
        }
    }
    return 1;
}

void shut_usb(void)#include "../common/simul.h"
{
    if(devicemoteur != NULL) delete devicemoteur;
    if(deviceinout != NULL) delete deviceinout;
    devicemoteur = NULL;
    deviceinout = NULL;
}

int get_codeuses(int *codeuse1, unsigned char *sens1, int *codeuse2, unsigned char *sens2)
{
    catchandreturn(devicemoteur->getcodeuses(codeuse1, sens1, codeuse2, sens2));
}

int set_speed(unsigned char vitesse1, unsigned char vitesse2)
{
    catcherror(devicemoteur->sendorder(vitesse1, vitesse2));
}

int get_digital_in(void)
{
    catchandreturn(deviceinout->get_digital_in());
}

int get_analog_in(unsigned short *result, unsigned char length)
{
    catchandreturn(deviceinout->get_analog_in(result, length));
}

int init_analog_in(unsigned char number)
{
    catchandreturn(deviceinout->init_analog_in(number));
}

int set_DC_motor(unsigned char number, char sens)
{
    unsigned char ordre, msk;
    if(sens > 0) ordre = 0x55;    // 01010101
    else if(sens == 0) ordre = 0; // 00000000
    else ordre = 0xaa;            // 10101010
    msk = 3<<((number-1)<<1);
    ordre = (ordre & msk) | (deviceinout->motors_last_order & (0xff-msk));
    catcherror(deviceinout->motors_order(ordre));
}

int set_servo(unsigned char number, unsigned char position)
{
    catcherror(deviceinout->servo_order(number, position));
}

#endif
