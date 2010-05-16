#ifndef CAPTORH
#define CAPTORH
#include "../common/lin_alg.h"
#include "sdl.h"
#include "parse_params.h"
#include "object.h"

class captor_t : public params_t
{
  public:
  vector_t pos;                  // position par rapport au centre du robot
  object_t *robot;               // robot de rattachement
  simul_info_t *simul_info;      // pointeur vers le simulateur 
   
  virtual void init_params();
  virtual double measure();
  virtual void draw();  
};



class color_captor_t : public captor_t
{
  public:

  double measure();
  void draw();
};

class dist_captor_t : public captor_t
{
  double z;
  vector_t dir; 

  public:

  void init_params();
  double measure();
  void draw();
};

#endif
