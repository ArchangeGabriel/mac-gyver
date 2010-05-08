#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PC_INCLUDE
#include "strategie.h"
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
  else if(init_analog_in(0) < 0)
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
        if(iter == -1)
          repare_usb();
        else
          callbackRecvCoder(double(iter)*PIC_FREQ, coder_left, coder_right);        
      }
    
      if(!strat_is_started() && (get_digital_in() & DIGIT_JACK))
        strat_lets_go();
        
      usleep(TIMER_CODER);
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
  
  if(set_speed(int(127.*pwleft)+128, int(127.*pwright)+128) == 1)
    return 0;
  else
    return (repare_usb() == 1) ? 0 : 1;
}
//------------------------------------------------------------------------------
int pic_move_pusher(int position)
{
  return (set_DC_motor(MOTOR_PUSHER, position) == 1) ? 0 : 1;
}
//------------------------------------------------------------------------------
bool pic_where_pusher(int position)
{
  if(position == MOTOR_PUSHER_FORWARD)
    return get_digital_in() & DIGIT_PUSHER_FRONT;
  else if(position == MOTOR_PUSHER_BACKWARD)
    return get_digital_in() & DIGIT_PUSHER_BACK;
  else
    return !(get_digital_in() & (DIGIT_PUSHER_FRONT | DIGIT_PUSHER_BACK));
}
//------------------------------------------------------------------------------
int pic_move_door(int position)
{
  return (set_servo(SERVOM_DOOR, position) == 1) ? 0 : 1;
}
//------------------------------------------------------------------------------
bool pic_where_door(int position)
{
  if(position == MOTOR_DOOR_OPEN)
    return get_digital_in() & DIGIT_DOOR_OPEN;
  else
    return get_digital_in() & DIGIT_DOOR_CLOSED;
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
