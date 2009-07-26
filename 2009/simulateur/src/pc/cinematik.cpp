#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "../common/comm.h"
#include "strategie.h"
#include "picAPI.h"

#include "cinematik.h"

bool time_out;

// position et vitesse actuelles
position_t pos;
vector_t speed;
float aspeed;

// Ancienne valeur des codeuses
int coder[2];

// vitesse de rotation instantanée de la roue (en rad/s)
float w[2];

float W_consigne[2];  // Vitesse demandée
float L_E[2];         // dernière erreur
float U_consigne[2];  // Tension demandée
float U_safe[2];      // Tension antidérapage
#define MAX_INTEGRAL  1000

float Cr;
float ULim;

// Calcule la tension antidérapage: mettre force à true pour forcément envoyer la tension aux moteurs
void refresh_U_safe(bool force = false);

// Asservissement en vitesse
void make_asserv(bool force = false);

#define _KPH  0.004
#define _KIH  0.000155
#define _KPL  0.04
#define _KIL  0.001


//------------------------------------------------------------------------------
void cine_init(int color)
{  
  pos = position_t((color==clGREEN)?_POS_INIT_X:(_LONGUEUR_TER-_POS_INIT_X),_POS_INIT_Y,((color==clGREEN)?_POS_INIT_A:(180-_POS_INIT_A))*M_PI/180.);
  speed = vector_t(0., 0.);
  aspeed = 0;
  
  for(int i=0; i<2; i++)
  {
    coder[i] = 0;
    
    w[i] = 0.;
    W_consigne[i] = 0.;
    
    U_consigne[i] = 0.; 
    U_safe[i] = 0.;
    L_E[i] = 0.;
  } 

  Cr = _CONST_TORQUE * _RAP_REDUC * _RENDEMNT * 0.98 * 0.2 * _TENSION / 2.;
  ULim = 0.5* // Marge de sécurité (mais augmente la distance de freinage/d'accélération)
            2* ((_MASSE / 3.) * _CONST_G * _ADHERE_ROUE * _RAYON_ROUE + Cr)
            / (_CONST_TORQUE * _RAP_REDUC * _RENDEMNT * 0.98);
  
  time_out = false; 
}
//------------------------------------------------------------------------------
position_t cine_get_position()
{
  return pos;
}
//------------------------------------------------------------------------------
void cine_set_position(position_t &_pos)
{
  pos = _pos;
}
//------------------------------------------------------------------------------
vector_t cine_get_speed()
{
  return speed;
}
//------------------------------------------------------------------------------
float cine_get_speed_wheel(int wheel)
{
  return w[wheel] * _RAYON_ROUE / _MOTOR_K;
}
//------------------------------------------------------------------------------
float cine_get_aspeed()
{
  return aspeed;
}
//------------------------------------------------------------------------------

