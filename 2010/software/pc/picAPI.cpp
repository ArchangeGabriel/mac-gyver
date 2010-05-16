#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define PC_INCLUDE
#include "strategie.h"
#include "../common/simul.h"
#include "picAPI.h"
#include "picAPI/pic_interface.h"

// Fonction de callack
void (*callbackOnJack)(void) = NULL;
void (*callbackRecvCoder)(double,int,int) = NULL;
void (*callbackRecvCaptors)(int,double*) = NULL;
void (*callbackRecvReset)(void) = NULL;

int exit_value = 0;
bool stop_loop = false;
bool pic_ready = false;

// Etat du robot
int digital_output = 0;
double power_left = 0;
double power_right = 0;
bool change_power = 0;
bool move_pusher = false;
int where_pusher; 
bool move_door = false;
int where_door;
unsigned short captors[4];

//------------------------------------------------------------------------------
void pic_MainLoop()
{     
  // Initialize connections  
  fprintf(stderr,"Init USB...                 ");  
  fflush(stdout);
  if(init_analog_in(NB_ANALOGS) < 0)
  {
    fprintf(stderr,"INIT ANALOG FAILED !\n");
    pic_exit(1);
    return;
  }
  else
  {
    fprintf(stderr,"ok\n");  
    fflush(stdout);
    
    struct timeval time;
    double curr_time;
    double init_time = time.tv_sec + time.tv_usec*0.000001;
    
    gettimeofday(&time,NULL);
    
    while(!stop_loop)
    {
      pic_ready = true;
        
      while(!stop_loop)
      {
        digital_output = get_digital_in();
        
        if(digital_output < 0)
        {
          fprintf(stderr,"Erreur get_digital_in\n");
          break;
        }

        // Jack
        if(!strat_is_started() && (digital_output & DIGIT_JACK))
          strat_lets_go();

        // Coders
        int coder_left, coder_right;
        unsigned char dir_left, dir_right;
        int iter = get_codeuses(&coder_left, &dir_left, &coder_right, &dir_right);
        if(iter == -1)
        {
          fprintf(stderr,"Erreur get_codeuses\n");
          break;
        }
        else
        {
          gettimeofday(&time,NULL);
          curr_time = time.tv_sec + time.tv_usec*0.000001;        
          callbackRecvCoder(curr_time-init_time, -coder_left, coder_right);
        }

        // Motors
        if(change_power)
        {
          change_power = 0;
          if(set_speed(int(127.*power_left)+128, int(127.*power_right)+128) != 1)
          {
            fprintf(stderr,"Erreur set_speed\n");
            break;
          }
        }
 
        // Pusher
        if(move_pusher)
        {
          if(set_DC_motor(MOTOR_PUSHER_LEFT, where_pusher) == 1)
          {
            if(set_DC_motor(MOTOR_PUSHER_RIGHT, where_pusher) == 1)
              move_pusher = false;
            else
            {
              set_DC_motor(MOTOR_PUSHER_LEFT, MOTOR_PUSHER_STOP);
              break;
            }
          }
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
        
        // Distance captors
        get_analog_in(captors, 4);        
        
        #ifdef SIMULATION  
        usleep(TIMER_CODER);
        #endif
      }
      
      if(!stop_loop)
      {
        pic_ready = false;
        while(repare_usb() != 1) {}
      }
    }
  }
}
//------------------------------------------------------------------------------
bool pic_is_ready()
{
  return pic_ready;
}
//------------------------------------------------------------------------------
void pic_exit(int value)
{
  exit_value = value;
  stop_loop = true;
}
//------------------------------------------------------------------------------
int pic_get_exit_value()
{
  return exit_value;
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
  change_power = true;
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
void pic_get_captors_values()
{
  double captors_val[4];
  for(int i=0; i<4; i++)
    captors_val[i] = 1030.0952 * pow((double)captors[i], -0.95);  

  callbackRecvCaptors(4, captors_val);
}
//------------------------------------------------------------------------------
void pic_OnRecvJack(void (*fun)(void))
{
  callbackOnJack = fun;
}
//------------------------------------------------------------------------------
void pic_OnRecvCaptors(void (*fun)(int, double*))
{
  callbackRecvCaptors = fun;
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
