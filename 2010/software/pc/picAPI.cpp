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

bool pic_ready = false;

// Etat du robot
int digital_output = 0;
double power_left = 0;
double power_right = 0;
bool move_pusher = false;
int where_pusher; 
bool move_door = false;
int where_door;

//------------------------------------------------------------------------------
void pic_MainLoop()
{    
  // Initialize connections  
  fprintf(stderr,"Init USB...                 ");  
  fflush(stdout);
  if(init_analog_in(0) < 0)
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
      pic_ready = true;
        
      while(true)
      {
        digital_output = get_digital_in();
        
        // Jack
        if(!strat_is_started() && (digital_output & DIGIT_JACK))
          strat_lets_go();

        // Coders
        if(callbackRecvCoder)
        {
          int coder_left, coder_right;
          unsigned char dir_left, dir_right;
          int iter = get_codeuses(&coder_left, &dir_left, &coder_right, &dir_right);
          if(iter == -1)
            break;
          else
            callbackRecvCoder(double(iter)*PIC_FREQ, coder_left, coder_right);        
        }
        
        // Motors
        if(set_speed(int(127.*power_left)+128, int(127.*power_right)+128) != 1)
          break;
 
        // Pusher
        if(move_pusher)
        {
          bool ok = false;
          if(set_DC_motor(MOTOR_PUSHER_LEFT, where_pusher) == 1 && set_DC_motor(MOTOR_PUSHER_RIGHT, -where_pusher) == 1)
            ok = true;
          if(ok)
            move_pusher = false;
          else
            break;
        }
               
        // Door
        if(move_door)
        {
          if(set_servo(SERVOM_DOOR, where_door) == 1)
            move_door = false;
          else
            break;
        }
          
        usleep(TIMER_CODER);
      }
      
      pic_ready = false;
      while(repare_usb() != 1) {}
    }
  }
}
//------------------------------------------------------------------------------
bool pic_is_ready()
{
  return pic_ready;
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
  
  power_left = pwleft;
  power_right = pwright;
  return 0;
}
//------------------------------------------------------------------------------
int pic_move_pusher(int position)
{
  where_pusher = position;
  move_pusher = true;
  return 0;
}
//------------------------------------------------------------------------------
bool pic_where_pusher(int position)
{
  if(position == MOTOR_PUSHER_FORWARD)
    return !(digital_output & DIGIT_PUSHER_FRONT);
  else if(position == MOTOR_PUSHER_BACKWARD)
    return !(digital_output & DIGIT_PUSHER_BACK);
  else
    throw(0);
}
//------------------------------------------------------------------------------
int pic_move_door(int position)
{
  where_door = position;
  move_door = true;
  return 0;
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
