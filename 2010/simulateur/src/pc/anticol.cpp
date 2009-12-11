#include <unistd.h>
#include <stdio.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "picAPI.h"
#include "anticol.h"
#include "path_planning.h"
#include "cinematik.h"
#include "strategie.h"

#define TIMER_DIST    100     // en millisecondes: timer pour regarder la valeur des capteurs

#define SLOWER_DIST   0.25    // en millisecondes
#define STOP_DIST     0.20    // en millisecondes

#define C0X   0.135 
#define C0Y   -0.12 
#define C1X   0.135 
#define C1Y   0.12
#define C2X   -0.135
#define C2Y   0.12
#define C3X   -0.135
#define C3Y   -0.12

#define INCERT   0.05   // incertitude sur la distance

#define MAX_ITER  5


float CX[4] = {C0X-_ROUE_X,C1X-_ROUE_X,C2X-_ROUE_X,C3X-_ROUE_X};
float CY[4] = {C0Y,C1Y,C2Y,C3Y};
float dist_captor[4];
int iter_captor[4] = {0, 0, 0, 0};
int status_captor[4];     // 0 - ok
                          // 1 - slower
                          // 2 - danger
                          // 3 - distrib

bool check_distrib(vector_t pos); // renvoie vrai si le capteur a vu un distributeur

//------------------------------------------------------------------------------
void anticolMainLoop()
{
  fprintf(stderr,"AC thread ok...\n");  fflush(stdout);
  while(true)
  {
    if(picDistCaptors() == 0)
      usleep(TIMER_DIST * 1000);
    else
      usleep(100000);
  }  
}
//------------------------------------------------------------------------------
float captor_get_dist(int i)
{
  return dist_captor[i];
}
//------------------------------------------------------------------------------
int captor_get_status(int i)
{
  return status_captor[i];
}
//------------------------------------------------------------------------------
// Renvoie la position de l'obstacle devant le ième capteur
vector_t captor_get_position(int i)
{
  position_t pos_robot = cine_get_position();
  vector_t pos_captor = vector_t(CX[i],CY[i]).rotate(pos_robot.a); 
  vector_t N = vector_t(cos(pos_robot.a),sin(pos_robot.a));
  if(i == 2 || i == 3) N = -N;
  pos_captor += pos_robot.v() + (N * dist_captor[i]);
  
  return pos_captor;
}
//------------------------------------------------------------------------------
void anticolOnRecvCaptors(int n, float *values)
{
  if(n != 4)
  {
    fprintf(stderr,"<anticol.cpp> Nombre de capteurs incorrect (%d).\n",n);
    fflush(stdout);
    return;
  }
}
//------------------------------------------------------------------------------

