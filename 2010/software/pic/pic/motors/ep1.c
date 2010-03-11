// my_ep1.c /// j'y fais ce que je veux ! Na !
// OUT buffer (pc vers pic)


//#include "ep1.h"
#include "ep2.h"
#include "app_usb.h"
#include <pic18fregs.h>
#include "common_types.h"
#include <boot_iface.h>
//#include <math.h>

#include "proto.h"

volatile uchar __at(0x500) ep1_OutBuffer[MY_EP1_BUFFER_SIZE];

void my_ep1_init(void)
{
    EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
    EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
    EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
}

void my_ep1_out(void)
{
    unsigned char alive[1];
    if(EP_OUT_BD(1).Cnt >= 1)
    {
        if(ep1_OutBuffer[0] == SETPWM)
        {
            if(ep1_OutBuffer[1]) CCPR2L = ep1_OutBuffer[1];
            if(ep1_OutBuffer[2]) CCPR1L = ep1_OutBuffer[2];
        }
        else if(ep1_OutBuffer[0] == ISDEAD)
        {
            while(EP_IN_BD(2).Stat.uc & BDS_USIE); // wait the Uown bit to be cleared
            ep2_num_bytes_to_send = 1;
            ep2_source_data = alive;
            alive[0] = ISDEAD;
            my_prepare_ep2_in();
        }

        EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
        if(EP_OUT_BD(1).Stat.DTS == 0)
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
        else
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
    }
    else // Raise an error
    {
        EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
        EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
        EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
    }
}
