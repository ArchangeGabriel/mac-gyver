/*-------------------------------------------------------------------------
  ep0.c - USB endpoint 0 callbacks
-------------------------------------------------------------------------*/

#include "ep0.h"
#include "application_iface.h"
#include "usb_descriptors.h"
#include "usb_std_req.h"
#include "usb.h"
#include "usb_status.h"
#include <pic18fregs.h>

/* Control Transfer States */
#define WAIT_SETUP          0
#define WAIT_IN             1
#define WAIT_OUT            2

#pragma udata usb_buf SetupBuffer
volatile far StandardRequest SetupBuffer;

#pragma udata usb_buf InBuffer
volatile far uchar InBuffer[EP0_BUFFER_SIZE];

static uchar ep0_state;
static uint  num_bytes_to_be_send;
static uchar *sourceData;
static uchar coming_cfg;

static uchar status[2];

uchar ep0_usb_std_request(void)
{   
    // hack to avoid register allocation bug in sdcc
    static uchar unknown_request; 

    unknown_request = FALSE;

    if(SetupBuffer.request_type != STANDARD) 
    {
        return FALSE;
    }

    switch(SetupBuffer.bRequest)
    {
        case SET_FEATURE:
            switch(SetupBuffer.wValue) // par moi
            {
                case ENDPOINT_HALT:
                    if((SetupBuffer.recipient != RECIPIENT_ENDPOINT)                               // bad recipient
                       || ((GET_DEVICE_STATE() < CONFIGURED_STATE) && (SetupBuffer.wIndex != 0))   // in address state but EP != 0
                       || ((GET_DEVICE_STATE() == CONFIGURED_STATE)                                // Configured...
                         && (                                                                      // but...
                           (!(UEP[SetupBuffer.wIndex] & (EPINEN_EN | EPOUTEN_EN)))                 // EP does not exist
                           || (UEP[SetupBuffer.wIndex] & EPSTALL_EN)                               // EP is already stalled
                            )
                          )
                      )
                    {
                        unknown_request = TRUE; // Invalid request
                        break;
                    }
                    // request is valid !
                    UEP[SetupBuffer.wIndex] |= EPSTALL_EN; // Stall EP
                    break;
                case DEVICE_REMOTE_WAKEUP:
                    unknown_request = (SetupBuffer.recipient != RECIPIENT_DEVICE) || set_device_remote_wakeup();
                    break;
                default: // case TEST_MODE should only be available on high-speed capable devices. This device is not.
                    unknown_request = TRUE;
                    break;
            }
            break;
        case CLEAR_FEATURE:
            switch(SetupBuffer.wValue) // par moi
            {
                case ENDPOINT_HALT:
                    if((SetupBuffer.recipient != RECIPIENT_ENDPOINT)                               // bad recipient
                       || ((GET_DEVICE_STATE() < CONFIGURED_STATE) && (SetupBuffer.wIndex != 0))   // in address state but EP != 0
                       || ((GET_DEVICE_STATE() == CONFIGURED_STATE)                                // Configured...
                         && (                                                                      // but...
                           (!(UEP[SetupBuffer.wIndex] & (EPINEN_EN | EPOUTEN_EN)))                 // EP does not exist
                           || (!(UEP[SetupBuffer.wIndex] & EPSTALL_EN))                            // EP is not stalled
                            )
                          )
                      )
                    {
                        unknown_request = TRUE; // Invalid request
                        break;
                    }
                    // request is valid !
                    ep_init[GET_ACTIVE_CONFIGURATION()][SetupBuffer.wIndex](); // Reset EP
                    break;
                case DEVICE_REMOTE_WAKEUP:
                    unknown_request = (SetupBuffer.recipient != RECIPIENT_DEVICE) || clear_device_remote_wakeup();
                    break;
                default: // case TEST_MODE is not needed (see above).
                    unknown_request = TRUE;
                    break;
            }
            break;
        case GET_CONFIGURATION:
            sourceData = &GET_ACTIVE_CONFIGURATION();
            num_bytes_to_be_send = 1;
            break;
        case GET_DESCRIPTOR:
            switch(SetupBuffer.bDescType)
            {
             
                case DEVICE_DESCRIPTOR:
                    sourceData = (uchar *) device_descriptor;
                    num_bytes_to_be_send = device_descriptor->bLength;
                    break;
                case CONFIGURATION_DESCRIPTOR:
                    sourceData = configuration_descriptor[SetupBuffer.bDescIndex];
                    num_bytes_to_be_send = ((USB_Configuration_Descriptor*)sourceData)->wTotalLength;
                    break;
                case STRING_DESCRIPTOR:
                    sourceData = string_descriptor[SetupBuffer.bDescIndex];
                    num_bytes_to_be_send = sourceData[0];
                    break;
                default:
                    // This is required to stall the DEVICE_QUALIFIER request
                    unknown_request = TRUE;
                    break;
            }
            break;
        case GET_INTERFACE:
            // TODO not implemented
            break;
        case GET_STATUS:
            sourceData = status;
            num_bytes_to_be_send = 2;
            status[1] = 0;
            switch(SetupBuffer.recipient)
            {
                case RECIPIENT_DEVICE:
                    status[0] = GET_DEVICE_STATUS();
                    break;
                case RECIPIENT_INTERFACE:
     /*               if(((GET_DEVICE_STATE() < CONFIGURED_STATE) && (SetupBuffer.wIndex != 0))
                      || ((GET_DEVICE_STATE() == CONFIGURED_STATE)
                        && (SetupBuffer.wIndex >= ((USB_Configuration_Descriptor**)configuration_descriptor)[GET_ACTIVE_CONFIGURATION()]->bNumInterfaces))) // interface does not exist*/
                    if(SetupBuffer.wIndex !=0) // Only for single interfaced devices
                    {
                        unknown_request = TRUE;
                    }
                    else
                    {
                        status[0] = 0;
                    }
                    break;
                case RECIPIENT_ENDPOINT:
                    if(((GET_DEVICE_STATE() < CONFIGURED_STATE) && (SetupBuffer.wIndex != 0))
                      || ((GET_DEVICE_STATE() == CONFIGURED_STATE)
                        && (!(UEP[SetupBuffer.wIndex] & (EPINEN_EN | EPOUTEN_EN)))))                 // EP does not exist
                    {
                        unknown_request = TRUE;
                    }
                    else
                    {
                        status[0] = UEP[SetupBuffer.wIndex] & EPSTALL_EN;
                    }
                    break;
                default:
                    unknown_request = TRUE;
                    break;
            }
            break;
        case SET_ADDRESS:
            SET_DEVICE_STATE(ADDRESS_PENDING_STATE);
            break;
        case SET_CONFIGURATION:
            // is this configuration valid ?
            if(device_descriptor->bNumConfigurations >= SetupBuffer.bConfigurationValue)
            {
                coming_cfg = SetupBuffer.bConfigurationValue;
                SET_DEVICE_STATE(CONFIGURATION_PENDING_STATE);
            }
            else // invalid configuration
            {
                // Reply with a request error (STALL)
                unknown_request = TRUE;
            }
            
            break;
        case SET_INTERFACE:
            // TODO not implemented
            break;
//        case SYNCH_FRAME:
// only for isochronous synchronization
//            break;
        default:
            unknown_request = TRUE;
            break;
    }
    return !unknown_request;
}


