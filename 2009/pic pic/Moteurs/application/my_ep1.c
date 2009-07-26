// my_ep1.c /// j'y fais ce que je veux ! Na !
// OUT buffer (pc vers pic)


#include "my_ep1.h"
#include "my_ep2.h"
#include "my_usb.h"
#include <pic18fregs.h>
#include "common_types.h"
#include <boot_iface.h>
#include <math.h>

#include "codeuses.h"

#include "toto.h"

volatile uchar __at(0x500) ep1_OutBuffer[MY_EP1_BUFFER_SIZE];

uint ep2_num_bytes_to_send;
uchar *ep2_source_data;

unsigned char mydata[8];

unsigned char checksum;

void my_ep1_init(void)
{
    EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
    EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
    EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
}

void my_ep1_out(void)
{
    if(EP_OUT_BD(1).Cnt >= 1)
    {
	if(ep1_OutBuffer[0]) CCPR1L = ep1_OutBuffer[0];
	if(ep1_OutBuffer[1]) CCPR2L = ep1_OutBuffer[1];

	ep2_num_bytes_to_send = 8;
	ep2_source_data = (unsigned char*) codeusecnt;

        my_prepare_ep2_in(); // Envoi

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
