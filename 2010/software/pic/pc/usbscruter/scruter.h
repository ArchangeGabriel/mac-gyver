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


#define SETUP        1
#define ACK          0

struct usbmon_line{
  unsigned long int timestamp;
  char event_type;
  char transfer_type;
  char transfer_direction; 
  char bus;
  char device;
  char endpoint;
  short urb_status;
  unsigned char interval;
  char* data;
};
