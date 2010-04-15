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
#include "path_tracker.h"
#include "strategie.h"
#include "usb.h"
#include "visualizer.h"
#include "sdl.h"
#include "../common/comm.h"
#include "webcam.hpp"

// Strategie prête
bool strat_ready = false;

// Couleur du robot
int color;

// Gestion du temps
bool started;
bool game_over;
struct timeval start_time;

// Configuration du terrain
int config_terrain;

// Recallage
bool recallage_necessaire;

// Boucle principale de la strategie
void stratMainLoop();

//------------------------------------------------------------------------------
void strat_init()
{
  fprintf(stderr,"IA thread...                    ok\n");  fflush(stdout);

  started = false;
  game_over = false;
  recallage_necessaire = false;  
  config_terrain = 0;
 
  // Initialise le suivit de position
  cine_init();
  
  // Initialise la webcam
  webcam_init();
  
  strat_ready = true; 
         
  // Waits until the starting signal is given
  fprintf(stderr,">>> Waiting for jack...\n");  fflush(stdout);
  while(!started)
  {       
    usleep(10000);
  }
  
  stratMainLoop();
}
//------------------------------------------------------------------------------
bool is_strat_ready()
{
  return strat_ready;
}
//------------------------------------------------------------------------------
void stratMainLoop()
{
  fprintf(stderr,">>> Let's go!\n");  fflush(stdout);
  
  while(!config_terrain) usleep(10000);    
   
  wait_for_it(pt_go_to(symetrize(position_t(2.7,1.8,M_PI_2)),tpDEST,false));

  while(true)
  {
    usleep(100000);    
  }   
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
void strat_set_config_terrain(int c)
{
  #ifdef VISUALIZE
  visu_draw_background(c);  
  Load_SDL_Background();  
  #endif

  config_terrain = c;
}
//------------------------------------------------------------------------------
int strat_get_config_terrain()
{
  return config_terrain;
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
