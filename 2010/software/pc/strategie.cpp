#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <vector>
#include <time.h>

using namespace std;

#define PC_INCLUDE
#include "../common/const.h"
#include "picAPI.h"
#include "webcamAPI.h"
#include "cinematik.h"
#include "path_planning.h"
#include "strategie.h"
#include "usb.h"
#include "../common/comm.h"
#include "webcam.hpp"

// Couleur du robot
int color;

// Gestion du temps
bool started;
bool game_over;
struct timeval start_time;

// Recallage
bool recallage_necessaire;

// Attend qu'un mutex se libère et le détruit
void wait_for_it(pthread_mutex_t *mutex);

// Prend une position pour le vert et renvoit la position adaptée à la couleur
position_t symetrize(position_t pos);


//------------------------------------------------------------------------------
void strat_init()
{
  fprintf(stderr,"IA thread ok...\n");  fflush(stdout);
  // Initialise le suivit de position
  cine_init(color);
  picOnRecvCoder(cine_OnCoderRecv);
  
  started = false;
  game_over = false;
  recallage_necessaire = false;  
 
  fprintf(stderr,"Waiting for jack...\n");  fflush(stdout);
  while(!started)
  {
    usleep(100000);
  }
  
  stratMainLoop();
}
//------------------------------------------------------------------------------
void strat_set_color(int _color)
{
  color = _color;
}
//------------------------------------------------------------------------------
int strat_get_color()
{
  return color;
}
//------------------------------------------------------------------------------
float strat_elapsed_time()
{
  struct timeval time;
  gettimeofday(&time,NULL);   
  return ((float)(time.tv_sec-start_time.tv_sec))+(((float)(time.tv_usec-start_time.tv_usec)))*0.000001;
}
//------------------------------------------------------------------------------
void strat_lets_go()
{
  gettimeofday(&start_time,NULL); 
   
  started = true;
}
//------------------------------------------------------------------------------
void stratMainLoop()
{
  fprintf(stderr,"Let's go!\n");  fflush(stdout);
  
  webcam_t WC("0", 640, 480);
  WC.start();
  WC.do_capture();
  image_t img = WC.get_image2();
 
  
  wait_for_it(pp_go_to(symetrize(position_t(2.6,1.3,M_PI)),tpDEST,false,false,false));

  while(true)
  {
    usleep(100000);    
  }   
}
//------------------------------------------------------------------------------
bool strat_is_started()
{
  return started;
}
//------------------------------------------------------------------------------
void strat_game_over()
{
  printf("Game Over...\n");
  fflush(stdout);
  sleep(1000);
  game_over = true;  
}
//------------------------------------------------------------------------------
void wait_for_it(pthread_mutex_t *mutex)
{
  if(mutex == NULL) return;
    
  pthread_mutex_lock(mutex);    
  pthread_mutex_unlock(mutex);      
  pthread_mutex_destroy(mutex);      
  delete mutex;
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
void strat_recallage()
{
  // FIXME
  // Faire la manip de recalage
  // utiliser cine_set_position(position_t newpos)
  recallage_necessaire = false;
}
//------------------------------------------------------------------------------
void strat_signal_recall()
{
  recallage_necessaire = true;
}
//------------------------------------------------------------------------------
