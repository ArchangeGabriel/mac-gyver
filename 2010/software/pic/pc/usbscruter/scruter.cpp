// main.cpp //

using namespace std;

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <errno.h>
#include <usb.h>
#include <time.h>

#include "proto.h"
#include "scruter.h"


FILE *input;
FILE *output;
int motorbus,motordevice,inoutbus,inoutdevice;

int codeuse[2],sens[2];
unsigned int nbtrame;
unsigned long int lasttrametime;
struct usbmon_line line;

static void sighandler(int sig)
{
  cerr << endl << "Caught signal " << sig << ".\n";
  fclose(input);
  fclose(output);
  exit(-1);
}

void init()
{
  struct usb_bus *busses;
  int i;
  motorbus = 6;
  motordevice = 13;
  inoutbus = 0;
  inoutdevice = 0;
  nbtrame = 0;
  lasttrametime = 0;
  usb_init();
  usb_find_busses();
  usb_find_devices();
  busses = usb_get_busses();
  for (struct usb_bus *bus = busses; bus; bus = bus->next)
  {
    struct usb_device *dev;

    for (dev = bus->devices; dev; dev = dev->next)
    {
      /* Look for matching devices */
      if (dev->descriptor.idVendor  == MACGYVERID)
      {
        if(dev->descriptor.idProduct == MOTORID)
        {
          // Carte moteur trouvee !
          cout << "Carte moteur trouvee a l'emplacement " << bus->dirname << ":" << dev->filename << endl;
          motorbus = atoi(bus->dirname);
          motordevice = atoi(dev->filename);
        }
        else if(dev->descriptor.idProduct == INOUTID)
        {
          // Carte InOut trouvee !
          cout << "Carte InOut trouvee a l'emplacement " << bus->dirname << ":" << dev->filename << endl;
          inoutbus = atoi(bus->dirname);
          inoutdevice = atoi(dev->filename);
        }
        else if(dev->descriptor.idProduct == BOOTLOADERID)
        {
          cout << "Carte non programmee trouvee a l'emplacement " << bus->dirname << ":" << dev->filename << endl;
        }
      }
    }
  }
  input = fopen("./test.mon","r");//fopen("/sys/kernel/debug/usb/usbmon/0u","r");
  output = fopen("./raw.mon","w");
  printf("Initialization finished\n");
}

void parse_ctrl_setup(struct usbmon_line *line)
{
  if(line->data[0]=='s')
  {
    struct usb_ctrl_setup setup;
    sscanf(&line->data[2],"%x %x %x %x %x",&setup.bRequestType,&setup.bRequest,&setup.wValue,&setup.wIndex,&setup.wLength);
    switch(setup.bRequest)
    {
      case USB_REQ_SET_CONFIGURATION :
        printf("SET_CONFIGURATION(%d)\n",setup.wValue);
        break;
      case USB_REQ_CLEAR_FEATURE :
        printf("CLEAR_FEATURE:");
        switch(setup.wValue)
        {
          case 0: //ENDPOINT_HALT :
            printf("ENDPOINT_HALT(EP%d)\n",setup.wIndex);
            break;
          default:
            printf(" %s\n",line->data);
            break;
        }
        break;
      case USB_REQ_GET_DESCRIPTOR :
        printf("GET_DESCRIPTOR: %s\n",line->data);
        break;
      default:
        printf("Unknown setup: %s\n",line->data);
        break;
    }
  }
  else if(line->data[0]=='0') printf("ACK and answers: %s\n",&line->data[2]);
  else printf("Unknown Error: %s\n",line->data);
}

void treatlinemotor(struct usbmon_line * line)
{
  char * content;
  int length,databegin;
  int i;
  char datatag, datacontent[10];
  content = strchr(line->data,' ');
  sscanf(content," %d %c%n",&length,&datatag,&databegin);
  if(datatag=='=')
  {
    if(length > 10) length = 10;
    content = &content[databegin];
    for(i=0;i<length;i++)
    {
      sscanf(content,"%2x%n",&datacontent[i],&databegin);
      content = &content[databegin];
    }
    if((length==10)&&((datacontent[0]&MSKCODER)==CODERS))
    {
      memcpy(codeuse,&datacontent[2],8);
      sens[0] = MSKSENS0 & datacontent[0];
      sens[1] = (MSKSENS1 & datacontent[0])>>1;
      lasttrametime = line->timestamp;
      nbtrame++;
      printf("\r");
    }
    else printf("Unknown data: %s\n",line->data);
  }
  else if(line->event_type==SUBMISSION) printf("\r");
  else if(line->event_type==ERROR) printf(" Transmission Error: %s\n",line->data); 
  else printf("Unknown data: %s\n",line->data);
}

