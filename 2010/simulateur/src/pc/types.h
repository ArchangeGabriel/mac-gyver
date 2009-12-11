#ifndef TYPESH
#define TYPESH

#include "../common/lin_alg.h"

typedef struct position_t
{
  float x,y,a;
  
  position_t(){}
  position_t(float x,float y,float a):x(x),y(y),a(a){}
  vector_t v() {return vector_t(x,y);}
} position_t;

typedef struct tension_t
{
  float left,right;
  
  tension_t(float left,float right):left(left),right(right){}
} tension_t;

#endif
