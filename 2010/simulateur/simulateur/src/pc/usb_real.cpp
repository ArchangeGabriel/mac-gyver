#include <stdio.h>
#include <usb.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "../common/comm.h"
#include "usb.h"

usb_dev_handle* did[2] = {NULL, NULL};

struct usb_device* dev[2];

bool connected[2]={false, false};

/* In millisecond. Note that with libusb 0.11 on Linux, a timeout of 0 results
 * in a immediate return... */
#define USB_TIMEOUT 2000  /* 15 minutes */

#define EP_OUT 1
#define EP_IN 2

//------------------------------------------------------------------------------
void connect_init()
{
  struct usb_bus *busses; //info bus sub

  fprintf(stderr,"Get busses...\n"); fflush(stdout);
  usb_init();
  fprintf(stderr,"Init usb...Done.\n"); fflush(stdout);
  usb_find_busses();
  fprintf(stderr,"Find busses...Done.\n"); fflush(stdout);
  usb_find_devices();
  fprintf(stderr,"Find devices...Done.\n"); fflush(stdout);
  busses = usb_get_busses();
  fprintf(stderr,"Get busses...Done.\n"); fflush(stdout);  

  for (struct usb_bus *bus = busses; bus; bus = bus->next)
  {
    struct usb_device *_dev;

    for (_dev = bus->devices; _dev; _dev = _dev->next)
    {
      /* Look for matching devices */
      if (_dev->descriptor.idVendor  == 0xa5a5) // une de nos cartes
      { 
        if (_dev->descriptor.idProduct == 0x0002) // carte Divers
        {
          try 
          {
            fprintf(stderr,"Divers trouvee\n");
            dev[1] = _dev;
          }
          catch (char const* msg)
          {
              printf("%s\n",msg);
          }
        }
        if (_dev->descriptor.idProduct == 0x0003) // carte Moteur
        {
          try 
          {
            fprintf(stderr,"Moteur trouvee\n");
            dev[0] = _dev;
          }
          catch (char const* msg)
          {
            printf("%s\n",msg);
          }
        }
      }
    }
  } 
  
  if(!WANT_PIC1 || !dev[0])
    fprintf(stderr,"Carte moteur absente.\n");

  if(!WANT_PIC2 || !dev[1])
    fprintf(stderr,"Carte divers absente.\n");

  if((WANT_PIC1 && !dev[0]) || (WANT_PIC2 && !dev[1]))
    exit(0);
}
//------------------------------------------------------------------------------
int connect_usb(int picID)
{
  int c;
  
  did[picID] = usb_open(dev[picID]);
  if(!did[picID])
  {
    fprintf(stderr,"Unable to open device.\n");
    return -1;
  }

  c = usb_set_configuration(did[picID],3);
  if(c)
  {
    close_connection(picID);
    fprintf(stderr,"Impossible to change configuration.\n");
  }
    
  c = usb_claim_interface(did[picID], 0);
  if(c)
  {
    close_connection(picID);
    fprintf(stderr,"Device interface 0 unavailable.\n");
  }
  
  connected[picID]=true;  
  return 0;
}
//------------------------------------------------------------------------------
bool is_connected(int picID)
{
  return connected[picID];
}
//------------------------------------------------------------------------------
char* read_usb(int picID)
{
  if(!connected[picID]) return NULL;
  
  char buff[100];
  int read_len;
  int len;
  int len_type;
  int len_header;
  int len_data;
  
  while((read_len=usb_bulk_read(did[picID], EP_IN, buff, 100, USB_TIMEOUT))<=0)
  {
    if(read_len<0) 
    {
      printf("Erreur read!!!!\n");
      fflush(stderr);
      return NULL;
    }
  }

  len_type = sizeof(int);
  len_header=get_msg_header_size(buff);
  len_data=get_msg_data_size(buff);
  len=len_type+len_header+len_data;

  if(len!=read_len)
  {
    printf("pic%d: packet size mismatch (packet type: %d, size %d instead of %d)\n", picID+1, get_msg_type(buff), read_len, len);
    fflush(stdout);
  }

  char *msg=new char[len]; 
  memcpy(msg,buff,len);

//  struct timeval t;
//  gettimeofday(&t,NULL);   
  //printf("Type: %d, %d %d %d\n",get_msg_type(buff),len_type, len_header, len_data);
  return msg;    
/*  len_type = sizeof(int);  
  if(usb_bulk_read(did[picID], EP_IN, buff, len_type, USB_TIMEOUT)<0)
  {
    printf("nul1\n"); fflush(stdout);
    return NULL; 
  }
  printf("<%f> read(%d): %d\n",t.tv_sec+t.tv_usec*0.000001,picID,*((int*)buff)); fflush(stdout);
  printf("len_type = %d\n",len_type);
  len_header=get_msg_header_size(buff);
  if(len_header>0 && usb_bulk_read(did[picID], EP_IN, &buff[len_type], len_header, USB_TIMEOUT)<0)
  {
    printf("nul2\n"); fflush(stdout);
    return NULL; 
  }
  len_data=get_msg_data_size(buff);
  len=len_type+len_header+len_data;  
  char *msg=new char[len]; 
  memcpy(msg,buff,len_type+len_header);   
  printf("len_header = %d\n",len_header);  
  if(len_data>0 &&  usb_bulk_read(did[picID], EP_IN, &msg[len_type+len_header], len_data, USB_TIMEOUT))
  {
    printf("nul3 (%d)(len_head=%d)(msg[8]=%d)\n",picID, len_header, ((int*)msg)[2]); fflush(stdout);
    return NULL; 
  }
  for(int i=0; i<len; i++)
    printf("%d ",msg[i]);
  printf("\n");
  return msg; */
}
//------------------------------------------------------------------------------
int write_usb(int picID,void *msg,int size)
{
  if(!connected[picID]) return -1;

//  printf("write PIC%d size = %d\n",picID+1, size);
//  fflush(stdout);
  //for(int i=0; i<size; i++)
    //printf(" %d\n",((unsigned char*)msg)[i]); 
  if(usb_bulk_write(did[picID], EP_OUT, (char*)msg, size, USB_TIMEOUT)<0)
    return -1;  
  return 0;
}
//------------------------------------------------------------------------------
void close_connection(int picID)
{
  fprintf(stderr,"Trying to close connection to PIC %d (0 ou 1)\n",picID);
  connected[picID]=false;  
}
//------------------------------------------------------------------------------
