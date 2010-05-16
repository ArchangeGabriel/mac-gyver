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
#include "sdl.h"

#include "cinematik.h"

bool time_out;

//#define MEGASS

// Dernier reçu de la valeur des codeuses
//double cine_last_time = 0;

// position et vitesse actuelles
position_t pos; // Position du centre des roues
//vector_t speed;
//double aspeed;

// Ancienne valeur des codeuses
//int coder[2];
#define NB_INT 10

int code[2][NB_INT];

typedef struct
{
  double time;
  int coder[2];
}coderdata_t;

coderdata_t Coder[NB_INT];
int pointeur_codeur = 0;

// vitesse de rotation instantanée de la roue (en rad/s)
double w[2]; // EN fait non. Vitesse au sol instantanée en m/s
double W_consigne[2];  // Vitesse demandée // Aussi en m/s
#ifdef MEGASS
double Erreurs[2][2];
#else
double L_E[2];         // dernière erreur
double I_E[2];         // intégrale de l'erreur
#endif
double U_consigne[2];  // Tension demandée // Oui, enfin, tension... entre -1 et 1, et puis pas demandée, mais en sortie plutot.
//double U_safe[2];      // Tension antidérapage
//#define MAX_INTEGRAL  1000

#define scale_W 2
#define DELTA_MAX 0.1

//double Cr;
//double ULim;

// Calcule la tension antidérapage: mettre force à true pour forcément envoyer la tension aux moteurs
//void refresh_U_safe(bool force = false);

// Asservissement en vitesse
void make_asserv();

// Constantes asservissement
#ifndef MEGASS
#ifdef SIMULATION
/*#define _KPH0  0.4//0.800
#define _KIH0  0.06//0.290
#define _KDH0  0.04*/
//29   0.011092
#define _KPH0  0.5//0.800
#define _KIH0  0.08//0.290
#define _KDH0  0.03
//36   0.011062
/*#define _KPH0  0.25//0.800
#define _KIH0  0.05//0.290
#define _KDH0  0.05
*/
#define _KPL0  1.15//1.15
#define _KIL0  0.038//0.038
#define _KDL0  0.
#define _KPH1  0.5
#define _KIH1  0.08
#define _KDH1  0.03
#define _KPL1  1.15
#define _KIL1  0.038
#define _KDL1  0.
#else
#define _KPH0  0.029//0.029//0.004
#define _KIH0  0.032//0.032//0.000155
#define _KDH0  0.
#define _KPL0  0.000000004//0.04
#define _KIL0  0.005//0.001
#define _KDL0  0.
#define _KPH1  0.029//0.029//0.004
#define _KIH1  0.032//0.032//0.000155
#define _KDH1  0.
#define _KPL1  0.000000004//0.04
#define _KIL1  0.005//0.001
#define _KDL1  0.
#endif // 0.031, 0.021 ou 0.029, 0.032
#endif

#ifndef VISUALIZE
int trace_X=0;
int max_X=800;
int max_Y=600;
double var = 0;
sdlWindow graph_window(max_X, max_Y, "Graph");  
#endif

FILE * fichiersortie;
bool asserv_on;

