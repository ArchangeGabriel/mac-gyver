/*-------------------------------------------------------------------------
  usb.h - Main USB functions
-------------------------------------------------------------------------*/

#ifndef USB_H_
#define USB_H_

#include "common_types.h"
#include "usb_descriptors.h"

/* USB device states */
#define DETACHED_STATE              0
#define ATTACHED_STATE              1
#define POWERED_STATE               2
#define DEFAULT_STATE               3
#define ADDRESS_PENDING_STATE       4
#define ADDRESS_STATE               5
#define CONFIGURATION_PENDING_STATE 6
#define CONFIGURED_STATE            7

/* Device state accessors */
#define SET_DEVICE_STATE(x)         { usb_device_state = x; }
#define GET_DEVICE_STATE()          (usb_device_state)

/* Current configuration accessors */
#define SET_ACTIVE_CONFIGURATION(x) { usb_active_cfg = x; }
#define GET_ACTIVE_CONFIGURATION()  (usb_active_cfg)

/* Default configurations */
#define DEFAULT_CONFIGURATION  0
#define BOOT_CONFIGURATION     1
#define FLASH_CONFIGURATION    2

/* Current interface accessors */ // Vraiment n'importe quoi ca !!
#define SET_ACTIVE_ALTERNATE_SETTING(x)     { usb_active_alt_setting = x; }
#define GET_ACTIVE_ALTERNATE_SETTING()      (usb_active_alt_setting)

/* USB PID: Token Types - See chapter 8 in the USB specification */
#define SETUP_TOKEN         0x0d //0b00001101
#define OUT_TOKEN           0x01 //0b00000001
#define IN_TOKEN            0x09 //0b00001001

/* USB direction */
#define OUT                 0
#define IN                  1

/* UEPn Initialization Parameters */
// Handshake should be disable for isoch
#define EPHSHK_EN           0x10 // Enable handshake packet
#define EPCONDIS_EN         0x08
#define EPOUTEN_EN          0x04
#define EPINEN_EN           0x02
#define EPSTALL_EN          0x01

                                    
/* Buffer Descriptor Status Register Initialization Parameters */
#define BDS_BSTALL          0x04 //Buffer Stall enable
#define BDS_DTSEN           0x08 //Data Toggle Synch enable
#define BDS_INCDIS          0x10 //Address increment disable
#define BDS_KEN             0x20 //SIE keeps buff descriptors enable

#define BDS_DAT0            0x00 //DATA0 packet expected next
#define BDS_DAT1            0x40 //DATA1 packet expected next

#define BDS_USIE            0x80 //SIE owns buffer
#define BDS_UCPU            0x00 //CPU owns buffer

/* Buffer Descriptor Status Register */
typedef union
{
    uchar uc;
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned BSTALL:1; // Buffer Stall Enable
        unsigned DTSEN:1;  // Data Toggle Synch Enable
        unsigned INCDIS:1; // Address Increment Disable
        unsigned KEN:1;    // BD Keep Enable
        unsigned DTS:1;    // Data Toggle Synch Value
        unsigned UOWN:1;   // USB Ownership
    };
    struct{
        unsigned :2;
        unsigned PID0:1;   // Packet Identifier, bit 0
        unsigned PID1:1;   // Packet Identifier, bit 1
        unsigned PID2:1;   // Packet Identifier, bit 2
        unsigned PID3:1;   // Packet Identifier, bit 3
        unsigned :2;
    };
    struct{
        unsigned :2;
        unsigned PID:4;    // Packet Identifier
        unsigned :2;
    };
} BDStat;

/* Buffer Descriptor Table */
typedef union 
{
    struct
    {
        BDStat Stat;       // Buffer Descriptor Status Register
        uchar Cnt;         // Number of bytes to send/sent/(that can be )received
        uchar ADRL;        // Buffer Address Low
        uchar ADRH;        // Buffer Address High
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        __data uchar *ADR; // Buffer Address
    };
} BufferDescriptorTable;

/* Out buffer descriptor of endpoint ep */
/* BEWARE : it works only without ping pong buffers */
#define EP_OUT_BD(ep) (ep_bdt[ep << 1])

/* In buffer descriptor of endpoint ep */
/* BEWARE : it works only without ping pong buffers */
#define EP_IN_BD(ep)  (ep_bdt[(ep << 1) + 1])

/* Buffer descriptors Table */
extern volatile BufferDescriptorTable __at (0x400) ep_bdt[32];

//extern volatile uchar __at(0x500) ep1_OutBuffer[EP1_BUFFER_SIZE];
//extern volatile uchar __at(0x540) ep2_InBuffer[EP2_BUFFER_SIZE];

extern uchar __at(0x005f) usb_device_state;
extern uchar __at(0x005e) usb_active_cfg;
extern uchar __at(0x005d) usb_active_alt_setting;

extern uchar * const UEPpoint[];
#define UEP *UEPpoint

extern const USB_Device_Descriptor *device_descriptor;
extern const uchar code * code * configuration_descriptor;
extern const uchar* const *string_descriptor;
extern void (*** ep_init)(void);
extern void (*** ep_in)(void);
extern void (*** ep_out)(void);
extern void (*** ep_setup)(void);
extern void (*suspend)(void);
extern void (*wakeup)(void);
extern char (*set_device_remote_wakeup)(void);
extern char (*clear_device_remote_wakeup)(void);
extern uchar (*get_device_status)(void);

void init_usb(void);
void reset_usb(void);
void usb_sleep(void);
void dispatch_usb_event(void);
void fill_in_buffer(uchar EPnum,
                    uchar **source,
                    uint buffer_size, 
                    uint *nb_byte);

#endif /*USB_H_*/
