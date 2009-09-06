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

// Couleur du robot
int color;

// Pour la configuration
int cd[2] = {0,0};
int cp[10] = {0,0,0,0,0,0,0,0,0,0};
int config_palets = -1;
int config_distrib = -1;
int configs [] = {07,15,25,45,26,16,34,46,54,64};

// Gestion du temps
bool started;
bool game_over;
struct timeval start_time;

// Recallage
bool recallage_necessaire;

// Collecte
int nbr_el_collected;
vector< vector_t > palets;     // Palets repérés par la webcam
int distribFond, distribCote;
int recolte_initiale;          // On essaie de récolter les palets par terre
void strat_initial_recolte();

float
  XGAUCHE = _LONGUEUR_TER/2. - (0.4+0.50),
  XMILIEU = _LONGUEUR_TER/2. - (0.4+0.25),
  XDROITE = _LONGUEUR_TER/2. - (0.4+0.00),
  YHAUT   = _LARGEUR_TER/2. + 0.125-0.6,
  YMILHAUT= _LARGEUR_TER/2. + 0.125-0.4,
  YMILBAS = _LARGEUR_TER/2. + 0.125-0.2,
  YBAS    = _LARGEUR_TER/2. + 0.125-0.0
;

// Depose
vector< vector_t > colonnes;


// Attend qu'un mutex se libère et le détruit
void wait_for_it(pthread_mutex_t *mutex);

// Collecte un palet (suppose que la pince est fermée et levée
void collect_palet();

// Prend une position pour le vert et renvoit la position adaptée à la couleur
position_t symetrize(position_t pos);

// Test protocole
void test_proto();

