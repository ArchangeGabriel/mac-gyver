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
// Renvoie de l'obstacle devant le ième capteur
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
  
  float tmp = values[1];
  values[1] = values[2];
  values[2] = tmp;

  for(int i=0; i<4; i++)
  {
    dist_captor[i] = values[i];
//    printf("%f ",values[i]); fflush(stdout);
  }
//  printf("\n");
 
 
  position_t pos = cine_get_position();
  vector_t N = vector_t(cos(pos.a),sin(pos.a));
  
  int id_capt;
  float angle_add;
  
  // La gauche
  if(cine_get_speed_wheel(0)>0.)  // on avance
  {
    id_capt = 0;
    angle_add = 0.;
  }
  else                            // on recule
  {
    id_capt = 3;
    angle_add = M_PI;
  }  
  
  pp_lock_situ();
  
  if(values[id_capt] < STOP_DIST)
  {
    vector_t pos_capt = captor_get_position(id_capt);
    if(!check_distrib(pos_capt))
    {
      iter_captor[id_capt]++;
      if(iter_captor[id_capt] >= MAX_ITER)
      {
        status_captor[id_capt] = 1;
        pp_stop(id_capt);
      }
    }
    else
    {
      iter_captor[id_capt]=0;
      status_captor[id_capt] = 3;    
    }
  } else if(values[id_capt] < SLOWER_DIST)
  {
    vector_t pos_capt = captor_get_position(id_capt);
    if(!check_distrib(pos_capt))
    {
      iter_captor[id_capt]++;
      if(iter_captor[id_capt] >= MAX_ITER)
      {    
        status_captor[id_capt] = 2;
        pp_slow_down();
      }
    }
    else
    {
      iter_captor[id_capt]=0;    
      status_captor[id_capt] = 3;
    }
  }
  else
  {
    iter_captor[id_capt]=0; 
    status_captor[id_capt] = 0;    
    pp_no_danger();
  }
  status_captor[id_capt==0?3:0] = 0;    
  iter_captor[id_capt==0?3:0] = 0;     

  // La droite
  if(cine_get_speed_wheel(1)>0.)  // on avance
  {
    id_capt = 1;
    angle_add = 0.;
  }
  else                            // on recule
  {
    id_capt = 2;
    angle_add = M_PI;
  }  
  
  if(values[id_capt] < STOP_DIST)
  {
    vector_t pos_capt = captor_get_position(id_capt);
    if(!check_distrib(pos_capt))
    {
      iter_captor[id_capt]++;
      if(iter_captor[id_capt] >= MAX_ITER)
      {
        status_captor[id_capt] = 1;
        pp_stop(id_capt);
      }
    }
    else
    {
      iter_captor[id_capt]=0;
      status_captor[id_capt] = 3;    
    }
  } else if(values[id_capt] < SLOWER_DIST)
  {
    vector_t pos_capt = captor_get_position(id_capt);
    if(!check_distrib(pos_capt))
    {
      iter_captor[id_capt]++;
      if(iter_captor[id_capt] >= MAX_ITER)
      {    
        status_captor[id_capt] = 2;
        pp_slow_down();
      }
    }
    else
    {
      iter_captor[id_capt]=0;    
      status_captor[id_capt] = 3;
    }
  }
  else
  {
    iter_captor[id_capt]=0; 
    status_captor[id_capt] = 0;    
    pp_no_danger();
  }  
  status_captor[id_capt==1?2:1] = 0; 
  iter_captor[id_capt==1?2:1] = 0;       
  pp_unlock_situ(); 
}
//------------------------------------------------------------------------------
bool check_distrib(vector_t pos)
{
  int cfg = strat_config_distrib();
  
  // distributeurs
  if(cfg == 0 || cfg == -1)
  {
    vector_t d = vector_t((pos.x<_LONGUEUR_TER/2.)?0.04:_LONGUEUR_TER-0.04,_LARGEUR_TER/2.-0.25) - pos;
    if(d.norme()<0.04+INCERT)
    {
      if(cfg == -1) strat_set_config_distrib(0);
      return true;
    }
  }
  if(cfg == 1 || cfg == -1)
  {
    vector_t d = vector_t((pos.x<_LONGUEUR_TER/2.)?0.04:_LONGUEUR_TER-0.04,_LARGEUR_TER/2.+0.25) - pos;
    if(d.norme()<0.04+INCERT)
    {
      if(cfg == -1) strat_set_config_distrib(1);
      return true;
    }
  }  
  vector_t d = vector_t((pos.x<_LONGUEUR_TER/2.)?0.289:_LONGUEUR_TER-0.289,_LARGEUR_TER-0.04) - pos;
  if(d.norme()<0.04+INCERT)
    return true;

  // Support de balise  
  if(pos.norme() < INCERT) return true;
  if((pos-vector_t(_LONGUEUR_TER/2.,0.)).norme() < INCERT) return true;
  if((pos-vector_t(_LONGUEUR_TER,0.)).norme() < INCERT) return true;  
  if((pos-vector_t(0.,_LARGEUR_TER)).norme() < INCERT) return true;
  if((pos-vector_t(_LONGUEUR_TER/2.,_LARGEUR_TER)).norme() < INCERT) return true;
  if((pos-vector_t(_LONGUEUR_TER,_LARGEUR_TER)).norme() < INCERT) return true;  
  
  // Plexi
  if(pos.x>_LONGUEUR_TER/2.-0.01-INCERT && pos.x<_LONGUEUR_TER/2.+0.01+INCERT)
  {
    if(pos.y>_LARGEUR_TER-INCERT)
      return true;
  }
  
  // Hors table
  if(pos.x<INCERT || pos.x>_LONGUEUR_TER-INCERT ||
     pos.y<INCERT || pos.y>_LARGEUR_TER-INCERT)
    return true;
    
  return false;
}
//------------------------------------------------------------------------------