//------------------------------------------------------------------------------
void cine_init()
{  
  //char cheminfichier[256];
  //sprintf(cheminfichier,"./output/output_left_%1.7f_%1.7f.txt",_KPH0,_KIH0);
  //sprintf(cheminfichier,"./output/output_right_%1.7f_%1.7f.txt",_KPH1,_KIH1);

  // position of the center of the robot
  if(get_color() == clYELLOW)
    pos = position_t(_POS_INIT_X_YELLOW, _POS_INIT_Y_YELLOW, _POS_INIT_A_YELLOW*M_PI/180.); 
  else
    pos = position_t(_POS_INIT_X_BLUE,   _POS_INIT_Y_BLUE,   _POS_INIT_A_BLUE * M_PI/180.);
    
  cine_set_position(pos);  // position of the wheels' center
//  speed = vector_t(0., 0.);
//  aspeed = 0;
  
  //fichiersortie = fopen(cheminfichier,"w");
  
  cine_onoff(true);
  
  for(int i=0; i<2; i++)
  {
   // coder[i] = 0;
    
    w[i] = 0.;
    W_consigne[i] = 0.;
    
    U_consigne[i] = 0.; 
   // U_safe[i] = 0.;
#ifdef MEGASS
    Erreurs[0][i] = 0.;
    Erreurs[1][i] = 0.;
#else
    L_E[i] = 0.;
    I_E[i] = 0.;
#endif
  }
  for(int i=0;i<NB_INT;i++)
  {
    Coder[i].time = 0.0;
    Coder[i].coder[0] = 0;
    Coder[i].coder[1] = 0;
  } 

 /* Cr = _CONST_TORQUE * _RAP_REDUC * _RENDEMNT * 0.98 * 0.2 * _TENSION / 2.;
  ULim = 0.85* // Marge de sécurité (entre 0 et 1) (mais augmente la distance de freinage/d'accélération si trop petit)
            2* ((_MASSE / 3.) * _CONST_G * _ADHERE_ROUE * _RAYON_ROUE + Cr)
            / (_CONST_TORQUE * _RAP_REDUC * _RENDEMNT * 0.98);
  */
  time_out = false; 

//  ULim = 4.502167 (pour 0.5)
//  printf("ULim = %f\n",ULim);

  //printf("Ul; Ur; VCl; VCr; Vl; Vr\n");
}
//------------------------------------------------------------------------------
position_t cine_get_position()
{
  vector_t c(_ROUE_X*cos(pos.a), _ROUE_X*sin(pos.a));
  return position_t(pos.x - c.x, pos.y - c.y, pos.a);
}
//------------------------------------------------------------------------------
position_t cine_get_wheel_center_position()
{
  return pos;
}
//------------------------------------------------------------------------------
void cine_set_position(position_t &_pos)
{
  vector_t c(_ROUE_X*cos(_pos.a), _ROUE_X*sin(_pos.a));
  pos.x = _pos.x + c.x;
  pos.y = _pos.y + c.y;
  pos.a = _pos.a;    
}
//------------------------------------------------------------------------------
/*vector_t cine_get_speed()
{
  return speed;
}*/
//------------------------------------------------------------------------------
double cine_get_speed_wheel(int wheel)
{
  return w[wheel];
}
//------------------------------------------------------------------------------
/*double cine_get_aspeed()
{
  return aspeed;
}*/
//------------------------------------------------------------------------------
void make_asserv_II_La_revanche2(double *V_inst,double *V_cons,double *U_cons,double *ErrCour,double *ErrVit);
//-----------------------------------------------------------------------------

