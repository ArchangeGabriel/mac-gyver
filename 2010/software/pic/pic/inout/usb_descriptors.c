/*-------------------------------------------------------------------------
  usb_descriptors.c - USB device, interface, class, ep, string descriptors
-------------------------------------------------------------------------*/

#include "app_usb_descriptors.h"
#include "boot_iface.h"
#include "ep1.h"
#include "ep2.h"
#include "proto.h"

#define EP0_BUFFER_SIZE 8

const USB_Device_Descriptor device_descriptor =
{    
    sizeof(USB_Device_Descriptor),    // Size of this descriptor in bytes
    DEVICE_DESCRIPTOR,                // Device descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0xff,                   // Class Code
    0x00,                   // Subclass code
    0xff,                   // Protocol code
    EP0_BUFFER_SIZE,        // Max packet size for EP0
    MACGYVERID,             // Vendor ID
    INOUTID,                // Product ID
    0x0100,                 // Device release number in BCD format
    1,                      // Manufacturer string index
    2,                      // Product string index
    3,                      // Device serial number string index
    3                       // Number of possible configurations
};

/* Configurations Descriptor */
const USB_Default_Composite_Descriptor default_cfg = {
    // Configuration Descriptor 
    {sizeof(USB_Configuration_Descriptor),    // Size of this descriptor in bytes
    CONFIGURATION_DESCRIPTOR,                 // CONFIGURATION descriptor type
    sizeof(default_cfg),          // Total length of data for this configuration
    1,                      // Number of interfaces in this configuration
    1,                      // Index value of this configuration
    4,                      // Configuration string index
    DEFAULT | SELF_POWERED, // Attributes
    0},                     // Max power consumption (2X mA)
    
    // Boot Interface Descriptor
    {sizeof(USB_Interface_Descriptor),   // Size of this descriptor in bytes
    INTERFACE_DESCRIPTOR,                // Interface descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    0,                      // Number of endpoints in this interface
    0xff,                   // Class code
    0xff,                   // Subclass code
    0xff,                   // Protocol code
    4}                      // Interface string index
};

const USB_Flash_Composite_Descriptor flash_cfg = {
    // Configuration Descriptor 
    {sizeof(USB_Configuration_Descriptor),    // Size of this descriptor in bytes
    CONFIGURATION_DESCRIPTOR,                 // CONFIGURATION descriptor type
    sizeof(flash_cfg),          // Total length of data for this configuration
    1,                      // Number of interfaces in this configuration
    2,                      // Index value of this configuration
    5,                      // Configuration string index
    DEFAULT | SELF_POWERED, // Attributes
    0},                     // Max power consumption (2X mA)
    
    // Flash Interface Descriptor
    {sizeof(USB_Interface_Descriptor),   // Size of this descriptor in bytes
    INTERFACE_DESCRIPTOR,               // Interface descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this interface
    0xff,                   // Class code
    0xff,                   // Subclass code
    0xff,                   // Protocol code
    5},                     // Interface string index
    
    // Flash Endpoint Descriptors
    {{sizeof(USB_Endpoint_Descriptor),
      ENDPOINT_DESCRIPTOR,
      EP(1) | OUT_EP,
      BULK,
      64,   // 68 bytes max
      0}, // not used for full speed bulk EP
      
    {sizeof(USB_Endpoint_Descriptor),
     ENDPOINT_DESCRIPTOR,
     EP(2) | IN_EP,
     BULK,
     64,   // 68 bytes max
     0}}   // not used for full speed bulk EP
};

const USB_Application_Composite_Descriptor application_cfg = {
    // Configuration Descriptor 
    {sizeof(USB_Configuration_Descriptor),    // Size of this descriptor in bytes
    CONFIGURATION_DESCRIPTOR,                 // CONFIGURATION descriptor type
    sizeof(application_cfg),          // Total length of data for this configuration
    1,                      // Number of interfaces in this configuration
    3,                      // Index value of this configuration
    4,                      // Configuration string index
    DEFAULT | SELF_POWERED, // Attributes
    0},                     // Max power consumption (2X mA)
    
    // Boot Interface Descriptor
    {sizeof(USB_Interface_Descriptor),   // Size of this descriptor in bytes
    INTERFACE_DESCRIPTOR,                // Interface descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this interface
    0xff,                   // Class code
    0xff,                   // Subclass code
    0xff,                   // Protocol code
    4},                     // Interface string index

    
    // My  Endpoint Descriptors
    {{sizeof(USB_Endpoint_Descriptor),
      ENDPOINT_DESCRIPTOR,
      EP(1) | OUT_EP,
      BULK,
      64,   // 68 bytes max
      0}, // not used for full speed bulk EP
      
    {sizeof(USB_Endpoint_Descriptor),
     ENDPOINT_DESCRIPTOR,
     EP(2) | IN_EP,
     BULK,
     64,   // 68 bytes max
     0}}   // not used for full speed bulk EP
};


const uchar code * const configuration_descriptor[] = {
    (const uchar*) &default_cfg,
    (const uchar*) &flash_cfg,
    (const uchar*) &application_cfg
};

