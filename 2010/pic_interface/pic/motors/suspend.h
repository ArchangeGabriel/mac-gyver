// suspend.h //



////////////////////////////////////////

#define setalim(); {PINALIM=1;}
#define cutalim(); {PINALIM=0;}
void application_suspend(void);
void application_wakeup(void);
char set_device_remote_wakeup(void);
char clear_device_remote_wakeup(void);
