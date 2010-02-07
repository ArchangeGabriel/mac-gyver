/*-------------------------------------------------------------------------
  usb_std_req.h - USB Standard SETUP requests
-------------------------------------------------------------------------*/

#ifndef USB_STD_REQ_H_
#define USB_STD_REQ_H_

#include "config.h"
#include "common_types.h"

/* bmRequestType Definitions */
#define HOST_TO_DEVICE      0
#define DEVICE_TO_HOST      1

#define STANDARD            0
#define CLASS               1
#define VENDOR              2

#define RECIPIENT_DEVICE    0
#define RECIPIENT_INTERFACE 1
#define RECIPIENT_ENDPOINT  2
#define RECIPIENT_OTHER     3

/******************************************************************************
 * wValue Standard Feature Selector
 * USB 2.0 Spec Ref Table 9-6
 *****************************************************************************/
#define ENDPOINT_HALT          0
#define DEVICE_REMOTE_WAKEUP   1
#define TEST_MODE              2

/******************************************************************************
 * bRequest Standard Device Requests
 * USB 2.0 Spec Ref Table 9-4
 *****************************************************************************/
#define GET_STATUS        0
#define CLEAR_FEATURE     1
#define SET_FEATURE       3
#define SET_ADDRESS       5
#define GET_DESCRIPTOR    6
#define SET_DESCRIPTOR    7
#define GET_CONFIGURATION 8
#define SET_CONFIGURATION 9
#define GET_INTERFACE     10
#define SET_INTERFACE     11
#define SYNCH_FRAME       12


typedef union
{
    /* Buffer as a char array */
    struct
    {
        uchar uc[EP0_BUFFER_SIZE];
    };
    
    /* Standard Device Requests */
    struct
    {
        uchar bmRequestType;
        uchar bRequest;    
        uint  wValue;
        uint  wIndex;
        uint  wLength;
    };
    struct
    {
        unsigned recipient:5;               // Device, Interface, Endpoint, Other
        unsigned request_type:2;            // Standard, Class, Vendor
        unsigned data_transfer_direction:1; // Host-to-device, Device-to-host
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    
    // Get Descriptor
    struct
    {
        unsigned :8;
        unsigned :8;
        uchar bDescIndex;                // For Configuration and String DSC Only
        uchar bDescType;                 // Device, Configuration, String, 
        uint wLanguageID;                // Language ID
        unsigned :8;
        unsigned :8;
    };
    
    // Set Address
    struct
    {
        unsigned :8;
        unsigned :8;
        uchar bAddress;                   // Device Address (0 to 127)
        unsigned :8;                      // Should be Zero (behavior unspecified if not)
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    
    // Set Configuration
    struct
    {
        unsigned :8;
        unsigned :8;
        uchar bConfigurationValue;       // Configuration Value
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    
    // Set Interface
    struct
    {
        unsigned :8;
        unsigned :8;
        uchar bLSBAlternateSetting;      // Alternate Setting LSB
        uchar bMSBAlternateSetting;      // Alternate Setting MSB
        uchar bLSBInterface;             // Interface Number LSB
        uchar bMSBInterface;             // Interface Number MSB
        unsigned :8;
        unsigned :8;
    };
    
} StandardRequest;



#endif /*USB_STD_REQ_H_*/