void cine_OnCoderRecv(double time, int left, int right)
{ 
  double dt = time - Coder[pointeur_codeur].time; // Temps courant - dernier temps enregistré.
  if(dt == 0.) return;

  int dleft = left - Coder[pointeur_codeur].coder[0];
  int dright = right - Coder[pointeur_codeur].coder[1];

  double distL = ((double)dleft)  / ((double)(_FREQ_CODER));
  double distR = ((double)dright) / ((double)(_FREQ_CODER));

  double da = (distL - distR) / _ROUE_Y;
  double dx = (distL+distR)/2.*cos(pos.a);
  double dy = (distL+distR)/2.*sin(pos.a);
  pos.x+=dx;
  pos.y+=dy;
  pos.a+=da;
  
  pointeur_codeur ++;
  pointeur_codeur %= NB_INT;

  dleft = left - Coder[pointeur_codeur].coder[0];
  dright = right - Coder[pointeur_codeur].coder[1];

  distL = ((double)dleft)  / ((double)(_FREQ_CODER));
  distR = ((double)dright) / ((double)(_FREQ_CODER));

  dt = time - Coder[pointeur_codeur].time;
  w[0] = distL / dt;
  w[1] = distR / dt;
  
  Coder[pointeur_codeur].time = time;
  Coder[pointeur_codeur].coder[0] = left;
  Coder[pointeur_codeur].coder[1] = right;
  
#ifndef MEGASS
  if(asserv_on) make_asserv();
#else
  if(asserv_on) make_asserv_II_La_revanche2(w,W_consigne,U_consigne,Erreurs[0],Erreurs[1]);
#endif
}
//------------------------------------------------------------------------------
#ifndef MEGASS
void make_asserv()
{
  double E;
  double temp;
  for(int i=0; i<2 ; i++)
  {
    E = W_consigne[i] - w[i];    
/*    if(W_consigne[i] < 0.01)
      U_consigne[i] = 0.;
    else*/
   
    {
      //if(W_consigne[i]/* * _RAYON_ROUE / _MOTOR_K*/ < 0.1)
      //  U_consigne[i] += _KPL * (E - L_E[i]) + _KIL * E; // d'abord le KPL, pas d'oscillation
     // else    
      //  U_consigne[i] += _KPH * (E - L_E[i]) + _KIH * E; // puis le KIL, augmente la vitesse de reponse
      if(W_consigne[i] < 0.1)
      {
        if(i==0) temp = _KPL0 * E + _KIL0 * I_E[i] + _KDL0 * (E - L_E[i]); //LEFT
      	if(i==1) temp = _KPL1 * E + _KIL1 * I_E[i] + _KDL1 * (E - L_E[i]); //RIGHT
      	#ifdef SIMULATION      	
        U_consigne[i] = temp;
        #else
        if(temp > 0)
          U_consigne[i] = fmin(temp,DELTA_MAX);
        else
          U_consigne[i] = fmax(temp,-DELTA_MAX);
        #endif
      }
      else
      {
        if(i==0) temp = _KPH0 * E + _KIH0 * I_E[i] + _KDH0 * (E - L_E[i]); //LEFT
	      if(i==1) temp = _KPH1 * E + _KIH1 * I_E[i] + _KDH1 * (E - L_E[i]); //RIGHT
      	#ifdef SIMULATION      	
        U_consigne[i] = temp;
        #else
        if(temp > 0)
          U_consigne[i] = fmin(temp,DELTA_MAX);
        else
          U_consigne[i] = fmax(temp,-DELTA_MAX);
        #endif
      }
    }
    L_E[i] = E;
    I_E[i] += E;
  }
 
  #ifndef VISUALIZE
  if(trace_X<max_X)
  {
    graph_window.LigneVerticaleSDL(trace_X,0,max_Y, graph_window.makeColorSDL(0,0,0));
    graph_window.setPixelVerif(trace_X, max_Y/2+(int)((double)(max_Y/2.)*W_consigne[0]*scale_W), graph_window.makeColorSDL(0,255,0));        
    graph_window.setPixelVerif(trace_X, max_Y/2+(int)((double)(max_Y/2.)*w[0]*scale_W), graph_window.makeColorSDL(128,255,0));
    
    //graph_window.setPixelVerif(trace_X, max_Y/2+(int)((double)(max_Y/2.)*W_consigne[1]*scale_W), graph_window.makeColorSDL(0,0,255));        
    //graph_window.setPixelVerif(trace_X, max_Y/2+(int)((double)(max_Y/2.)*w[1]*scale_W), graph_window.makeColorSDL(128,0,255));
    
    graph_window.LigneHorizontaleSDL(0,max_Y/2,max_X, graph_window.makeColorSDL(255,255,255));
    trace_X++;
  }  
  if(trace_X>=100) var += pow(w[0]-W_consigne[0],2);
  if(w[0]>W_consigne[0])
    printf("dépassement: %d!\n", trace_X);
    
  if(trace_X==max_X) {trace_X=0; printf("std dev: %f\n", sqrt(var/double(max_X-100)));}
  if((trace_X%1)==0) graph_window.RefreshSDL();
  printf("WC: (%f,%f),  w:(%f,%f),  Uc:(%f,%f)\n",W_consigne[0],W_consigne[1],w[0],w[1],U_consigne[0],U_consigne[1]);  
  #endif
  
  pic_MotorsPower(U_consigne[0], U_consigne[1]);

  if(fichiersortie) {fprintf(fichiersortie,"%f;%f;%f;%f;%f;%f\n",W_consigne[0],W_consigne[1],w[0],w[1],U_consigne[0],U_consigne[1]);fflush(fichiersortie);}
  //refresh_U_safe(force);
}
#endif

