
using namespace std;

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "pic_interface.h"

static void sighandler(int sig) {
    shut_usb();
    cerr << "Caught signal " << sig << ".\n";
    exit(-1);
}

int main(int argc, char**argv)
{    
    int    i;
   
    i = 1;

    /* Catch some signals to properly shut down the hardware */
    signal(SIGHUP, sighandler);
    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGTERM, sighandler);
        
    setup_usb_connexions();
    while(1)  // ad vitam eternam
    {
        i = get_digital_in(); // ici j'appelle mes fonctions de transfert (hihi, great pun)
        if(i == -1)
        {
            if(repare_usb() == -1)
            {
                shut_usb();  // Ca merdouille grave !!
                return -1;
            }
        }
    }
}
