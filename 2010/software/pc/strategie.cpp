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

// Gestion du temps
bool started;
bool game_over;
struct timeval start_time;

// Configuration du terrain
int config_terrain;

// Recallage
bool recallage_necessaire;

// Boucle principale de la strategie
void strat_MainLoop();

//------------------------------------------------------------------------------
void* strat_MainLoop(void*)
{	 
 	// On initialise tout 
  strat_init();
  
  strat_ready = true;  
         
  // Waits until the starting signal is given
  fprintf(stderr,">>> Waiting for jack...\n");  fflush(stdout);
  while(!started)
  {       
    usleep(10000);
  }
  
  // et c'est parti!
  fprintf(stderr,">>> Let's go!\n");  fflush(stdout);  
  
  // on attend qu'on ait la config du terrain     
  while(!config_terrain) usleep(10000);    
     
  // on va à un endroit improbable   
  wait_for_it(pt_go_to(symetrize(position_t(2.7,1.8,M_PI_2)),tpDEST,false));

  // on attend et on sert à rien
  while(true)
    usleep(100000);
  
  return NULL;
}
//------------------------------------------------------------------------------
void strat_init()
{
	started = false;
  game_over = false;
  recallage_necessaire = false;  
  config_terrain = 0;
 
  // Initialise le suivit de position
  cine_init();
    
  // Vérifie la connection avec les pics
  while(!pic_is_ready())
    usleep(10000);
	 
	// Place les composants
  pic_move_pusher(MOTOR_PUSHER_BACKWARD);
  pic_move_door(MOTOR_DOOR_OPEN);
  while(!pic_where_pusher(MOTOR_PUSHER_BACKWARD))
    usleep(10000);
}
//------------------------------------------------------------------------------
bool strat_is_ready()
{
  return strat_ready;
}
//------------------------------------------------------------------------------
void strat_set_config_terrain(int c)
{
  #ifdef VISUALIZE
  visu_draw_background(c);  
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
  
  // reset's position of the robot
  position_t pos = symetrize(position_t(_POS_INIT_X,_POS_INIT_Y,_POS_INIT_A*M_PI/180.));
  cine_set_position(pos);
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
