#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PC_INCLUDE
#include "picAPI.h"
#include "picAPI/pic_interface.h"

// Fonction de callack
void (*callbackOnJack)(void) = NULL;
void (*callbackRecvCoder)(double,int,int) = NULL;
void (*callbackRecvReset)(void) = NULL;

//------------------------------------------------------------------------------
void pic_MainLoop()
{    
  // Initialize connections  
  fprintf(stderr,"Init USB...                 ");  
  fflush(stdout);
  if(setup_usb_connexions() < 0)
  {
    fprintf(stderr,"SETUP UBS FAILED !\n");
    exit(1);
  }
  else if(init_analog_in(NB_ANALOGS) < 0)
  {
    fprintf(stderr,"INIT ANALOG FAILED !\n");
    exit(1);
  }
  else
  {
    fprintf(stderr,"ok\n");  
    fflush(stdout);

    while(true)
    {
      if(callbackRecvCoder)
      {
        int coder_left, coder_right;
        unsigned char dir_left, dir_right;
        int iter = get_codeuses(&coder_left, &dir_left, &coder_right, &dir_right);
        callbackRecvCoder(double(iter)*double(TIMER_CODER)/1000., coder_left, coder_right);        
      }
      usleep(TIMER_CODER*1000);
    }
  }
}
//------------------------------------------------------------------------------
void pic_Jack()
{
  if(callbackOnJack)
    callbackOnJack();
}
//------------------------------------------------------------------------------
int pic_MotorsPower(double pwleft, double pwright)
{
  if(pwleft>1.) pwleft = 1.;
  if(pwleft<-1.) pwleft = -1.;  
  if(pwright>1.) pwright = 1.;
  if(pwright<-1.) pwright = -1.;
  
  return (set_speed(int(127.*pwleft)+128, int(127.*pwright)+128) == 1) ? 0 : 1;
}
//------------------------------------------------------------------------------
void pic_Reset()
{
  if(callbackRecvReset)
    callbackRecvReset();
}
//------------------------------------------------------------------------------
void pic_OnRecvJack(void (*fun)(void))
{
  callbackOnJack = fun;
}
//------------------------------------------------------------------------------
void pic_OnRecvCoder(void (*fun)(double, int, int))
{
  callbackRecvCoder = fun;
}
//------------------------------------------------------------------------------
void pic_RecvReset(void (*fun)(void))
{
  callbackRecvReset = fun;
}
//------------------------------------------------------------------------------
