/*-------------------------------------------------------------------------
  application_iface.c - Application interface
-------------------------------------------------------------------------*/

#include "config.h"
#include "common_types.h"
#include "usb_descriptors.h"

typedef struct {
    uchar invalid;                   // != 0 when the application is not valid
    void* device_descriptor;
    void** configuration_descriptor; // pointer to an array of pointer to configuration descriptors
    uchar** string_descriptor;       // pointer to an array of pointer to string descriptors
    void (*** ep_init)(void);
    void (*** ep_in)(void);
    void (*** ep_out)(void);
    void (*** ep_setup)(void);
    void (*main) (void);
    void (*suspend) (void);
    void (*wakeup) (void);
    char (*set_device_remote_wakeup)(void);
    char (*clear_device_remote_wakeup)(void);
} ApplicationData;

const ApplicationData __at(APPLICATION_DATA_ADDRESS) application_data = {
    0xFF,           // Application is not valid by default
    NULL,           // no device descriptor
    NULL,           // no configuration descriptor
    NULL,           // no string descriptor
    NULL,           // ep_init
    NULL,           // ep_in
    NULL,           // ep_out
    NULL,           // ep_setup
    NULL,           // application main
    NULL,           // application suspend procedure
    NULL,           // application wakeup procedure
    NULL,           // application set_device_remote_wakeup procedure
    NULL            // application clear_device_remote_wakeup procedure
};
