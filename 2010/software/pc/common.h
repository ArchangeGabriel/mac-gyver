#ifndef TYPESH
#define TYPESH

#include <pthread.h>
#include "../common/lin_alg.h"

typedef struct position_t
{
  float x,y,a;
  
  position_t(){}
  position_t(float x,float y,float a):x(x),y(y),a(a){}
  vector_t v() const {return vector_t(x,y);}
} position_t;

typedef struct tension_t
{
  float left,right;
  
  tension_t(float left,float right):left(left),right(right){}
} tension_t;


//------------------------------------------------------------------------------
/*
Règle/Renvoie la couleur
*/
enum{clBLUE,clYELLOW};

void set_color(int _color);
int get_color();

//------------------------------------------------------------------------------
/*
Prend une position pour le robot en haut à gauche et renvoit la position adaptée à la couleur
*/
position_t symetrize(position_t pos);

//------------------------------------------------------------------------------
/*
Attend qu'un mutex se libère et le détruit
*/
void wait_for_it(pthread_mutex_t *mutex);

#endif
