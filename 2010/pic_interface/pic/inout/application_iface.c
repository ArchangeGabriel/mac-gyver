/*-------------------------------------------------------------------------
  application_iface.c - Application data
-------------------------------------------------------------------------*/

#include "common_types.h"
#include "app_usb_descriptors.h"
#include "application_iface.h"

void application_main(void);

/*typedef struct {
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
} ApplicationData;*/

const ApplicationData __at(0x2000) application_data = {
    0x00,                          // Application is valid
    &device_descriptor,            // device descriptor
    configuration_descriptor,      // configuration descriptor
    string_descriptor,             // string descriptor
    ep_init,                       // ep_init
    ep_in,                         // ep_in
    ep_out,                        // ep_out
    ep_setup,                      // ep_setup
    application_main,              // application main
    NULL,           // application suspend procedure
    NULL,           // application wakeup procedure
    NULL,           // application set_device_remote_wakeup procedure
    NULL            // application clear_device_remote_wakeup procedure
};
