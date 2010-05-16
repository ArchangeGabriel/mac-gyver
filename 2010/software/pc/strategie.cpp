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
#include "../common/simul.h"
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

#ifdef SIMULATION
int SendInfo(int type = MSG_INFO, int input_config = -1);  // pour envoyer la couleur au simu
#endif

// Strategie prête
bool strat_ready = false;

// Gestion du temps
bool started;
bool game_over;
struct timeval start_time;

// Configuration du terrain
int input_config_terrain;   // passed in the command line
int config_terrain;

// Recallage
bool recallage_necessaire;

// Boucle principale de la strategie
void strat_MainLoop();

// Attend 1m30s
pthread_t game_over_thread;
void* game_over_look_up(void*);

//------------------------------------------------------------------------------
void* strat_MainLoop(void *_input_config)
{	 
  input_config_terrain = *(int*)_input_config;
  pthread_create(&game_over_thread, NULL, game_over_look_up, NULL);
  
 	// On initialise tout 
  strat_init();
  
  strat_ready = true;  

  // Waits until the starting signal is given
  fprintf(stderr,">>> Waiting for jack...\n");  fflush(stdout);
  while(!started)
    usleep(10000);
  
  // et c'est parti!
  fprintf(stderr,">>> Let's go!\n");  fflush(stdout);  
  
  // Commenter Thread PT
  //cine_motors(0.3,0.3);
  //return NULL;

  // on attend qu'on ait la config du terrain     
  // while(!config_terrain) usleep(10000);    
 
     
  // on va à un endroit improbable   
  // avec les splins
  /* pt_go_to(symetrize(position_t(1.5,1.222,45./180.*M_PI)),tpWAYPOINT);
  wait_for_it(pt_go_to(symetrize(position_t(2.63,1.85,M_PI_2)),tpDEST));*/
   
  /*pt_add_step(symetrize(position_t(0.35,0.52,M_PI/2.)),tpMOVE);
  pt_add_step(symetrize(position_t(0.35,0.52,29.05/180.*M_PI)),tpTURN);
  pt_add_step(symetrize(position_t(2.65,1.85,29.05/180.*M_PI)),tpMOVE);
  wait_for_it(pt_add_step(symetrize(position_t(2.65,1.85,M_PI/2.)),tpTURN));  
  */
  pt_add_step(symetrize(position_t(0.35,0.55,M_PI/2.)),tpMOVE);
  pt_add_step(symetrize(position_t(0.35,0.55,29.05/180.*M_PI)),tpTURN);
  wait_for_it(pt_add_step(symetrize(position_t(2.35,1.70,29.05/180.*M_PI)),tpMOVE));
  pic_move_door(MOTOR_DOOR_CLOSED);
  sleep(1);
  pic_move_door(MOTOR_DOOR_FREE);
  pt_add_step(symetrize(position_t(2.35,1.70,0)),tpTURN); 
  pt_add_step(symetrize(position_t(2.65,1.70,0)),tpMOVE);
  pt_add_step(symetrize(position_t(2.65,1.70,M_PI/2.)),tpTURN);     
  wait_for_it(pt_add_step(symetrize(position_t(2.65,1.87,M_PI/2.)),tpMOVE));

  pic_move_door(MOTOR_DOOR_OPEN);
  pic_move_pusher(MOTOR_PUSHER_FORWARD);  
  

 /* pic_move_door(MOTOR_DOOR_CLOSED);
  wait_for_it(pt_add_step(symetrize(position_t(2.63,1.95,M_PI_2)),tpWAYPOINT,false));  
  pic_move_door(MOTOR_DOOR_OPEN);
  pic_move_pusher(MOTOR_PUSHER_FORWARD);
  sleep(6);
  pic_move_pusher(MOTOR_PUSHER_BACKWARD);  */

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
  
  #ifdef SIMULATION
  SendInfo(MSG_POS_INFO, input_config_terrain);
  #endif
  
  // A commenter: sert aux test de reconnaissance des configs
  // Try to find configuration
  /*int config = wc_reco_config();
  if(config == input_config_terrain)
    printf("OK\n");
  else
    printf("FAILED\n");
  exit(0);*/
   	 
	// Place les composants
  //pic_move_pusher(MOTOR_PUSHER_BACKWARD);
  pic_move_door(MOTOR_DOOR_OPEN);
  //sleep(1);
  pic_move_door(MOTOR_DOOR_FREE);
  pic_move_pusher(MOTOR_PUSHER_BACKWARD);
  //while(!pic_where_pusher(MOTOR_PUSHER_BACKWARD))
    //usleep(10000);
}
//------------------------------------------------------------------------------
void* game_over_look_up(void*)
{
  while(true)
  {
    if(strat_is_started() && strat_elapsed_time()>90.)
    {
      cine_motors(0,0);
      strat_game_over();   
    }
    sleep(1);
  }
  return NULL;
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
  cine_init();
}
//------------------------------------------------------------------------------
bool strat_is_started()
{
  return started;
}
//------------------------------------------------------------------------------
bool strat_is_game_over()
{
  return game_over;
}
//------------------------------------------------------------------------------
void strat_game_over()
{
  printf("Game Over...\n");
  fflush(stdout);
  cine_reset_asserv();
 // cine_motors(0.0,0.0);
  sleep(1000);
  game_over = true;  
  started = false;
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
