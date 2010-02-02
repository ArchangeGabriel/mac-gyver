#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <math.h>

#include "../common/comm.h"
#include "../common/const.h"
#include "../pic/simul.h"
#include "io.h"
#include "sdl.h"

#include "keycontrol.h"

extern pthread_cond_t pic_cond;
extern sem_t pic_sem;

//------------------------------------------------------------------------------void* keycontrol_main(void *Args)
{
  //<simulation>
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
  PIC_ARG *Simul=(PIC_ARG*)Args; 
  Simul->robot->data[Simul->my_nbr].destX = _POS_INIT_X;
  Simul->robot->data[Simul->my_nbr].destY = _POS_INIT_Y;
  Simul->robot->data[Simul->my_nbr].destA = _POS_INIT_A;        
  //</simulation> 
  
  double posX,posY,angle;     // la position
  
  SDL_Event event;
  bool key[6]={false,false,false,false,false,false};  //0-up 1-down 2-left 3-right
  double vitesse=1.;
  double u[2];
  
  
  //Initialisation du calcul de la position
  posX  = _POS_INIT_X;
  posY  = _POS_INIT_Y;
  angle = _POS_INIT_A;
  
  long codeuse[2];
  codeuse[0]=get_input(Simul,CODER,0);
  codeuse[1]=get_input(Simul,CODER,1);    
  long gauche = codeuse[0], dgauche;
  long droite = codeuse[1], ddroite;
  double distgauche, distdroite, dalpha, dx, dy;
  
  printf("\n\
You're using the keyboard!\n\
 - press 'a'/'q' to move faster/slower\n\
 - press 'z'/'s' to open/close the clamp\n\
 - press 'r'/'f'/'v' to make the belt roll outside/stop/roll inside\n\
 - press 't'/'g'/'b' to move the clamp fowards/middle/backwards\n\
 - press 'y'/'n' to move the clamp up/down\n\n\
 "); 
  
  while(!is_SDL_ready()) {pthread_yield();}
  while(1)
  { 
    SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_ALLEVENTS);
    if(event.type==SDL_KEYDOWN)
      switch(event.key.keysym.sym)
      {
        case 273: key[0]=true; break;   // up
        case 274: key[1]=true; break;   // down
        case 276: key[2]=true; break;   // left
        case 275: key[3]=true; break;   // right 
        case 97:  key[4]=true; break;   // a (vitesse+)
        case 113: key[5]=true; break;   // q (vitesse-)  
        case 114: set_output(Simul,BELT,0,1); break;  // r
        case 102: set_output(Simul,BELT,0,0); break;    // f
        case 118: set_output(Simul,BELT,0,-1); break; // v       
        case 116: set_output(Simul,HPOS,0,posClampFront); break;     // t
        case 103: set_output(Simul,HPOS,0,posClampMiddle); break;     // g        
        case 98:  set_output(Simul,HPOS,0,posClampBack); break;     // b
        case 121: set_output(Simul,VPOS,0,400); break;    // y
        case 110:  set_output(Simul,VPOS,0,-100); break; // n        
        case 122: set_output(Simul,CLAMP,0,stClampOpen); break;  // z
        case 115: set_output(Simul,CLAMP,0,stClampClosed); printf("%f\n",((float)MAX_DIST_CAPT) - ((float)Simul->robot->dist_captors[0].measure()) * MAX_DIST_CAPT / ((float)RANGE_DIST_CAPT));  break;  // s 
        default: break; //printf("%d\n",event.key.keysym.sym);
      }
    if(event.type==SDL_KEYUP)
      switch(event.key.keysym.sym)
      {
        case 273: key[0]=false; break;  // up
        case 274: key[1]=false; break;  // down
        case 276: key[2]=false; break;  // left
        case 275: key[3]=false; break;  // right    
        case 97: 
        if(key[4])
        {
          vitesse+=0.1; 
          if(vitesse>1.) vitesse=1.;
          key[4]=false;
        }
        break;
        case 113:
        if(key[5])
        {
          vitesse-=0.1;
          if(vitesse<0.) vitesse=0.;
          key[5]=false;
        }
        break;           
        default: break;                
      }
    if(event.type==SDL_KEYDOWN || event.type==SDL_KEYUP)     
    {
      u[0]=0.;
      u[1]=0.;             
      if(key[0]) {u[0]+=1.; u[1]+=1.;}  // up
      if(key[1]) {u[0]-=1.; u[1]-=1.;}  // down
      if(key[2])  // left
      {
        if(u[0]!=0.) u[0]/=2.; else {u[0]=-1.; u[1]=1.;}
      }
      if(key[3])  // right
      {
        if(u[1]!=0.) u[1]/=2.; else {u[0]=1.; u[1]=-1.;}
      }
      int UG=vitesse*u[0]*((double)RANGE_MOTOR);
      int UD=vitesse*u[1]*((double)RANGE_MOTOR);
      set_output(Simul,MOTORS,0,UG);
      set_output(Simul,MOTORS,1,UD);    
      event.type=!(SDL_KEYDOWN|SDL_KEYUP);       
    }   
      
    //Calcul de la position
    codeuse[0]=get_input(Simul,CODER,0);
    codeuse[1]=get_input(Simul,CODER,1);
    
    dgauche = codeuse[0] - gauche;
    ddroite = codeuse[1] - droite;  
    distgauche = dgauche * _RAYON_ROUE  / (_FREQ_CODER * _MOTOR_K);
    distdroite = ddroite * _RAYON_ROUE  / (_FREQ_CODER * _MOTOR_K);
    dalpha=(distgauche-distdroite) / _ROUE_Y;
    dx = (distdroite+distgauche)/2.*cos(angle);
    dy = (distdroite+distgauche)/2.*sin(angle);
    posX+=dx;//+_ROUE_X*(cos(angle)-cos(angle+dalpha));   
    posY+=dy;//+_ROUE_X*(sin(angle)-sin(angle+dalpha));
    angle+=dalpha;    
    gauche = codeuse[0]; 
  	droite = codeuse[1];

    //<simulation>
    Simul->robot->data[Simul->my_nbr].posX=posX;
    Simul->robot->data[Simul->my_nbr].posY=posY;    
    Simul->robot->data[Simul->my_nbr].angle=angle;    
    pthread_barrier_wait(Simul->barrier);
    pthread_barrier_wait(Simul->barrier);    
    if(*Simul->STOP)
    {
      *Simul->alive_thread = *Simul->alive_thread-1;
      break;
    }
    //</simulation>
  }
  return NULL;
}
//------------------------------------------------------------------------------