void ep0_init(void)
{
    ep0_state = WAIT_SETUP;
    EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
    EP_OUT_BD(0).ADR = (uchar __data *)&SetupBuffer;
    EP_OUT_BD(0).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    EP_IN_BD(0).Stat.uc = BDS_UCPU;
    UEP0 = EPINEN_EN | EPOUTEN_EN | EPHSHK_EN;
}

void ep0_in(void)
{
    if(GET_DEVICE_STATE() == ADDRESS_PENDING_STATE)
    {                                
        UADDR = SetupBuffer.bAddress;
        if(UADDR != 0)
        {
            SET_DEVICE_STATE(ADDRESS_STATE);
        }
        else
        {
            SET_DEVICE_STATE(DEFAULT_STATE);
        }
    }
        
    if(ep0_state == WAIT_IN)
    {
        fill_in_buffer(0, &sourceData, EP0_BUFFER_SIZE, &num_bytes_to_be_send);
        
        if(EP_IN_BD(0).Stat.DTS == 0)
        {
            EP_IN_BD(0).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
        else
        {
            EP_IN_BD(0).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
    }
    else
    {
        ep0_init();
    }

   if(GET_DEVICE_STATE() == CONFIGURATION_PENDING_STATE)
    {

        // First, disable all endpoints.
        // UEP0 is never disabled
        UEP1  = 0; UEP2  = 0; UEP3  = 0; UEP4  = 0;
        UEP5  = 0; UEP6  = 0; UEP7  = 0; UEP8  = 0;
        UEP9  = 0; UEP10 = 0; UEP11 = 0; UEP12 = 0;
        UEP13 = 0; UEP14 = 0; UEP15 = 0;
        
        // switch the functions vectors
        if(coming_cfg <= FLASH_CONFIGURATION)
        {
            // switch back to the bootloader vectors
            ep_init  = boot_ep_init;
            ep_in    = boot_ep_in;
            ep_out   = boot_ep_out;
            ep_setup = boot_ep_setup;
        }
        else
        {
            // switch to the application vectors
            ep_init  = application_data.ep_init;
            ep_in    = application_data.ep_in;
            ep_out   = application_data.ep_out;
            ep_setup = application_data.ep_setup;
        }

        SET_ACTIVE_CONFIGURATION(coming_cfg);

        if(coming_cfg == 0)
        {
            SET_DEVICE_STATE(ADDRESS_STATE);
        }
        else
        {
            static uchar i;

            // Switch to decrement loop because of a sdcc bug
            for(i = 15; i > 0; i--)
//            for(i = 1; i < 16; i++)
            {
                ep_init[coming_cfg][i]();
            }
            
            SET_DEVICE_STATE(CONFIGURED_STATE);
        }
    }
}

void ep0_setup(void)
{

    ep0_state = WAIT_SETUP;
    num_bytes_to_be_send = 0;
    
    if(ep0_usb_std_request())
    {
        UCONbits.PKTDIS = 0;
        if(SetupBuffer.data_transfer_direction == DEVICE_TO_HOST)
        {
            ep0_state = WAIT_IN;
    
            EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
            EP_OUT_BD(0).ADR = (uchar __data *)&SetupBuffer;            
            EP_OUT_BD(0).Stat.uc = BDS_USIE;
    
            EP_IN_BD(0).ADR = (uchar __data *)InBuffer;
            if(SetupBuffer.wLength < num_bytes_to_be_send)
                {
                    num_bytes_to_be_send = SetupBuffer.wLength;
                }
            fill_in_buffer(0, &sourceData, EP0_BUFFER_SIZE, &num_bytes_to_be_send);
            EP_IN_BD(0).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
            
        }
        else // HOST_TO_DEVICE
        {
            ep0_state = WAIT_OUT;
    
            EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
            EP_OUT_BD(0).ADR = (uchar __data *)InBuffer;
            EP_OUT_BD(0).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;

            EP_IN_BD(0).Cnt = 0;
            EP_IN_BD(0).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
    }
    else
    {
        UCONbits.PKTDIS = 0;
        EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
        EP_OUT_BD(0).ADR = (uchar __data *)&SetupBuffer;
        EP_OUT_BD(0).Stat.uc = BDS_USIE | BDS_BSTALL;

        EP_IN_BD(0).Stat.uc  = BDS_USIE | BDS_BSTALL;
    }
}
