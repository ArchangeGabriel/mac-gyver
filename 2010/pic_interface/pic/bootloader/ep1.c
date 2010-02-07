/*-------------------------------------------------------------------------
  ep1.c - USB endpoint 1 callbacks
-------------------------------------------------------------------------*/

#include "ep1.h"
#include "ep2.h"
#include "usb.h"
#include "flash.h"
#include <pic18fregs.h>
#include <delay.h>


//#pragma udata usb_buf ep1_OutBuffer
volatile uchar __at(0x500) ep1_OutBuffer[EP1_BUFFER_SIZE];

void ep1_init(void)
{
    EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
    EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
    EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
}

void ep1_out(void)
{
    if(EP_OUT_BD(1).Cnt >= 1)
    {
        switch(ep1_OutBuffer[0])
        {
            case ERASE_FLASH_CMD:
                TBLPTRL = ep1_OutBuffer[1];
                TBLPTRH = ep1_OutBuffer[2];
                TBLPTRU = ep1_OutBuffer[3];
                erase_block();
                break;
            case WRITE_FLASH_CMD:
                TBLPTRL = ep1_OutBuffer[1];
                TBLPTRH = ep1_OutBuffer[2];
                TBLPTRU = ep1_OutBuffer[3];
                write_block((__data uchar *)&(ep1_OutBuffer[4]));
                break;
            case READ_FLASH_CMD:
                ep2_num_bytes_to_send = EP2_BUFFER_SIZE;
                // TODO: Use a Pointer union to set that
                ep2_source_data = (uchar __code *) (ep1_OutBuffer[1] + ep1_OutBuffer[2] * 256 + ep1_OutBuffer[3] * 65536);
                prepare_ep2_in();
                break;
            case VALID_APPLICATION_CMD:
                break;
            case RESET_CMD:
                delay10ktcy(10);
                Reset();
                break;
            case SECTION_DESCRIPTOR_CMD:
                ep2_num_bytes_to_send = sizeof(section_descriptor);
                ep2_source_data = (uchar __code *) section_descriptor;
                prepare_ep2_in();
                break;
                
            default:
                // Raise an error
                EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
                EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
                EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
                break;
        }
        EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
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
        EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
        EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
        EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
    }
}

/*void getbuffersaddresses(uchar **ep1out, uchar **ep2in)
{
    *ep1out = (uchar __data *)&ep1_OutBuffer;
    *ep2in = (uchar __data *)&ep2_InBuffer;
}*/
