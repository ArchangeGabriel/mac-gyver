/*-------------------------------------------------------------------------
  application.h - Application entry points
-------------------------------------------------------------------------*/

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "config.h"
#include "common_types.h"
//#include "usb_descriptors.h"

typedef struct {
    uchar invalid;                   // != 0 when the application is not valid
    void* device_descriptor;
    uchar code * code * configuration_descriptor; // pointer to an array of pointer to configuration descriptors
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

extern const ApplicationData __at(APPLICATION_DATA_ADDRESS) application_data;

#endif /*APPLICATION_H_*/