//struct timeval ltime;   
void cine_OnCoderRecv(int left, int right)
{  
  int dleft = left - coder[0];
  int dright = right - coder[1];
  float distL = ((float)dleft)  / ((float)(_FREQ_CODER * _MOTOR_K)) * _RAYON_ROUE;
  float distR = ((float)dright) / ((float)(_FREQ_CODER * _MOTOR_K)) * _RAYON_ROUE;
  float dt = ((float)TIMER_CODER) * 0.001;
  
/*  struct timeval time;  
  gettimeofday(&time,NULL);   
  float t = ((float)time.tv_sec - ltime.tv_sec) + 0.000001*((float)time.tv_usec - ltime.tv_usec);
  ltime = time;
  printf("dt: %f\n",t); */
  float da = (distL - distR) / _ROUE_Y;
  float dx = (distL+distR)/2.*cos(pos.a);
  float dy = (distL+distR)/2.*sin(pos.a);
  pos.x+=dx;
  pos.y+=dy;
  pos.a+=da;
  
  w[0] = ((float)dleft)  / ((float)(_FREQ_CODER)) / dt;
  w[1] = ((float)dright) / ((float)(_FREQ_CODER)) / dt;  
  
  vector_t N = vector_t(cos(pos.a), sin(pos.a));
  speed = N * ((w[0]+w[1]) * _RAYON_ROUE / (2. * _MOTOR_K));
  aspeed = da / dt;
  
  coder[0] = left;
  coder[1] = right;

  make_asserv();
     
  if(strat_is_started() && strat_elapsed_time()>90.)
  {
    time_out = true;
    while(picMotorsPower(0., 0.)<0)
      usleep(1000);     
    strat_game_over();   
  }
}
//------------------------------------------------------------------------------
void make_asserv(bool force)
{
  float E;
  E = W_consigne[0] - w[0];    
  if(W_consigne[0] == 0)
    U_consigne[0] = 0.;
  else
  {
    if(w[0] * _RAYON_ROUE / _MOTOR_K < 0.1)
      U_consigne[0] = U_consigne[0] + _KPL * (E - L_E[0]) + _KIL * E;
    else    
      U_consigne[0] = U_consigne[0] + _KPH * (E - L_E[0]) + _KIH * E;
  }
  L_E[0] = E;  
  E = W_consigne[1] - w[1];    
  if(W_consigne[1] == 0)
    U_consigne[1] = 0.;
  else  
  {
    if(w[1] * _RAYON_ROUE / _MOTOR_K < 0.1)
      U_consigne[1] = U_consigne[1] + _KPL * (E - L_E[1]) + _KIL * E;
    else    
      U_consigne[1] = U_consigne[1] + _KPH * (E - L_E[1]) + _KIH * E;
  }
  L_E[1] = E;    
 
  //printf("WC: (%f,%f),  w:(%f,%f)\n",W_consigne[0],W_consigne[1],w[0],w[1]);
  refresh_U_safe(force);
}
//------------------------------------------------------------------------------
void refresh_U_safe(bool force)
{
  if(U_consigne[0]>_TENSION)  U_consigne[0] = _TENSION;
  if(U_consigne[0]<-_TENSION) U_consigne[0] = -_TENSION;
  if(U_consigne[1]>_TENSION)  U_consigne[1] = _TENSION;    
  if(U_consigne[1]<-_TENSION) U_consigne[1] = -_TENSION; 
    
  //Antidérapage
  float U,Umin,Umax;  // Calcul intermédiaire
  
  for(int i=0;i<2;i++)  
  {    
    if(U_safe[i] != U_consigne[i])
    {
      U = U_consigne[i];
      Umax = w[i] / (_CONST_SPEED) + ULim;
      Umin = w[i] / (_CONST_SPEED) - ULim;
      if(U > Umax)
        U = Umax;
      if(U < Umin)
        U = Umin;  
      if(U > _TENSION) U = _TENSION;
      else if(U < -_TENSION) U = -_TENSION;
      U_safe[i] = U / _TENSION;
      force = true;
    }
  }
  fflush(stdout);
  
  if(force && !time_out)
  {
   // printf("UC: (%f,%f) v: (%f,%f) vc:(%f,%f) AL: %f BL: %f AR: %f BR: %f\n",U_consigne[0], U_consigne[1], w[0]*_RAYON_ROUE / _MOTOR_K, w[1]*_RAYON_ROUE / _MOTOR_K, W_consigne[0]*_RAYON_ROUE / _MOTOR_K, W_consigne[1]*_RAYON_ROUE / _MOTOR_K,_AL, _BL, _AR, _BR);
    while(picMotorsPower(U_safe[0], U_safe[1])<0)
      usleep(1000);
  }
}
//------------------------------------------------------------------------------  
void cine_motors(float left, float right)
{
  W_consigne[0] = left / _RAYON_ROUE * _MOTOR_K;  
  W_consigne[1] = right / _RAYON_ROUE * _MOTOR_K; 
  
  make_asserv(true);      
}
//------------------------------------------------------------------------------
