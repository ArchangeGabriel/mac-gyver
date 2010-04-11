#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "picAPI.h"
#include "strategie.h"
#include "path_planning.h"
#include "cinematik.h"
#include "anticol.h"
#include "sdl.h"
#include "visualizer.h"

#include "pc_main.h"
#include "main.h"

// A décommenter pour utiliser la console
#define CONSOLE

pthread_t IAThread;
void* start_IA(void*);

pthread_t PPThread;
void* start_PP(void*);

pthread_t ACThread;
void* start_AC(void*);

#ifdef VISUALIZE
pthread_t SDLThread;
void* start_SDL(void*);
#endif

#ifdef CONSOLE
pthread_t ConsoleThread;
#endif

//------------------------------------------------------------------------------
int pc_main(int argc, char **argv)
{ 
  int color = -1; 
  int i;

  optind = 1;
  while((i = getopt(argc, argv, "by")) > 0)
  {
    switch(i)
    {
      case 'r':
      color = clBLUE;
      fprintf(stderr,"Je suis bleu!\n");      
      break;
      case 'g':
      color = clYELLOW;
      fprintf(stderr,"Je suis jaune!\n");
      break;
      default:
      break;
    }
  }
  
  if(color == -1)
  {
    color = clBLUE;
    fprintf(stderr,"Pas de couleur spécifiée, supposé blue. (Utiliser -b ou -y pour spécifier la couleur)\n");
  }

  strat_set_color(color);
  
  #ifdef VISUALIZE
  initSDL();
  visu_draw_background(0);
  Load_SDL_Background();  
  pthread_create(&SDLThread, NULL, start_SDL, NULL);   // Affichage SDL
  while(!is_SDL_ready()) usleep(10000);
  #endif

  // Initialise les fonction de callback
  picOnRecvJack(strat_lets_go);
  picOnRecvDistCaptors(anticolOnRecvCaptors);
  picOnRecvCoder(cine_OnCoderRecv);
  #ifdef SIMULATION  
  picRecvReset(strat_init);
  #endif

  pthread_create(&IAThread, NULL, start_IA, NULL);   // Intelligence Artificielle
  while(!is_strat_ready()) usleep(10000);
    
  pthread_create(&PPThread, NULL, start_PP, NULL);   // Path Planner
  pthread_create(&ACThread, NULL, start_AC, NULL);   // Anti Collision
  
  #ifdef CONSOLE
  atexit(exiting);
  pthread_create(&ConsoleThread, NULL, console, NULL);   // Console
  #endif
  
  picMainLoop();
  
  return 0;
}
//------------------------------------------------------------------------------
void* start_IA(void*)
{
  strat_init(); 
  return NULL;
}
//------------------------------------------------------------------------------
void* start_PP(void*)
{
  ppMainLoop(); 
  return NULL;
}
//------------------------------------------------------------------------------
void* start_AC(void*)
{
  anticolMainLoop(); 
  return NULL;
}
//------------------------------------------------------------------------------
void* start_SDL(void*)
{
  visu_draw_robot(); 
  return NULL;
}
//------------------------------------------------------------------------------
#ifndef SIMULATION
int main(int argc, char **argv)
{
  return pc_main(argc,argv);
}
#endif
//------------------------------------------------------------------------------

