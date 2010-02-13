#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "pic/simul.h"
#include "pic/pic_main1.h"
#include "pic/pic_main2.h"
#include "the_cup/colors.h"
#include "the_cup/the_playground.h"
#include "keycontrol.h"

#include "simulateur.h"

//----------------------------------------------------------------------------//
//                                                                            //
//                                Simulateur                                  //
//                                                                            //
//----------------------------------------------------------------------------//
simulateur_t::simulateur_t()
{
  pthread_mutex_init(&mutex,NULL);
  params.reset();
  robot_list.clear();
  robot_thread_list.clear();
  nbr_pics = 0;  
}
//----------------------------------------------------------------------------
simulateur_t::~simulateur_t()
{
  pthread_mutex_destroy(&mutex);
}
//----------------------------------------------------------------------------
void simulateur_t::load_params(const char *file)
{
  printf("Loading parameters...           ");
  fflush(stdout);
  params.load_params(file);
  printf("ok\n");
  fflush(stdout);
}
//----------------------------------------------------------------------------
void simulateur_t::load_simul()
{ 
  STOP=0;    
  nbr_pics = 0;    
  if(params.nbr_robots)
  {
    vector<robot_t *> robots;      
    robots.resize(params.nbr_robots);
    for(int i=0;i<params.nbr_robots;i++)
    {
      robots[i] = new robot_t;
      robots[i]->simul_info=&params;   
      robots[i]->load_params(params.files_robot[i]);
      if(robots[i]->manual)
        nbr_pics += 1; 
      else
        nbr_pics += robots[i]->nbr_pics;         
    }
    pthread_barrier_init(&barrier,NULL,nbr_pics+1);      
    for(int i=0;i<params.nbr_robots;i++)
      if(robots[i]->manual)
      {
        add_robot(robots[i],keycontrol_main);
      }
      else
      {
        if(robots[i]->nbr_pics == 1)
          add_robot(robots[i],pic_main1);
        else
          add_robot(robots[i], pic_main1, pic_main2);
      }
  }
  else
    clear_objects();  
     
  add_obstacles(&params);
  
  if(params.simul_3D_view_file[0]!='\0')
  {
    Oz=0.;
    Xaxis.x=1.;  Xaxis.y=0.;
    Yaxis.y=0.;  Yaxis.y=1.;    
    view3D.load_params(params.simul_3D_view_file);
    view3D.create(&Xaxis, &Yaxis, &Oxy, &Oxy, &Oz, 0.); 
    view3D.start();  
  }
}
//----------------------------------------------------------------------------
void simulateur_t::add_object(object_t *obj)
{
  obj->simul_info=&params;
  obj->maj_const_vars();
  params.add_object(obj);
}
//----------------------------------------------------------------------------
void simulateur_t::clear_objects()
{   
  STOP = 1;
  pthread_barrier_wait(&barrier);    
  pthread_barrier_wait(&barrier);    
  while(nbr_pics>0)
  {
    pthread_yield();
  }
  
  for(unsigned int i=0;i<robot_thread_list.size();i++)
  {
    printf("Cancelling robot's thread...    ");
    fflush(stdout);  
    pthread_cancel(robot_thread_list[i]);
    printf("ok\n");
    fflush(stdout);    
  }    
  pthread_barrier_destroy(&barrier);
  for(unsigned int i=0;i<params.objets_fixes.size();i++)
  {
    printf("Deleting object...              ");
    fflush(stdout);   
    delete params.objets_fixes[i];
    printf("ok\n");
    fflush(stdout); 
  } 
  for(unsigned int i=0;i<params.objets_mobiles.size();i++)
  {
    printf("Deleting object...              ");
    fflush(stdout);   
    delete params.objets_mobiles[i];
    printf("ok\n");
    fflush(stdout); 
  }   
  printf("Clearing lists...               ");
  fflush(stdout);   
  robot_list.clear();
  robot_thread_list.clear();
  params.reset();
  view3D.param_list.clear();
  printf("ok\n");
  fflush(stdout);   
}
//----------------------------------------------------------------------------
void simulateur_t::add_robot(robot_t *robot,void* (*func1)(void*), void* (*func2)(void*))
{
  robot->type=OBJ_ROBOT;  
  add_object(robot);
  robot_list.resize(robot_list.size()+1);
  robot_list[robot_list.size()-1]=robot; 

  ROBOT_INFO info;
  info.largeur_ter=pgHeight;
  info.longueur_ter=pgWidth;
  info.dimension_x=robot->dimX;
  info.dimension_y=robot->dimY;
  info.hauteur=robot->hauteur;
  info.roues_x=robot->roues_x;
  info.roues_y=robot->roues_y;       
  info.rayon_roue=robot->rayon_roue; 
  info.pos_init.x=robot->position.x;
  info.pos_init.y=robot->position.y;
  info.vit_init.x=robot->speed.x;
  info.vit_init.y=robot->speed.y;      
  info.angle_init=robot->angle;
  info.vit_angle_init=robot->omega;
  info.masse=robot->masse;
  info.g=params.g;
  info.Umax=robot->tension;
  info.periodes_codeuse=robot->periodes_codeuse;
  info.moteur_L=robot->moteur[0].L;
  info.moteur_R=robot->moteur[0].R;
  info.moteur_Kc=robot->moteur[0].Kc;
  info.moteur_Kv=robot->moteur[0].Kv;
  info.moteur_r=robot->rayon_roue;
  info.moteur_k=robot->moteur[0].k;
  info.moteur_n=robot->moteur[0].n;
  info.moteur_e=params.coeff_frott_roue;
  info.moteur_J=robot->moteur[0].J;  
  
  
  
  PIC_ARG *Args = (PIC_ARG *)malloc(sizeof(PIC_ARG));
  Args->my_id=robot_list.size()-1;
  Args->my_nbr=0;
  Args->barrier=&barrier;
  Args->STOP=&STOP;  
  Args->alive_thread=&nbr_pics;
  Args->robot=robot;
  Args->info=info;

  pthread_t thread1;  
  pthread_create(&thread1, NULL, func1, Args);  
  robot_thread_list.resize(robot_thread_list.size()+1);    
  robot_thread_list[robot_thread_list.size()-1]=thread1;
 
  if(func2)
  {
    PIC_ARG *Args = (PIC_ARG *)malloc(sizeof(PIC_ARG));
    Args->my_id=robot_list.size()-1;
    Args->my_nbr=1;
    Args->barrier=&barrier;
    Args->STOP=&STOP;  
    Args->alive_thread=&nbr_pics;    
    Args->robot=robot;
    Args->info=info;

    pthread_t thread2;  
    pthread_create(&thread2, NULL, func2, Args);  
    robot_thread_list.resize(robot_thread_list.size()+1);    
    robot_thread_list[robot_thread_list.size()-1]=thread2;
  }
}
//----------------------------------------------------------------------------
void simulateur_t::do_step_simul()
{ 
  params.Iterations++; 
  params.robot_proceed=params.nbr_robots; 
  
 /* struct timeval tot,deb;
  gettimeofday(&tot,NULL); 
*/
  //for(unsigned int i=0;i<robot_list.size();i++)
  //  pthread_mutex_lock(&robot_list[i]->mutex);
  
  //gettimeofday(&deb,NULL); 
  for(unsigned int i=0;i<params.objets_mobiles.size();i++)
  {
    params.objets_mobiles[i]->calc_forces();
    params.objets_mobiles[i]->calc_vitesse();
    params.objets_mobiles[i]->calc_position();
  }
  /*gettimeofday(&calct,NULL);
  calct.tv_sec-=deb.tv_sec;
  calct.tv_usec-=deb.tv_usec;*/
  params.counter=0;
//  gettimeofday(&deb,NULL); 
/*  for(unsigned int i=0;i<params.objets_mobiles.size();i++)
    params.objets_mobiles[i]->check_collisions();  
  for(unsigned int i=0;i<params.objets_fixes.size();i++)
    params.objets_fixes[i]->check_collisions();     */
  for(unsigned int i=0;i<params.objets.size();i++)
    if(params.objets[i]->TTL==0)
    {
      params.objets[i]->TTL=_TTL;
      params.objets[i]->check_collisions();      
    }
    else
      params.objets[i]->TTL--;
  params.clean_coll_checked();    
  counter=params.counter;   
  
/*  gettimeofday(&stept,NULL);
  stept.tv_sec-=deb.tv_sec;
  stept.tv_usec-=deb.tv_usec;
*/
  //for(unsigned int i=0;i<robot_list.size();i++)
  //  pthread_mutex_unlock(&robot_list[i]->mutex); 
  
/*  gettimeofday(&totalt,NULL);
  totalt.tv_sec-=tot.tv_sec;
  totalt.tv_usec-=tot.tv_usec; */
}
//----------------------------------------------------------------------------
void simulateur_t::draw_background()
{
  printf("Creating background...          ");
  fflush(stdout);
      
  draw_playground_2D(params.scale);

  printf("ok\n");
  fflush(stdout);                 
}
//----------------------------------------------------------------------------              
void simulateur_t::draw()
{     
  Draw_SDL_Background();                 
  for(unsigned int i=0;i<params.objets_mobiles.size();i++)
    params.objets_mobiles[i]->draw();
  for(unsigned int i=0;i<params.objets_fixes.size();i++)
    if(!params.objets_fixes[i]->hide)
      params.objets_fixes[i]->draw();    
}
//----------------------------------------------------------------------------//

