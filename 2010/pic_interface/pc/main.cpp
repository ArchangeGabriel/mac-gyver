/*-------------------------------------------------------------------------
  main.c - docker main function

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

/* $Id: main.cxx,v 1.6 2006/11/11 14:05:16 gaufille Exp $ */

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
