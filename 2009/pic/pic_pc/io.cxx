

#include "my_usbdevice.h"
#include "comm.h"
#include "commandes.h"

static USBDeviceDivers* devicedivers;
static USBDeviceMoteur* devicemoteur;

void init_io()
{
    struct usb_bus *busses; //info bus sub

    usb_init();
    usb_find_busses();
    usb_find_devices();

    for (struct usb_bus *bus = busses; bus; bus = bus->next)
    {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next)
        {
            /* Look for matching devices */
            if (dev->descriptor.idVendor  == 0xa5a5) // une de nos cartes
	    { 
                if (dev->descriptor.idProduct == 0x0002) // carte Divers
                {
                    try 
                    {
			                cout << "Divers trouvee" << endl;
                      devicedivers = new USBDeviceDivers(dev);
                    }
                    catch (char const* msg)
                    {
                        cerr << msg << endl; 
                    }
                }
                if (dev->descriptor.idProduct == 0x0003) // carte Moteur
                {
                    try 
                    {
			cout << "Moteur trouvee" << endl;
                        devicemoteur = new USBDeviceMoteur(dev);
                    }
                    catch (char const* msg)
                    {
                        cerr << msg << endl; 
                    }
                }
            }
        }
    }
}

void close_io()
{
    delete devicedivers;
    delete devicemoteur;
}

//int get_input  (int in_type,int in_index);
//int set_output(int out_type,int out_index,int out_value);

int get_input(int in_type, int in_index) // Sauf pour l'argument CODER, une reponse negative est une erreur
{
    switch(in_type)
    {
	case CODER:
		devicemoteur->get_codeuses();
	        if(in_index) return devicemoteur->codeuse_droite;
		else return devicemoteur->codeuse_gauche;
		break;
	case VPOS:
		devicedivers->set_moteurs(0x00, 0x00); // on envoi rien pour recuperer la reponse
                return !(devicedivers->etat.moteur2 & ACTIV_MOTEUR_2);
		break;
	case HPOS:
		devicedivers->set_moteurs(0x00, 0x00); // rien envoyer
                return !(devicedivers->etat.moteur1 & ACTIV_MOTEUR_1);
		break;
	case CLAMP:
		//.....
		break;
	case JACK:
		devicedivers->get_digitals();
		return devicedivers->etat.numerique & IN_JACK;
	case DIST:
		devicedivers->get_analogs();
		return devicedivers->etat.analogique[in_index];
	default: return -2;
    }
}


int set_output(int out_type,int out_index,int out_value) // retourne -2 sur erreur d'arguments, 
	                                                          // -1 sur probleme de communication,
								  //  1 sinon
{
    switch (out_type)
    {
	case VPOS:
		switch (out_value)
		{
		    case posClampBack:
			    return devicedivers->set_moteurs(0x00, ACTIV_MOTEUR_2 | 0x00); // Va en position 0
		    case posClampMiddle:
			    return devicedivers->set_moteurs(0x00, ACTIV_MOTEUR_2 | 0x01); // Va en position 1
		    case posClampFront:
			    return devicedivers->set_moteurs(0x00, ACTIV_MOTEUR_2 | 0x02); // Va en position 2
		    default:
			    return -2;
		}
	case HPOS:
		return devicedivers->set_moteurs(ACTIV_MOTEUR_1 | ((unsigned char)out_value & POSITION_MOTEUR_1), 0x00);
	case CLAMP:
		switch(out_value)
		{
		    case stClampClosed:
			    return devicedivers->set_servo(PINCE_FERMEE);
		    case stClampOpen:
			    return devicedivers->set_servo(PINCE_OUVERTE);
		    default:
			    return -2;
		}
	case MOTORS:
		if(out_index) return devicemoteur->sendorder(0x00, ((unsigned char)out_value) + 128);
		else return devicemoteur->sendorder(((unsigned char)out_value) + 128, 0x00);
	case BELT:
		if (out_value>0) return devicedivers->set_moteurs(0x00, ACTIV_MOTEUR_3 | MOT3_SORTANT); // fait sortir les palets
		else if (out_value==0) return devicedivers->set_moteurs(0x00, ACTIV_MOTEUR_3 | MOT3_ARRET); //Arrete le tapis
		else return devicedivers->set_moteurs(0x00, ACTIV_MOTEUR_3 | MOT3_ENTRANT); //fait rentrer les palets
	default:
		return -2;
    }
}