// Test homologation
void test_homolo1();
void test_homolo2();

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
  nbr_el_collected = 0;
  palets.clear();
  distribFond = 5;
  distribCote = 5;
  
  fprintf(stderr,"Waiting for jack...\n");  fflush(stdout);
  while(!started)
  {
    int config = wc_reco_config();
    if(config != -1)
    {
      cd[config/100]++;
      cp[config%100]++;
    }
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
int strat_config_distrib()
{
  return config_distrib;
}
//------------------------------------------------------------------------------
int strat_config_palets()
{
  return config_palets;
}
//------------------------------------------------------------------------------
void strat_set_config_distrib(int c)
{
  config_distrib = c;
}
//------------------------------------------------------------------------------
void strat_set_config_palets(int c)
{
  config_palets = c;
}
//------------------------------------------------------------------------------
void strat_lets_go()
{
  gettimeofday(&start_time,NULL); 
  
  // Calcul la configuration la plus probable
       if(cd[0]>cd[1]) config_distrib = 0;
  else if(cd[0]<cd[1]) config_distrib = 1;
  else config_distrib = -1;
  
  int max = -1;
  for(int i = 0; i<10; i++)
  {
    if(cp[i]>max) {config_palets = i; max = cp[i];}
    else if(cp[i] == max) {config_palets = -1;}
  }
  
  if(config_palets == -1)
    recolte_initiale = false;
  else
    recolte_initiale = true;  
    
  int config_line[2];
  if(config_palets != -1)
  {
    config_line[0]=config_palets%10;
    config_line[1]=config_palets/10;
    for(int Y=0;Y<2;Y++)
    {
      for(int X=0;X<3;X++)
      {
        if(config_line[Y]&1)
        {
          palets.resize(palets.size()+2);
          palets[palets.size()-1] = vector_t(_LONGUEUR_TER/2. + (0.4 + X*0.25) * ((color == clGREEN)?-1.:1.),
                                             _LARGEUR_TER/2. + 0.125 - Y*0.2);
                                                     
          palets[palets.size()-2] = vector_t(_LONGUEUR_TER/2. + (0.4 + X*0.25) * ((color == clGREEN)?-1.:1.),
                                             _LARGEUR_TER/2. + 0.125 - (3-Y)*0.2);
        }
        config_line[Y]=config_line[Y]>>1;      
      }
    }
  }
  
  started = true;
}
//------------------------------------------------------------------------------
void stratMainLoop()
{
  fprintf(stderr,"Let's go!\n");  fflush(stdout);
  while(true) {}
  
  pthread_mutex_t *mutPos;    
  mutPos = pp_go_to(symetrize(position_t(0.6,0.35,M_PI_2)),tpDEST,false,false,false);
  wait_for_it(mutPos);
  mutPos = pp_go_to(symetrize(position_t(0.6,1.25,M_PI_2)),tpDEST,false,false,false);    
  wait_for_it(mutPos);
  mutPos = pp_go_to(symetrize(position_t(0.85,1.7,M_PI_2)),tpDEST,false,false,false);    
  wait_for_it(mutPos);  
  mutPos = pp_go_to(symetrize(position_t(0.85,1.87,M_PI_2)),tpDEST,false,false,false); 
  wait_for_it(mutPos);  
  mutPos = pp_go_to(symetrize(position_t(0.85,1.7,M_PI)),tpDEST,false,false,true); 
  wait_for_it(mutPos); 
  mutPos = pp_go_to(symetrize(position_t(0.4,1.7,-M_PI_2)),tpDEST,false,false,true); 
  wait_for_it(mutPos);   
  mutPos = pp_go_to(symetrize(position_t(0.6,0.25,0)),tpDEST,false,false,false); 
  wait_for_it(mutPos);    
  mutPos = pp_go_to(symetrize(position_t(0.85,0.3,M_PI_2)),tpDEST,false,false,false); 
  wait_for_it(mutPos); 
  mutPos = pp_go_to(symetrize(position_t(0.85,1.87,M_PI_2)),tpDEST,false,false,false); 
  wait_for_it(mutPos);  
  mutPos = pp_go_to(symetrize(position_t(0.85,0.25,M_PI_2)),tpDEST,false,false,true);  
  wait_for_it(mutPos);       
  mutPos = pp_go_to(symetrize(position_t(1.1,0.25,M_PI_2)),tpDEST,false,false,false);  
  wait_for_it(mutPos);       
  mutPos = pp_go_to(symetrize(position_t(0.90,1.87,M_PI_2)),tpDEST,false,false,false); 
  wait_for_it(mutPos); 
  mutPos = pp_go_to(symetrize(position_t(0.90,1.7,M_PI_2)),tpDEST,false,false,true); 
  wait_for_it(mutPos);   
//  pp_go_to_distrib(distribGrandCote);
//  pp_go_to_distrib(distribPetitCoteDepot);  
//  pp_go_to(position_t(1,1,-M_PI/4.*3.), tpDEST, false, false);
//  pp_go_to(position_t(0.5,0.5,-M_PI/4.*3.), tpDEST, true, false);  
//  cine_motors(-0.2,0.2);
//  test_homolo1();
  
//  test_proto();
    
 /* while(true)
  {     
    while(strat_collecte() == -1)
      if(recallage_necessaire)
        strat_recallage();
    
    while(strat_depose() == -1)
      if(recallage_necessaire)
        strat_recallage();
  } */
   
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
  picMotorBeltPower(beltStop);
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
void collect_palet()
{
  pthread_mutex_t *mutV;
  pthread_mutex_t *mutClamp;
      
  // On descend un peu
  mutV = picClampVPos(31);  
  wait_for_it(mutV);
  // On ouvre la pince
  mutClamp = picClampState(stClampOpen);
  wait_for_it(mutClamp);
  // On descend
  mutV = picClampVPos(0);  
  wait_for_it(mutV);
  // On ferme la pince
  mutClamp = picClampState(stClampClosed);
  wait_for_it(mutClamp);
  // On remonte
  mutV = picClampVPos(40);  
  wait_for_it(mutV);  
}
//------------------------------------------------------------------------------
position_t symetrize(position_t pos)
{
  if(color == clRED)
  {
    pos.x = _LONGUEUR_TER - pos.x;
    pos.a = M_PI - pos.a;
    while(pos.a>M_PI) pos.a -= 2.*M_PI;
  }
  return pos;  
}
//------------------------------------------------------------------------------
int strat_collecte()
{
  if(nbr_el_collected == 4)
  {
    // on éjecte tout ce qui se présente
    picMotorBeltPower(beltOutside);
    return 0;
  }
  if(recolte_initiale && config_distrib != -1)
  {
    strat_initial_recolte();
    return -1;
  }
  else
  {  
    pthread_mutex_t *mutClamp;
    pthread_mutex_t *mutV;      
    pthread_mutex_t *mutH;
    pthread_mutex_t *mutPos;      
    pthread_mutex_t *mutPos2;    

    // Active courroie et monte la pince
    mutClamp = picClampState(stClampClosed);    
    picMotorBeltPower(beltOutside);    
    mutV = picClampVPos(35);
    mutH = picClampHPos(0);   
    
    if(config_distrib == -1)
    {
    }
    else if(distribFond < distribCote)
    {
      if(config_distrib == 0)
        mutPos = pp_go_to_distrib(distribPetitCoteRobot, tpDEST, false, true, true, &mutPos2);
      else
        mutPos = pp_go_to_distrib(distribPetitCoteDepot, tpDEST, false, true, true, &mutPos2);          
    }
    else
    {
      mutPos = pp_go_to_distrib(distribGrandCote, tpDEST, false, true, true, &mutPos2);
    }

    wait_for_it(mutClamp);
    wait_for_it(mutV);    
    wait_for_it(mutH);     
    wait_for_it(mutPos2);    
    picMotorBeltPower(beltInside); 
    wait_for_it(mutPos);    
    
    //nbr_el_collected++;
    return -1;      
  }
  return 0;
}
//------------------------------------------------------------------------------
int strat_depose()
{
  pthread_mutex_t *mutPos;
  pthread_mutex_t *mutV;
  pthread_mutex_t *mutH;
  pthread_mutex_t *mutClamp;   

  mutPos = pp_go_to_drop(2, 0, vector_t(1.5,1.0));
  mutV = picClampVPos(70); 
  mutH = picClampHPos(posClampMiddle);

  // on attent qu'on soit à destination
  wait_for_it(mutPos);
  wait_for_it(mutV);  
  wait_for_it(mutH);  

  // On sort la pince  
  mutH = picClampHPos(posClampFront);
  wait_for_it(mutH);
  
  // On descend un peu les palets
  mutV = picClampVPos(62);    
  wait_for_it(mutV); 
  
  // On ouvre la pince
  mutClamp = picClampState(stClampOpen);
  wait_for_it(mutClamp);   

  // On rentre la pince  
  mutH = picClampHPos(posClampMiddle);
  wait_for_it(mutH);
  
  nbr_el_collected = 0;
 
  // FIXME
  //Faudrait se rappeller de l'endroit où on dépose les colonnes pour pouvoir les agrandir
  //Et décider d'une politique de dépot (est-ce que déposer au centre est toujours le mieux?
  //N'est-il pas plus rigolo de déposer sur les colonnes adverses pour faire buger le robot d'en face?
        
  return 0;
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
void strat_initial_recolte()
{
  pthread_mutex_t *mutV;
  pthread_mutex_t *mutH;  
  pthread_mutex_t *mutClamp;   
  pthread_mutex_t *mutPosWP,*mutPos;      
  position_t pos;

  // Active courroie et monte la pince
  mutClamp = picClampState(stClampOpen);    
  wait_for_it(mutClamp);
  picMotorBeltPower(beltInside);    
  mutV = picClampVPos(40);
  mutH = picClampHPos(posClampBack);  
  wait_for_it(mutH);
  wait_for_it(mutV);  

  switch(config_palets+1)
  {
   case 1:   // 1/12
      pos = symetrize(position_t(0.25,_LARGEUR_TER/2.+0.125-0.6-0.02,0.));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);  
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, 0.));    
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XMILIEU-0.12, YHAUT,0.));
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);  //BLOQUEUR
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XDROITE-0.12, YHAUT,0.));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();  // BLOQUEUR
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XDROITE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);
      collect_palet();
      // Repositionnement sur la table pour éviter les perturbations sur les autres palets???
      break;

    case 2:  // 2/12
      pos = symetrize(position_t(XDROITE, YHAUT-0.42 , M_PI_2));    //init
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);  
      
      pos = symetrize(position_t(XDROITE, YHAUT, M_PI_2));    
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XDROITE, YMILHAUT, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XDROITE, YMILBAS, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XDROITE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      break;
      
      
   case 3: // 3/12
