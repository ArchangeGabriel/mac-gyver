#include <unistd.h>
#include <stdio.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "picAPI.h"
#include "anticol.h"
#include "path_tracker.h"
#include "cinematik.h"
#include "strategie.h"

vector_t captor_pos[4] = {
  vector_t(13.5, 14.5),   // front left
  vector_t(13.5, -14.5),  // front right
  vector_t(-13.5, -14.5), // back right     
  vector_t(-13.5, 14.5)}; // back left
    

//------------------------------------------------------------------------------
void* ac_MainLoop(void*)
{
  // Attend que le robot soit prêt
  while(!strat_is_ready()) usleep(10000);
  
  while(true)
  {
    pic_get_captors_values();
    usleep(500000);
  }  
}
//------------------------------------------------------------------------------
vector_t get_captor_pos(vector_t capt_pos, position_t robot_pos, double dist)
{
  vector_t pos;
  const double c = cos(robot_pos.a);
  const double s = sin(robot_pos.a);  
  
  pos.x = robot_pos.x + capt_pos.x * c - capt_pos.y * s + dist * c;
  pos.y = robot_pos.y + capt_pos.x * s + capt_pos.y * c + dist * s;
  return pos;  
}
//------------------------------------------------------------------------------
void ac_OnRecvCaptors(int n, double *values)
{
  if(n != 4)
  {
    fprintf(stderr,"<anticol.cpp> Nombre de capteurs incorrect (%d).\n",n);
    fflush(stdout);
    return;
  }
  
  bool obstacle = false;
  double coeff;
  position_t robot_pos = cine_get_position();

  for(int i=0; i<2/*4*/; i++)
  {
    //printf("%f ", values[i]);
    if(i == ANALO_DIST_FRONT_LEFT || ANALO_DIST_FRONT_RIGHT)
      coeff = 1;
    else
      coeff = -1;
    vector_t pos = get_captor_pos(captor_pos[i], robot_pos, values[i]*coeff);
    
    if((values[i] < 0.10) && (values[i] > 0.01))
    {
      if(!(pos.x >= 0.6 && pos.x <= 2.4 && pos.y <= 0.6) &&   // colline
         !(pos.y >= 2. && (pos.x <= 0.6 || pos.x >= 2.4)))    // goals
      pt_stop();
      obstacle = true;
    }        
  }
  //printf("\n");
  
  if(!obstacle && pt_is_stopped())
    pt_resume();
}
//--------------  ----------------------------------------------------------------

