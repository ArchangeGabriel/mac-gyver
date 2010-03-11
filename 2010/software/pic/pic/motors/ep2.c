// my_ep2.c /// j'y fais ce que je veux ! Na !
// IN buffer (pic vers pc)


//#include "ep1.h"
#include "ep2.h"
#include "app_usb.h"
#include <pic18fregs.h>

#include <boot_iface.h>

static uchar last_send_was_null;

//#pragma udata usb_buf ep2_InBuffer
volatile uchar __at(0x540) ep2_InBuffer[MY_EP2_BUFFER_SIZE];

uint ep2_num_bytes_to_send;
uchar *ep2_source_data;

//uchar *ep1_OutBuffer;
//extern volatile uchar __at(0x540) ep2_InBuffer[MY_EP2_BUFFER_SIZE];


void my_ep2_init(void)
{
    // Set DAT1 so that the first call to prepare_ep2_in
    // will switch to DAT0, which is correct for the first
    // message
    EP_IN_BD(2).Stat.uc = BDS_UCPU | BDS_DAT1 | BDS_DTSEN;
    UEP2 = EPHSHK_EN | EPINEN_EN | EPCONDIS_EN;       // Init EP2 as an IN EP
}

void my_prepare_ep2_in(void)
{
    last_send_was_null = (ep2_num_bytes_to_send < MY_EP2_BUFFER_SIZE);
    EP_IN_BD(2).Cnt = ep2_num_bytes_to_send;
    EP_IN_BD(2).ADR = (uchar __data *)&ep2_InBuffer;
    fill_in_buffer(2, &ep2_source_data, MY_EP2_BUFFER_SIZE, &ep2_num_bytes_to_send);
    if(EP_IN_BD(2).Stat.DTS == 0)
    {
        EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
    }
    else
    {
        EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    }
}

void my_ep2_in(void)
{
    return;
/*    if(UEIEbits.BTOEE && UEIRbits.BTOEF)
    {
        // Error during last transfert. Redoing it.
        UEIRbits.BTOEF = 0;
        if(EP_IN_BD(2).Stat.DTS == 0)
        {
            EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
        else
        {
            EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
    }
    else
    {
        if((ep2_num_bytes_to_send == 0) && last_send_was_null)
        {
            // There is nothing more to send, so keep
            // the EP2 buffer under the CPU responsability
            return;
        }
        last_send_was_null = (ep2_num_bytes_to_send < MY_EP2_BUFFER_SIZE);
        EP_IN_BD(2).Cnt = ep2_num_bytes_to_send;
        fill_in_buffer(2, &ep2_source_data, MY_EP2_BUFFER_SIZE, &ep2_num_bytes_to_send);
        if(EP_IN_BD(2).Stat.DTS == 0)
        {
            EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
        else
        {
            EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
    }*/
}