void treatlineinout(struct usbmon_line * line)
{
  char * content;
  int length,databegin;
  int i;
  char datatag, datacontent[18];
  content = strchr(line->data,' ');
  sscanf(content," %d %c%n",&length,&datatag,&databegin);
  if(datatag=='=')
  {
    if(length > 18) length = 18;
    content = &content[databegin];
    for(i=0;i<length;i++)
    {
      sscanf(content,"%2x%n",&datacontent[i],&databegin);
      content = &content[databegin];
    }
    if(length >= 1)
    {
      switch(datacontent[0])
      {
        case MOTORS: 
          printf("set_motors(%x)\n",datacontent[1]);
          break;
        case ANALOG:
          if(line->transfer_direction=='o') printf("ask_for_analog(%d)\n",datacontent[1]);
          else if((length >= 2) && (length==(int)(2*datacontent[1] + 2)))
          {
            printf("get_analog(%d:",datacontent[1]);
            for(i=0;i<datacontent[1];i++)
            {
              printf(" %d",*((unsigned short *)&datacontent[2+2*i]));
            }
            printf(")\n");
          }
          else printf("Unknown data: %s\n",line->data);
          break;
        case INITAN:
          if(length >=2) printf("init_analog(%d)\n",datacontent[1]);
          else printf("Unknown data: %s\n",line->data);
          break;
        case SERVOS:
          if(length==3) printf("set_servo(%d:%3d)\n",datacontent[1],datacontent[2]);
          else printf("Unknown data: %s\n",line->data);
          break;
        case DIGITS:
          if(line->transfer_direction=='o') printf("ask_for_digital\n");
          else if(length == 2) printf("get_digital(%x)\n",datacontent[1]);
          else printf("Unknown data: %s\n",line->data);
          break;
        default:
          printf("Unknown data: %s\n",line->data);
          break;
      }
    }
    else printf("Unknown data: %s\n",line->data);
  }
}

void print_forhead(struct usbmon_line *line)
{
  printf("%3u,%06us ",line->timestamp/CLOCKS_PER_SEC,line->timestamp%CLOCKS_PER_SEC);
  if(line->transfer_direction==IN) printf("<--");
  else if(line->transfer_direction==OUT) printf("-->");
  else printf("***");
  printf(" %c EP%d:%c ",line->event_type,line->endpoint,line->transfer_type); 
}

int main(int argc, char**argv)
{
  int firstline,databegin;
  unsigned long int timereference;
  char stringline[128];

  /* Catch some signals to properly shut down the hardware */
  signal(SIGHUP, sighandler);
  signal(SIGINT, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGPIPE, sighandler);
  signal(SIGTERM, sighandler);

  firstline = 1;
  init();
  if(input == NULL)
  {
    printf("Unable to find /sys/kernel/debug/usb/usbmon/0u.\n");
    printf("Run 'mount -t debugfs none_debugs /sys/kernel/debug' and 'modprobe usbmon'.\n");
    return -1;
  }
  if(output == NULL) printf("Unable to open output file.\n");

  while(1)
  {
    if((fscanf(input,"%127[^\n]\n",stringline) == EOF)||(strlen(stringline)==0))
    {
      printf("codeuses %3d,%06ds <-- %10d : %d   \\\\ %10d : %d  (trame : %8x)\r",lasttrametime/CLOCKS_PER_SEC,lasttrametime%CLOCKS_PER_SEC,codeuse[0],sens[0],codeuse[1],sens[1],nbtrame);
    }
    else
    {
      fprintf(output,"%s\n",stringline);
      fflush(output);
      sscanf(&stringline[16],"%d %c %c%c:%d:%d:%d %n",&line.timestamp,&line.event_type,&line.transfer_type,&line.transfer_direction,&line.bus,&line.device,&line.endpoint,&databegin);
      if(firstline)
      {
        firstline = 0;
        timereference = line.timestamp;
        line.timestamp = 0;
      }
      else
      {
        line.timestamp -= timereference;
      }
      line.data = &stringline[databegin+16];
      if((line.bus == motorbus)&&(line.device == motordevice)) // concerne la carte moteur
      {
        print_forhead(&line);
        if(line.transfer_type==CONTROL) parse_ctrl_setup(&line);
        else treatlinemotor(&line);
      }
      else if((line.bus == inoutbus)&&(line.device == inoutdevice)) // concerne la carte inout
      {
        print_forhead(&line);
        if(line.transfer_type==CONTROL) parse_ctrl_setup(&line);
        else treatlineinout(&line);
      }
      else // ne nous concerne pas
      {
        char mychar;
        while(((mychar=fgetc(input)) != EOF)&&(mychar != '\n')); // go to the next line or OEF
      }
    }
    fflush(stdout);
  }
  fclose(input);
  fclose(output);
}
