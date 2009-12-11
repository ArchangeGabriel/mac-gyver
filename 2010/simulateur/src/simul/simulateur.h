#ifndef SIMULATEURH
#define SIMULATEURH

#include <pthread.h>

#include "parse_params.h"
#include "robot.h"
#include "camera.h"

//----------------------------------------------------------------------------//
//                                 Simulateur                                 //
//----------------------------------------------------------------------------//
class simulateur_t
{
  public:
  vector<robot_t*> robot_list;
  vector<pthread_t> robot_thread_list;
  simul_info_t params;
  pthread_mutex_t mutex;      //pour être syncro avec la console (exemple, la commande "re")
  pthread_barrier_t barrier;  //pour être sûr d'alterner simulation et gestion du pic
  int STOP;                   // arrêt des threads
  int nbr_pics;               // nombre de threads    
  struct timeval totalt,calct,stept;
  int counter;
  vector_t Oxy;
  vector_t Xaxis;
  vector_t Yaxis;  
  double Oz;
  camera_t view3D;

  simulateur_t(); 
  ~simulateur_t();
  void load_params(const char *file);
  void load_simul();  
  void add_object(object_t *obj);
  void clear_objects();  
  void add_robot(robot_t *robot,void* (*func1)(void*), void* (*func2)(void*) = NULL);  
/*  void lock_robots();
  void unlock_robots();*/
  void do_step_simul();
  void draw();
  void draw_background();
};
#endif

