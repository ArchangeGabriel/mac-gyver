#include "../common/comm.h"

#include "captor.h"

//----------------------------------------------------------------------------
void captor_t::init_params()
{
  printf("  --> Loading captor's params...");
  fflush(stdout); 
  pos.x = double_param_value("position_x");
  pos.y = double_param_value("position_y");  
  printf("ok\n");
  fflush(stdout);  
}
//----------------------------------------------------------------------------
int captor_t::measure()
{
  return 0;
}
//----------------------------------------------------------------------------
void captor_t::draw()
{
}
//----------------------------------------------------------------------------






//----------------------------------------------------------------------------
int color_captor_t::measure()
{
  vector_t p=pos.rotate_spec(robot->cosinus,robot->sinus)-robot->G_rot+robot->position;
  
  if(0.<=p.y && p.y<0.25)
  {
    for(int i=0;i<4;i++)
      if(0.9+i*0.4-0.0075<p.x && p.x<0.9+i*0.4+0.0075)
        return 1;
    return 0;     
  }
  else if(1.75<p.y && p.y<2.)
  {
    for(int i=0;i<3;i++)
      for(int j=0;j<4;j++)
        if(0.705+i*0.6+j*0.13-0.0075<p.x && p.x<0.705+i*0.6+j*0.13+0.0075)
          return 1;
    return 0;   
  }
  else if(2.<=p.y && p.y<=2.1)
  {
    if(0.6<p.x && p.x<1.2)
      return 1;
    else if(1.8<p.x && p.x<2.4)
      return 1;
    else
      return 0;
  }
  else
    return 0;
}
//----------------------------------------------------------------------------
void color_captor_t::draw()
{
  vector_t p=pos.rotate_spec(robot->cosinus,robot->sinus)-robot->G_rot+robot->position;
  
  DisqueSDL(((int)(robot->simul_info->scale*p.x)),
            ((int)(robot->simul_info->scale*p.y)),
            1,measure()==0?makeColorSDL(0,0,0):makeColorSDL(255,255,255));
}
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
void dist_captor_t::init_params()
{
  printf("  --> Loading captor's params...");
  fflush(stdout); 
  pos.x = double_param_value("position_x");
  pos.y = double_param_value("position_y");  
  z     = double_param_value("position_z");    

  dir.x = double_param_value("direction_x");
  dir.y = double_param_value("direction_y");  
  dir = dir / dir.norme();
  pos = pos - robot->G_init;
  printf("ok\n");
  fflush(stdout);  
}
//----------------------------------------------------------------------------
int dist_captor_t::measure()
{
  float dist = MAX_DIST_CAPT;
  float tmp;
  vector<object_t*> *obj = &robot->simul_info->objets;
  vector_t P,D;
  
  for(unsigned int i=0; i<obj->size(); i++)
    if((*obj)[i] != robot)
    {
      P = robot->position + pos.rotate(robot->angle);
      D = dir.rotate(robot->angle);
      tmp = (*obj)[i]->distance_to(P, D, z);
      if(tmp != -1. && tmp<dist)
        dist = tmp;  
    }
    
  return ((int)(logf(dist / COEFF_A_DIST) / logf(COEFF_B_DIST))) << 6;
} 
//----------------------------------------------------------------------------
void dist_captor_t::draw()
{
}
//----------------------------------------------------------------------------
