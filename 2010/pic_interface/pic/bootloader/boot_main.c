/*-------------------------------------------------------------------------
  boot_main.c - Pic boot main function

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------*/

/* $Id: boot_main.c,v 1.6 2006/04/21 20:47:41 gaufille Exp $ */

/* Reserve 255 bytes of stack at 0x200 */
#pragma stack 0x200 255

#include <pic18fregs.h>
#include "common_types.h"
#include "usb.h"
#include "usb_descriptors.h"
#include "application_iface.h"
#include "alim.h"
#include "pindebug.h"

#define NOT_FORCE_BL PORTBbits.RB7

void init_boot(void)
{
    static ulong count;

    ADCON1 = 0x0F;
    CMCON  = 0x07;
    TRISA  = 0xEF; //IIIOIIII
    TRISB  = 0xFF; //IIIIIIII
    TRISC  = 0xFF; //IIIIIIII
    TRISD  = 0xFF; //IIIIIIII
    TRISE  = 0xFF; //IIIIIIII

    PINDEBUG = 1;
    
    count = 0x80000;
    while(count)
    {
        count--;
    }
    
    PINDEBUG = 0;

    
    // The FORCE_BL pin is used to force the bootloader only mode
    if((application_data.invalid == 0) && NOT_FORCE_BL)
    { 
        // use application descriptors
        device_descriptor        = application_data.device_descriptor;
        configuration_descriptor = application_data.configuration_descriptor;
        string_descriptor        = application_data.string_descriptor;
        suspend                  = application_data.suspend;
        wakeup                   = application_data.wakeup;
    }
    else
    { 
        // use boot descriptors
        device_descriptor        = &boot_device_descriptor;
        configuration_descriptor = boot_configuration_descriptor;
        string_descriptor        = boot_string_descriptor;
        suspend                  = boot_suspend;
        wakeup                   = boot_wakeup;
    }

    // By default always use the booloader vectors
    // because the device is started in configuration 0
    // vectors will be switched in ep0.c when the configuration
    // will become greater than 2
    ep_init  = boot_ep_init;
    ep_in    = boot_ep_in;
    ep_out   = boot_ep_out;
    ep_setup = boot_ep_setup;

}

void main(void)
{
    init_boot();
    init_usb();

    while(1)
    {
        usb_sleep();
        dispatch_usb_event();
        if((application_data.invalid == 0) &&
           (GET_ACTIVE_CONFIGURATION() > FLASH_CONFIGURATION))
        {
            application_data.main();

            INTCON = 0; // Forbid interrupts
        }
    }
}