/* String descriptors */
/* Language desriptors (Unicode 3.0 (UTF-16) */
const uchar str0[] = {sizeof(str0),  STRING_DESCRIPTOR, 0x09,0x04};// french = 0x040c, english = 0x409

const uchar str1[] = {sizeof(str1),  STRING_DESCRIPTOR,
                                              'V',0x00,
                                              'a',0x00,
                                              's',0x00,
                                              'c',0x00,
                                              'o',0x00,
                                              ' ',0x00,
                                              'P',0x00,
                                              'r',0x00,
                                              'o',0x00,
                                              'j',0x00,
                                              'e',0x00,
                                              'c',0x00,
                                              't',0x00};
                                              
const uchar str2[] = {sizeof(str2),  STRING_DESCRIPTOR,
                                              'T',0x00,
                                              'o',0x00,
                                              'g',0x00,
                                              'g',0x00,
                                              'l',0x00,
                                              'e',0x00,
                                              ' ',0x00,
                                              'm',0x00,
                                              'o',0x00,
                                              'd',0x00,
                                              'u',0x00,
                                              'l',0x00,
                                              'e',0x00};
                                              
const uchar str3[] = {sizeof(str3), STRING_DESCRIPTOR, '1',0x00};

const uchar str4[] = {sizeof(str4),  STRING_DESCRIPTOR,
                                              'D',0x00,
                                              'e',0x00,
                                              'f',0x00,
                                              'a',0x00,
                                              'u',0x00,
                                              'l',0x00,
                                              't',0x00};
                                              
const uchar str5[] = {sizeof(str5),  STRING_DESCRIPTOR,
                                              'F',0x00,
                                              'l',0x00,
                                              'a',0x00,
                                              's',0x00,
                                              'h',0x00};

const uchar * const string_descriptor[] = {str0, str1, str2, str3, str4, str5};

/******************************************************************************
 * USB Endpoints callbacks
 *****************************************************************************/
//#pragma code null_function 0x3184
void null_function() __naked
{
    __asm
        return
    __endasm;
}

static void (* const ep_init_cfg0 [])(void) = {
                                        ep0_init,     // 0
                                        null_function, // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};

static void (* const ep_init_cfg1 [])(void) = {
                                        ep0_init,      // 0
                                        ep1_init,      // 1
                                        ep2_init,      // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15


/*static void (* const ep_init_cfg2 [])(void) = {    // Mon usb comme je le veux !
                                        ep0_init,      // 0
                                        my_ep1_init,   // 1
                                        my_ep2_init,   // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15*/

static void (* const ep_init_cfg2 [])(void) = {    // Mon usb comme je le veux !
                                        ep0_init,      // 0
                                        my_ep1_init,   // 1
                                        my_ep2_init,   // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15


/*
 *  ep_init_cfg0 is duplicated so it is used on cfg 0 (device not configured) 
 * and 1 (device configured) 
 */

void (** const ep_init[])(void) = {
                                     ep_init_cfg0,
                                     ep_init_cfg0,
                                     ep_init_cfg1,
                                     ep_init_cfg2
                                   };
           
static void (* const ep_in_cfg0 [])(void) = {
                                        ep0_in,       // 0
                                        null_function, // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15

static void (* const ep_in_cfg1 [])(void) = {
                                        ep0_in,       // 0
                                        null_function, // 1
                                        ep2_in,        // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15

/*static void (* const ep_in_cfg2 [])(void) = {
                                        ep0_in,       // 0
                                        null_function, // 1
                                        my_ep2_in,        // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15
*/
static void (* const ep_in_cfg2 [])(void) = {
                                        ep0_in,       // 0
                                        null_function, // 1
                                        my_ep2_in,        // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15


void (** const ep_in[])(void) =   {
                                        ep_in_cfg0,
                                        ep_in_cfg0,
                                        ep_in_cfg1,
                                        ep_in_cfg2
                                       };
                                        
static void (* const ep_out_cfg0 [])(void) = {
                                        ep0_init,      // 0
                                        null_function, // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15

static void (* const ep_out_cfg1 [])(void) = {
                                        ep0_init,       // 0
                                        ep1_out,       // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15

/*static void (* const ep_out_cfg2 [])(void) = {
                                        ep0_init,       // 0
                                        my_ep1_out,       // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15*/

static void (* const ep_out_cfg2 [])(void) = {
                                        ep0_init,       // 0
                                        my_ep1_out,       // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15


void (** const ep_out[])(void) =  {
                                        ep_out_cfg0,
                                        ep_out_cfg0,
                                        ep_out_cfg1,
                                        ep_out_cfg2
                                       };
                                        
static void (* const ep_setup_cfg0 [])(void) = {
                                        ep0_setup,     // 0
                                        null_function, // 1
                                        null_function, // 2
                                        null_function, // 3
                                        null_function, // 4
                                        null_function, // 5
                                        null_function, // 6
                                        null_function, // 7
                                        null_function, // 8
                                        null_function, // 9
                                        null_function, // 10
                                        null_function, // 11
                                        null_function, // 12
                                        null_function, // 13
                                        null_function, // 14
                                        null_function};// 15

void (** const ep_setup[])(void) = {
                                         ep_setup_cfg0,
                                         ep_setup_cfg0,
                                         ep_setup_cfg0,
                                         ep_setup_cfg0
                                        };

