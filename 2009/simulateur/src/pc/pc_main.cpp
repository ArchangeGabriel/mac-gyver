#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "picAPI.h"
#include "strategie.h"
#include "path_planning.h"
#include "anticol.h"
#include "sdl.h"

#include "pc_main.h"
#include "main.h"

pthread_t IAThread;
void* start_IA(void*);

pthread_t PPThread;
void* start_PP(void*);

pthread_t ACThread;
void* start_AC(void*);


pthread_t SDLThread;
void* start_SDL(void*);

// A décommenter pour utiliser la console
#define CONSOLE

#ifdef CONSOLE
pthread_t ConsoleThread;
#endif

//------------------------------------------------------------------------------
#ifdef SIMULATION
int pc_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{ 
  int color = -1; 
  int i;

  while((i = getopt(argc, argv, "rg")) > 0)
    switch(i)
    {
      case 'r':
      color = clRED;
      fprintf(stderr,"Franchement rouge t'as vu!\n");      
      break;
      case 'g':
      color = clGREEN;
      fprintf(stderr,"Franchement vert t'as vu!\n");
      break;
      default:
      break;
    }
  
  if(color == -1)
  {
    color = clGREEN;
    fprintf(stderr,"Pas de couleur spécifiée, supposé vert. (Utiliser -r ou -g pour spécifier la coleur)\n");
  }

  strat_set_color(color);
  
  #ifdef USE_SDL_FRANCHEMENT_OUAIS_TAS_VU
  initSDL();
  //SDL_First_Background();
  //Load_SDL_Background();  
  //pthread_create(&SDLThread, NULL, start_SDL, NULL);   // Anti Collision  
  #endif

  picOnRecvJack(strat_lets_go); // Point d'entrée pour démarrer le jeu ici
  picOnRecvDistCaptors(anticolOnRecvCaptors);
  #ifdef SIMULATION  
  picRecvReset(strat_init);
  #endif

  pthread_create(&IAThread, NULL, start_IA, NULL);   // Intelligence Artificielle
  pthread_create(&PPThread, NULL, start_PP, NULL);   // Path Planner
  pthread_create(&ACThread, NULL, start_AC, NULL);   // Anti Collision
  
  #ifdef CONSOLE
  atexit(exit_console);
  pthread_create(&ConsoleThread, NULL, console, NULL);   // Anti Collision 
  #endif
  
  picMainLoop();
  
  #ifdef USE_SDL_FRANCHEMENT_OUAIS_TAS_VU
  SDL_Quit();
  #endif  
  
  

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
  SDL_Draw_Robot(); 
  return NULL;
}
//------------------------------------------------------------------------------