//------------------------------------------------------------------------------
void cine_onoff(bool onoff)
{
  asserv_on = onoff;
  cine_reset_asserv();
}
//------------------------------------------------------------------------------
void cine_reset_asserv()
{
  U_consigne[0] = 0;
  U_consigne[1] = 0;
#ifndef MEGASS
  L_E[0] = 0;
  L_E[1] = 0; 
  I_E[0] = 0;
  I_E[1] = 0;
#else
  Erreurs[0][0] = 0;
  Erreurs[0][1] = 0;
  Erreurs[1][0] = 0;
  Erreurs[1][1] = 0;
#endif 
}
//------------------------------------------------------------------------------
/*void refresh_U_safe(bool force)
{
  for(int i=0; i<2 ; i++)
  {
    if(U_consigne[i]>  _TENSION) U_consigne[i] =  _TENSION;
    if(U_consigne[i]< -_TENSION) U_consigne[i] = -_TENSION;
  }
    
  //Antidérapage
  double U,Umin,Umax;  // Calcul intermédiaire
  
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
      U_consigne[i] = U;      
      U_safe[i] = U / _TENSION;
      force = true;
    }
  }
 
  if(force && !time_out)
  {
   // #endif  
    //printf("%f; %f; %f; %f; %f; %f\n",U_safe[0],U_safe[1], W_consigne[0]*_RAYON_ROUE / _MOTOR_K, W_consigne[1]*_RAYON_ROUE / _MOTOR_K,w[0]*_RAYON_ROUE / _MOTOR_K, w[1]*_RAYON_ROUE / _MOTOR_K);     
    //U_safe[0] = U_consigne[0] / _TENSION;
    //U_safe[1] = U_consigne[1] / _TENSION;
    //printf("%f ; %f \n",U_safe[0],U_safe[1]);
    pic_MotorsPower(U_consigne[0], U_consigne[1]);
 // }
}*/
//------------------------------------------------------------------------------  
void cine_motors(double left, double right)
{
  if(strat_is_started() && !strat_is_game_over())
  {
    W_consigne[0] = left; //  / _RAYON_ROUE * _MOTOR_K;
    W_consigne[1] = right; //  / _RAYON_ROUE * _MOTOR_K;
  }
}
//------------------------------------------------------------------------------

#define Kp_c 1.
#define Ki_c 1.
#define Kd_c 1.

#define Kp_v 1.
#define Ki_v 1.
#define Kd_v 1.

void make_asserv_II_La_revanche2(double *V_inst,double *V_cons,double *U_cons,double *ErrCour,double *ErrVit)
{
  double erreur;
  double correction;
// Asservissment courbure
  erreur = V_inst[0]*V_cons[1] - V_inst[1]*V_cons[0];
  correction = Kp_c*(erreur - ErrCour[0]) + Ki_c*erreur - Kd_c*(2*ErrCour[0] - ErrCour[1] - erreur);
  correction = ((correction > DELTA_MAX)||(correction < -DELTA_MAX)) ? DELTA_MAX : correction;
  U_cons[0] -= correction;
  U_cons[1] += correction;
  ErrCour[1] = ErrCour[0];
  ErrCour[0] = erreur;

// Asservissement vitesse
  erreur = V_inst[0] + V_inst[1] - V_cons[0] - V_cons[1];
  correction = Kp_v*(erreur - ErrVit[0]) + Ki_v*erreur - Kd_v*(2*ErrVit[0] - ErrVit[1] - erreur);
  correction = ((correction > DELTA_MAX)||(correction < -DELTA_MAX)) ? DELTA_MAX : correction;
  U_cons[0] += correction;
  U_cons[1] += correction;
  ErrVit[1] = ErrVit[0];
  ErrVit[0] = erreur;
}

/*#define Kp 1
#define Ki 1
#define Kd 1
#define COMMON_DIVIDER 1000 // 1/(précision des coeffs)

// Asservissement prototypal
void make_asserv_II_La_revanche(int *V_inst,int *V_cons,int *U_cons,int *Err)
{
  int erreur = V_inst[0]*V_cons[1] - V_inst[1]*V_cons[0];
  int correction = Kp*(erreur - Err[0]) + Ki*erreur + Kd*(2*Err[0] - Err[1] - erreur);
  correction /= COMMON_DIVIDER;
  U_cons[0] -= correction;
  U_cons[1] += correction;
  Err[1] = Err[0];
  Err[0] = erreur;
}*/

