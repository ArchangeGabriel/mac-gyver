#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "../pic/simul.h"
#include "../pic/pic_main1.h"
#include "../pic/pic_main2.h"
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
  //On rajoute les obstacles
  rect_obj_t *bord_terrain=new rect_obj_t;
  bord_terrain->type=OBJ_TABLE;
  bord_terrain->fixe=true;
  bord_terrain->inverse=true;
  bord_terrain->dimX=params.longueur_terrain;
  bord_terrain->dimY=params.largeur_terrain; 
  bord_terrain->position.x=params.longueur_terrain/2;
  bord_terrain->position.y=params.largeur_terrain/2;   
  bord_terrain->hauteur=0.07;
  bord_terrain->z=0.07/2;   
  bord_terrain->couleur=getColorSDL(clBlack);
  bord_terrain->matiere=mBois;
  add_object(bord_terrain);

  rect_obj_t *niveau2=new rect_obj_t;
  niveau2->type=OBJ_TABLE;
  niveau2->fixe=true;
  niveau2->dimX=0.6;
  niveau2->dimY=0.1; 
  niveau2->position.x=params.longueur_terrain/2;
  niveau2->position.y=params.largeur_terrain-0.05;   
  niveau2->hauteur=0.03;
  niveau2->z=0.03/2;  
  niveau2->couleur=getColorSDL(clDarkBrown);
  niveau2->matiere=mBois;
  add_object(niveau2);
    
  circle_obj_t *niveau3=new circle_obj_t;
  niveau3->type=OBJ_TABLE;  
  niveau3->fixe=true;
  niveau3->rayon=0.15; 
  niveau3->position.x=params.longueur_terrain/2;
  niveau3->position.y=params.largeur_terrain/2;   
  niveau3->hauteur=0.06;
  niveau3->z=0.06/2;   
  niveau3->couleur=getColorSDL(clDarkBrown);
  niveau3->matiere=mBois;
  add_object(niveau3);

  rect_obj_t *bord1=new rect_obj_t;
  bord1->type=OBJ_TABLE;  
  bord1->fixe=true;
  bord1->dimX=0.022;
  bord1->dimY=0.1; 
  bord1->position.x=0.589;
  bord1->position.y=params.largeur_terrain-0.05;   
  bord1->hauteur=0.07;
  bord1->z=0.07/2;  
  bord1->couleur=getColorSDL(clWhite);
  bord1->matiere=mBois;
  add_object(bord1);
   
  rect_obj_t *bord2=new rect_obj_t;
  bord2->type=OBJ_TABLE;  
  bord2->fixe=true;
  bord2->dimX=0.022;
  bord2->dimY=0.1; 
  bord2->position.x=params.longueur_terrain-0.589;
  bord2->position.y=params.largeur_terrain-0.05;   
  bord2->hauteur=0.07;
  bord2->z=0.07/2;    
  bord2->couleur=getColorSDL(clWhite);
  bord2->matiere=mBois;
  add_object(bord2);
    
  circle_obj_t *distrib1=new circle_obj_t;
  distrib1->type=OBJ_DISTRIB;  
  distrib1->fixe=true;
  distrib1->rayon=0.04; 
  distrib1->position.x=0.289;
  distrib1->position.y=params.largeur_terrain-0.04;   
  distrib1->hauteur=0.195;
  distrib1->z=0.195/2+0.04;   
  distrib1->couleur=getColorSDL(clGray);
  distrib1->matiere=mMetal;
  add_object(distrib1); 
  
  circle_obj_t *distrib2=new circle_obj_t;
  distrib2->type=OBJ_DISTRIB;  
  distrib2->fixe=true;
  distrib2->rayon=0.04; 
  distrib2->position.x=params.longueur_terrain-0.289;
  distrib2->position.y=params.largeur_terrain-0.04;   
  distrib2->hauteur=0.195;
  distrib2->z=0.195/2+0.04;   
  distrib2->couleur=getColorSDL(clGray);
  distrib2->matiere=mMetal;
  add_object(distrib2);   
  
  circle_obj_t *distrib3=new circle_obj_t;
  distrib3->type=OBJ_DISTRIB;    
  distrib3->fixe=true;
  distrib3->rayon=0.04; 
  distrib3->position.x=0.04;
  distrib3->position.y=params.largeur_terrain/2.+(params.config_palets<100?-0.25:+0.25);   
  distrib3->hauteur=0.195;
  distrib3->z=0.195/2+0.04;   
  distrib3->couleur=getColorSDL(clGray);
  distrib3->matiere=mMetal;
  add_object(distrib3);    
  
  circle_obj_t *distrib4=new circle_obj_t;
  distrib4->type=OBJ_DISTRIB;    
  distrib4->fixe=true;
  distrib4->rayon=0.04; 
  distrib4->position.x=params.longueur_terrain-0.04;
  distrib4->position.y=params.largeur_terrain/2.+(params.config_palets<100?-0.25:+0.25);   
  distrib4->hauteur=0.195;
  distrib4->z=0.195/2+0.04;  
  distrib4->couleur=getColorSDL(clGray);
  distrib4->matiere=mMetal;
  add_object(distrib4);    
  
  int config;
  switch(params.config_palets%100)
  {
    case 0: config = 0; break;  
    case 1: config = 07; break;
    case 2: config = 15; break;
    case 3: config = 25; break;
    case 4: config = 45; break;
    case 5: config = 26; break;
    case 6: config = 16; break;
    case 7: config = 34; break;
    case 8: config = 46; break;
    case 9: config = 54; break;
    default: config = 64; break;                                    
  } 
  
  palet_t *palet;
  int config_line[2];
  int X;
  config_line[0]=config%10;
  config_line[1]=config/10;
  for(int Y=0;Y<2;Y++)
  {
    for(X=0;X<3;X++)
    {
      if(config_line[Y]&1)
      {
        palet=new palet_t(GREEN);
        palet->type=OBJ_PALET;  
        palet->position.x=params.longueur_terrain/2.-0.4-X*0.25;
        palet->position.y=params.largeur_terrain/2.+0.125-Y*0.2;  
        palet->hauteur=0.03;
        palet->z=0.015;   
        add_palet(palet);

        palet=new palet_t(GREEN);
        palet->type=OBJ_PALET;  
        palet->position.x=params.longueur_terrain/2.-0.4-X*0.25;
        palet->position.y=params.largeur_terrain/2.+0.125-(3-Y)*0.2;
        palet->hauteur=0.03;
        palet->z=0.015;              
        add_palet(palet);
        
  /*      palet=new palet_t(RED);
        palet->type=OBJ_PALET;  
        palet->position.x=params.longueur_terrain/2.+0.4+X*0.25;
        palet->position.y=params.largeur_terrain/2.+0.125-Y*0.2;
        palet->hauteur=0.03;
        palet->z=0.015;              
        add_palet(palet);

        palet=new palet_t(RED);
        palet->type=OBJ_PALET;  
        palet->position.x=params.longueur_terrain/2.+0.4+X*0.25;
        palet->position.y=params.largeur_terrain/2.+0.125-(3-Y)*0.2;
        palet->hauteur=0.03;
        palet->z=0.015;              
        add_palet(palet);*/        
      }
      config_line[Y]=config_line[Y]>>1;
    }
  }
  
  for(int Z=0; Z<3; Z++)
  {
/*    palet=new palet_t(RED);
    palet->type=OBJ_PALET;  
    palet->position.x=0.04;
    palet->position.y=params.largeur_terrain/2.+(params.config_palets<100?-0.25:+0.25); 
    palet->hauteur=0.03;
    palet->z=0.015 + 0.03*Z;              
    add_palet(palet); 

    palet=new palet_t(RED);
    palet->type=OBJ_PALET;  
    palet->position.x=0.289;
    palet->position.y=params.largeur_terrain-0.04; 
    palet->hauteur=0.03;
    palet->z=0.015 + 0.03*Z;              
    add_palet(palet); */
    
    palet=new palet_t(GREEN);
    palet->type=OBJ_PALET;  
    palet->position.x=params.longueur_terrain-0.04;
    palet->position.y=params.largeur_terrain/2.+(params.config_palets<100?-0.25:+0.25); 
    palet->hauteur=0.03;
    palet->z=0.015 + 0.03*Z;              
    add_palet(palet); 
    
    palet=new palet_t(GREEN);
    palet->type=OBJ_PALET;  
    palet->position.x=params.longueur_terrain-0.289;
    palet->position.y=params.largeur_terrain-0.04; 
    palet->hauteur=0.03;
    palet->z=0.015 + 0.03*Z;              
    add_palet(palet);                 
  }
  
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
void simulateur_t::add_palet(object_t *obj)
{
  obj->simul_info=&params;
  obj->maj_const_vars();
  params.add_palet(obj);
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
  info.largeur_ter=params.largeur_terrain;
  info.longueur_ter=params.longueur_terrain;
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
      
  FillRectSDL(0,0,((int)(params.scale*params.longueur_terrain)),
                  ((int)(params.scale*params.largeur_terrain)),getColorSDL(clGround));  
  FillRectSDL(0,0,((int)(params.scale*0.5)),
                  ((int)(params.scale*0.5)),getColorSDL(clDarkGreen));
  FillRectSDL(((int)(params.scale*(params.longueur_terrain-0.5))),
              0,
              ((int)(params.scale*0.5)),
              ((int)(params.scale*0.5)),getColorSDL(clDarkRed));
  
  FillRectSDL(((int)(params.scale*(params.longueur_terrain/2-0.9))),
              ((int)(params.scale*(params.largeur_terrain-0.1))),
              ((int)(params.scale*1.8)),
              ((int)(params.scale*0.1)),getColorSDL(clBrown));
  FillRectSDL(((int)(params.scale*(params.longueur_terrain/2-0.3))),
              ((int)(params.scale*(params.largeur_terrain-0.1))),
              ((int)(params.scale*0.6)),
              ((int)(params.scale*0.1)),getColorSDL(clDarkBrown));                          
  DisqueSDL(((int)(params.scale*params.longueur_terrain/2)),
            ((int)(params.scale*params.largeur_terrain/2)),
            ((int)(params.scale*0.15)),getColorSDL(clDarkBrown));
            
  FillRectSDL(((int)(params.scale*(0.578))),
              ((int)(params.scale*(params.largeur_terrain-0.1))),
              ((int)(params.scale*0.022)),
              ((int)(params.scale*0.1)),getColorSDL(clWhite));
  FillRectSDL(((int)(params.scale*(params.longueur_terrain-0.6))),
              ((int)(params.scale*(params.largeur_terrain-0.1))),
              ((int)(params.scale*0.022)),
              ((int)(params.scale*0.1)),getColorSDL(clWhite));                                      
  
  DisqueSDL(((int)(params.scale*0.289)),
            ((int)(params.scale*(params.largeur_terrain-0.04))),
            ((int)(params.scale*0.04)),getColorSDL(clGray));
  DisqueSDL(((int)(params.scale*(params.longueur_terrain-0.289))),
            ((int)(params.scale*(params.largeur_terrain-0.04))),
            ((int)(params.scale*0.04)),getColorSDL(clGray));
  DisqueSDL(((int)(params.scale*0.04)),
            ((int)(params.scale*(params.largeur_terrain/2+(params.config_palets<100?-0.25:+0.25)))),
            ((int)(params.scale*0.04)),getColorSDL(clGray));
  DisqueSDL(((int)(params.scale*(params.longueur_terrain-0.04))),
            ((int)(params.scale*(params.largeur_terrain/2+(params.config_palets<100?-0.25:+0.25)))),
            ((int)(params.scale*0.04)),getColorSDL(clGray));                                      

  for(int i=0;i<3;i++)
    for(int j=0;j<4;j++)    
      FillRectSDL(((int)(params.scale*(params.longueur_terrain/2-0.9+0.105-0.0075+i*0.6+j*0.13))),
              ((int)(params.scale*(params.largeur_terrain-0.35))),
              ((int)(params.scale*0.015)),
              ((int)(params.scale*0.25)),getColorSDL(clBlack));

  for(int i=0;i<4;i++)         
    FillRectSDL(((int)(params.scale*(params.longueur_terrain/2-0.6-0.0075+i*0.4))),
                0,
                ((int)(params.scale*0.015)),
                ((int)(params.scale*0.25)),getColorSDL(clBlack)); 
                
  for(int i=0;i<3;i++)              
    for(int j=0;j<4;j++)  
    {
      DisqueSDL(((int)(params.scale*(params.longueur_terrain/2-0.4-i*0.25))),
                ((int)(params.scale*(params.largeur_terrain/2+0.125-j*0.2))),
                ((int)(params.scale*0.005)),getColorSDL(clBlack));  
      DisqueSDL(((int)(params.scale*(params.longueur_terrain/2+0.4+i*0.25))),
                ((int)(params.scale*(params.largeur_terrain/2+0.125-j*0.2))),
                ((int)(params.scale*0.005)),getColorSDL(clBlack)); 
    }                              
  printf("ok\n");
  fflush(stdout);                 
}
//----------------------------------------------------------------------------              
void simulateur_t::draw()
{     
  Draw_SDL_Background();                 
  for(unsigned int i=0;i<params.objets_mobiles.size();i++)
    params.objets_mobiles[i]->draw();
}
//----------------------------------------------------------------------------//

