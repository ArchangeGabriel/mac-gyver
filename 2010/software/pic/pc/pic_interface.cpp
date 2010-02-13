// pic_interface.cxx //

using namespace std;

#include <iostream>
#include <exception>
#include <usb.h>

#include "usbdevicemoteurs.h"
#include "usbdeviceinout.h"
#include "proto.h"

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
    cout << "find_usb" << endl;
    resultat = 0;
    usb_init();
    usb_find_busses();
    usb_find_devices();
    busses = usb_get_busses();
    cout << "busses bot" << endl;
    for (struct usb_bus *bus = busses; bus; bus = bus->next)
    {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next)
        {
            cout << "device";
            /* Look for matching devices */
            if (dev->descriptor.idVendor  == MACGYVERID)
            {
                cout << "matching";
                if((but & DEVICEMOTEUR) && (dev->descriptor.idProduct == MOTORID))
                {
                    // Carte moteur trouvee !
	            cout << "Carte moteur trouvee !" << endl;
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
	            cout << "Carte InOut trouvee !" << endl;
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
            }
            cout << endl;
        }
    }
    cout << "fin de find_usb" << (int)resultat << endl;
    return resultat;
}

int setup_usb_connexions()
{
    unsigned char c;
    c = find_usb(DEVICEMOTEUR | DEVICEINOUT);
    if(c == 0)
    {
        if(!(c & DEVICEMOTEUR)) cout << "DEVICEMOTEUR absent" << endl;
        if(!(c & DEVICEINOUT)) cout << "DEVICEINOUT absent" << endl;
        cout << "setup returns -1" << endl;
        return -1;
    }
    cout << "setup returns 1" << endl;
    return 1;
}

int repare_usb()
{
    int a,b;
    unsigned char c;
    a = devicemoteur->repare();
    b = deviceinout->repare();
    c = 0;
    if(a <= 0)
    {
        c = DEVICEMOTEUR;
    }
    if(b <= 0)
    {
        c |= DEVICEINOUT;
    }
    if(c != 0) 
    {
        a = (int)find_usb(c);
        if(a != 0)
        {
            return -1;
        }
    }
    return 1;
}

void shut_usb(void)
{
    delete devicemoteur;
    delete deviceinout;
}

int get_codeuses(int *codeuse1, unsigned char *sens1, int *codeuse2, unsigned char *sens2)
{
    catchandreturn(devicemoteur->getcodeuses(codeuse1, sens1, codeuse2, sens2));
}

int set_speed(unsigned char vitesse1, unsigned char vitesse2)
{
    catcherror(devicemoteur->sendorder(vitesse1, vitesse1));
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
    msk = 3<<(number<<1);
    ordre = (ordre & msk) | (deviceinout->motors_last_order & (0xff-msk));
    catcherror(deviceinout->motors_order(ordre));
}

int set_servo(unsigned char number, unsigned char position)
{
    catcherror(deviceinout->servo_order(number, position));
}
