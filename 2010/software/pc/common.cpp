#define PC_INCLUDE
#include "common.h"
#include "../common/const.h"

// Couleur du robot
int color;

//------------------------------------------------------------------------------
void set_color(int _color)
{
  color = _color;
}
//------------------------------------------------------------------------------
int get_color()
{
  return color;
}
//------------------------------------------------------------------------------
position_t symetrize(position_t pos)
{
  if(color == clYELLOW)
  {
    pos.x = _LONGUEUR_TER - pos.x;
    pos.a = M_PI - pos.a;
    while(pos.a>M_PI) pos.a -= 2.*M_PI;
  }
  return pos;  
}
//------------------------------------------------------------------------------
void wait_for_it(pthread_mutex_t *mutex)
{   
  pthread_mutex_lock(mutex);    
  pthread_mutex_unlock(mutex);      
  pthread_mutex_destroy(mutex);  
  delete mutex;    
}
//------------------------------------------------------------------------------