/*      pos = symetrize(position_t(XGAUCHE, YHAUT-0.42, M_PI_2)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
*/
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, M_PI_2/2));  
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XMILIEU-0.05, YMILHAUT-0.13, M_PI_2));   
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      collect_palet();
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XMILIEU, YMILBAS, 3*M_PI_2/2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      
      pos = symetrize(position_t(XGAUCHE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Repositionner pour éviter de dégommer tous les palets à terre?
      break;
      

   case 4: // 4/12
      pos = symetrize(position_t(XGAUCHE, YHAUT-0.42, M_PI_2)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, M_PI_2));  
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XGAUCHE, YMILHAUT, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XGAUCHE, YMILBAS, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XGAUCHE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Repositionner pour éviter de dégommer tous les palets à terre?
      break;



    case 5: // 5/12
      pos = symetrize(position_t(XMILIEU-0.02, YHAUT-0.42, M_PI_2)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XMILIEU, YHAUT-0.2, M_PI_2));
    mutPos = pp_go_to(pos, tpWAYPOINT, false, false, false);
      wait_for_it(mutPos);
      
      
      pos = symetrize(position_t(XMILIEU, YHAUT, M_PI_2));  
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XMILIEU, YMILHAUT, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XMILIEU, YMILBAS, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XMILIEU, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Un poil lent pour se repositionner avant de poser des palets: par acoups
      break;
      
            
    case 6: // 6/12
      pos = symetrize(position_t(XGAUCHE-0.35, YHAUT, 0.)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, 0.));    
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XMILIEU, YHAUT,0.));
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);  //BLOQUEUR
      collect_palet();
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XDROITE, YHAUT-0.12, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XDROITE, YMILHAUT, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      pos = symetrize(position_t(XDROITE, YMILBAS-0.1, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);
      collect_palet();
      break;
      
    case 7: // 7/12
      pos = symetrize(position_t(XGAUCHE, YHAUT, M_PI_2/2));  
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XMILIEU-0.05, YMILHAUT-0.13, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      collect_palet();
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XMILIEU, YMILBAS, 3*M_PI_2/2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      
      pos = symetrize(position_t(XGAUCHE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Repositionner pour éviter de dégommer tous les palets à terre?
      break;
      
      
      
      
      
    case 8: // 8/12
      pos = symetrize(position_t(XGAUCHE, YHAUT-0.35, M_PI_2)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, M_PI_2));  
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XGAUCHE, YMILHAUT, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XGAUCHE, YMILBAS, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XGAUCHE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Bouge des palets à la fin
      break;


    case 9: // 9/12
      pos = symetrize(position_t(XGAUCHE, YHAUT-0.35, M_PI_2)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, M_PI_2));  
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XGAUCHE, YMILHAUT, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XGAUCHE, YMILBAS, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XGAUCHE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Attention encore: degage des palets
      break;


    default: // 10/12
      pos = symetrize(position_t(XGAUCHE, YHAUT-0.35, M_PI_2)); // Positionnement initial
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      wait_for_it(mutPos);
      
      pos = symetrize(position_t(XGAUCHE, YHAUT, M_PI_2));  
      mutPosWP = pp_go_to(pos, tpWAYPOINT, false, false, false);    
      pos = symetrize(position_t(XGAUCHE, YMILHAUT, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, true, false, false);
      wait_for_it(mutPosWP);
      collect_palet();
      wait_for_it(mutPos); 
      pos = symetrize(position_t(XGAUCHE, YMILBAS, M_PI_2));    
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      pos = symetrize(position_t(XGAUCHE, YBAS, M_PI_2));
      mutPos = pp_go_to(pos, tpDEST, false, false, false);
      collect_palet();
      wait_for_it(mutPos);     
      collect_palet();
      // Atention: bouge legerement un des palets restants
      break;
  }

  nbr_el_collected = 4; 
  recolte_initiale = false;
}
//------------------------------------------------------------------------------
void test_homolo1()
{
  printf("HOMOL\n"); fflush(stdout);
  position_t pos;
  pthread_mutex_t *mutPos;   
  pthread_mutex_t *mutPosWP;
  /*
  //HOMOLOGATION
  pos = symetrize(position_t(XGAUCHE, YHAUT - 0.35 , M_PI_2));    //init
  mutPos = pp_go_to(pos, tpDEST, false, false, false);
  wait_for_it(mutPos);

  pos = symetrize(position_t(XGAUCHE, YHAUT-0.2 , M_PI_2));
  mutPos = pp_go_to(pos, tpDEST, false, false, false);
  wait_for_it(mutPos);

  pos = symetrize(position_t(XGAUCHE/2, YMILHAUT , M_PI_2));    //init
  mutPosWP = pp_go_to(pos, tpDEST, false, false, false);
  pos = symetrize(position_t(XGAUCHE/2, YMILBAS , M_PI_2));    //init
  mutPos = pp_go_to(pos, tpDEST, true, false, false);
  wait_for_it(mutPosWP);

  pos = symetrize(position_t(0.8, _LARGEUR_TER-0.6 , M_PI_2));
  mutPos = pp_go_to(pos, tpDEST, true, false, false);
  wait_for_it(mutPos);

  pos = symetrize(position_t(0.8, _LARGEUR_TER-0.27 , M_PI_2));
  mutPos = pp_go_to(pos, tpDEST, false, false, false); 
  wait_for_it(mutPos);
  // picMotorBeltPower(1);
  pos = symetrize(position_t(0.8, _LARGEUR_TER-0.6 , M_PI_2));
  mutPos = pp_go_to(pos, tpDEST, false, false, false); 
  wait_for_it(mutPos);
    */
  //TOURNER AUTOUR DU POT POUR TESTER L'EVITEMENT
  while(1){
    pos = symetrize(position_t(0.4, 0.5,     0));mutPos = pp_go_to(pos, tpDEST, false, false, false);  wait_for_it(mutPos);
    pos = symetrize(position_t(2.2, 0.5,M_PI_2));mutPos = pp_go_to(pos, tpDEST, false, false, false);  wait_for_it(mutPos);
    pos = symetrize(position_t(2.2, 1.5, M_PI ));mutPos = pp_go_to(pos, tpDEST, false, false, false);  wait_for_it(mutPos);
    pos = symetrize(position_t(0.7,1.5,-M_PI_2));mutPos = pp_go_to(pos, tpDEST, false, false, false);  wait_for_it(mutPos);
  }
}
//------------------------------------------------------------------------------
void test_homolo2()
{ 
  printf("HOMOL\n"); fflush(stdout);
  position_t pos;
  pthread_mutex_t *mutPos;   
  
  pos = symetrize(position_t(XGAUCHE, YBAS , M_PI_2));    //init
  mutPos = pp_go_to(pos, tpDEST, false, false, false);
  wait_for_it(mutPos);

  pos = symetrize(position_t(0.8, _LARGEUR_TER -0.50 , M_PI_2/2 ));
  mutPos = pp_go_to(pos, tpWAYPOINT, false, false, false);
  wait_for_it(mutPos);
  
  pos = symetrize(position_t(0.8, _LARGEUR_TER -0.35 , M_PI_2 ));
  mutPos = pp_go_to(pos, tpWAYPOINT, false, false, false);
  wait_for_it(mutPos);

  pos = symetrize(position_t(0.8, _LARGEUR_TER -0.2 , M_PI_2)); /* -0.27 avec courroie */
  mutPos = pp_go_to(pos, tpWAYPOINT, false, false, false); 
  wait_for_it(mutPos);
  // picMotorBeltPower(1); la courroie n'est pas dispo !!!! 
  pos = symetrize(position_t(0.85, _LARGEUR_TER-0.45 , M_PI_2));
  mutPos = pp_go_to(pos, tpDEST, false, false, false); 
  wait_for_it(mutPos);
  
  while(true)
  {
    usleep(1000);
  }
}
//------------------------------------------------------------------------------  
void test_proto()
{
  pthread_mutex_t *smut;
  int result;
  //roues
/*  printf("Motor forward...\n");  fflush(stdout);  
  result = picMotorsPower(1., 1.);
  if(result != 0)
    printf("couille : moteur avance\n");
  usleep(1000000);*/
/*  result = picMotorsPower(-0.5, -0.5);
  if(result != 0)
    printf("couille : moteur recule\n");
  usleep(500000);
  result = picMotorsPower(0, 0);
  if(result != 0)
    printf("couille : moteur stoppé\n");
  usleep(500000);*/
  //courroie
/*  result = picMotorBeltPower(beltInside);
  if(result != 0)
    printf("couille : courroie avance\n");
  usleep(10000000);
  result = picMotorBeltPower(beltOutside);
  if(result != 0)
    printf("couille : courroie recule\n");
  usleep(10000000);
  result = picMotorBeltPower(beltStop);
  if(result != 0)
    printf("couille : courroie stoppée\n");*/
  //pince derriere/milieu/devant
 /* smut = picClampHPos(posClampBack);
  if(smut == NULL)
    printf("couille : pince devant\n");
  wait_for_it(smut);
 
 smut = picClampHPos(posClampFront);
  if(smut == NULL)
    printf("couille : pince derriere\n");
  wait_for_it(smut);
  smut = picClampHPos(posClampMiddle);
  if(smut == NULL)
    printf("couille : pince milieu\n");
  wait_for_it(smut);*/

  //pince haut/bas
  smut = picClampVPos(100);
  if(smut == NULL)
    printf("couille : pince haut\n");
  wait_for_it(smut);
  smut = picClampVPos(0);
  if(smut == NULL)
    printf("couille : pince bas\n");
  wait_for_it(smut); 
  //pince moi si je rêve
/*  smut = picClampState(stClampLintel);
  if(smut == NULL)
    printf("couille : pince ouverte\n");
  wait_for_it(smut);
  smut = picClampState(stClampOpen);
  if(smut == NULL)
    printf("couille : pince ouverte\n");
  wait_for_it(smut);
  smut = picClampState(stClampClosed);
  if(smut == NULL)
    printf("couille : pince pince\n");
  wait_for_it(smut);
  fflush(stdout);*/
}
//------------------------------------------------------------------------------
