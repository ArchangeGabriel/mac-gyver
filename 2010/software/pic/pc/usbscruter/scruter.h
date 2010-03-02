// scruter.h //


#define IN          'i'
#define OUT         'o'

#define CONTROL     'C'
#define BULK        'B'
#define INTERRUPT   'I'
//#define ISOCHRONOUS 'C'

#define SUBMISSION  'S'
#define CALLBACK    'C'
#define ERROR       'E'

struct usbmon_line{
  unsigned long int timestamp;
  char event_type;
  char transfer_type;
  char transfer_direction; 
  int bus;
  int device;
  int endpoint;
  char* data;
};
