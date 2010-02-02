#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <ncurses.h>
#include <getopt.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "simulateur.h"
#include "sdl.h"
#include "server.h"
#include "../common/console.h"

#define VERSION      1
#define SUBVERSION   0

// Structure de données et thread pour la simulation
simulateur_t Simul;
pthread_t ServeurThread;
pthread_t ConsoleThread;
extern pthread_mutex_t server_mutex;
void launch_simul();
void cancel_simul();
void simulation();
void* serveur_main(void *Args);
void* console_func(void*);
void read_line(char *buff);
void console();
void monitor(long r);
void exiting();

bool simul_started = false;

// Pour la synchronisation entre la vitesse de simulation et la vitesse réelle
struct timeval Deb;
struct timeval Fin;
struct timeval fps_timer_deb;
struct timeval SDL_refresh;
int fps_count=0;
bool init_timer;

int FPS; // Nombre d'image par secondes

//------------------------------------------------------------------------------
int main(int argc,char **argv)
{
  int i;
  int server_port=11111;

  while((i = getopt(argc, argv, "p:")) > 0)
  {
    switch(i) {
      case 'p':
      server_port = strtol(optarg, NULL, 10);
      break;
    }
  }
  fflush(stdout);
  pthread_mutex_lock(&server_mutex);  
  pthread_create(&ServeurThread, NULL, &serveur_main, &server_port); 
  pthread_mutex_lock(&server_mutex);  // attend que le serveur se soit lancé 
  pthread_mutex_unlock(&server_mutex);
  
  Simul.load_params("params.ini");    
       
  printf("Initializing GLUT...            ");
  fflush(stdout); 
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(100,100);
  glutCreateWindow ("Webcam Output");
  webcam_t::init_scene(&Simul.params);
  glutIdleFunc(simulation);  
  printf("ok\n");  
  fflush(stdout);

  initSDL(((int)(Simul.params.scale*Simul.params.longueur_terrain)),((int)(Simul.params.scale*Simul.params.largeur_terrain)));  
  
  Simul.load_simul();            
              
  printf("Lancement du thread console...  ok\n");
  fflush(stdout); 
  pthread_create(&ConsoleThread, NULL, &console_func ,NULL);          
   
  atexit(exiting);  
    
  glutMainLoop();
}  
//------------------------------------------------------------------------------
void console()
{
  char buff[CONSOLE_MAX_LEN_LINE];
  list<robot_t*> :: iterator iter_robot; 
  scrollok(stdscr,true);
  printf("--- CONSOLE DU SIMULATEUR %d.%d ---\n",VERSION,SUBVERSION);
  printf("Tapez 'help' pour afficher les commandes disponibles\n");
  while(true)
  {
    printf(">>> ");
    fflush(stdout);
    read_line(buff);    
    if(buff[0]=='\0') continue;
    if(!strcmp(buff,"help"))
    {
      printf("Option de la ligne de commandes:\n");
      printf("L'option -p xxxx permet de régler le port utilisé par le serveur (11111 par défaut).\n\n");
      printf("Commandes:\n");
      printf("- q: pour quitter le simulateur\n");
      printf("- version: affiche la version du simulateur\n");
      printf("- fps: affiche le nombre d'images par seconde\n");        
      printf("- mon(r): affiche les variables du robot numéro r (commence à 0)\n");                                  
      printf("- re: recharge les paramètres initiaux\n");        
      printf("- cam(r,i): affiche la ième caméra du robot numéro r (commence à 0)\n");  
      printf("- viewcam: affiche la table en 3D\n");       
    } 
    else if(!strcmp(buff,"q"))
      break;
    else if(!strcmp(buff,"version"))
    {
      printf("Simulateur version %d.%d\n",VERSION,SUBVERSION);
    }
    else if(!strcmp(buff,"re"))
    {
      cancel_simul();
      Simul.load_params("params.ini"); 
      Simul.load_simul();
      launch_simul();
    }
    else if(!memcmp(buff,"mon(",4))
    {
      long r;
      char *Ptr=&buff[strlen(buff)];
      r=strtol(&buff[4],&Ptr,10);
      if(Ptr==&buff[4] || r<0)
        printf("Erreur de format, exemple: mon(0)\n");
      else
      {
        if(((unsigned long)r)>=Simul.robot_list.size())
          printf("Index de robot invalide. Max. index = %d\n",Simul.robot_list.size()-1);
        else
          monitor(r);       
      }
    } 
    else if(!memcmp(buff,"cam(",4))
    {
      unsigned long r,i;
      char *Ptr1,*Ptr2;
      bool error=false;
      Ptr1=&buff[4];
      Ptr2=&buff[strlen(buff)];
      r=strtol(Ptr1,&Ptr2,10);
      error=Ptr1==Ptr2;
      if(!error)
      {
        Ptr2++;
        Ptr1=&buff[strlen(buff)];
        i=strtol(Ptr2,&Ptr1,10);
        error=Ptr1==Ptr2;
        if(!error)
        {
          if(r>=Simul.robot_list.size())
            printf("Index de robot invalide. Max. index = %d\n",Simul.robot_list.size()-1);
          else
            if(i>=Simul.robot_list[r]->webcams.size())
              printf("Index de webcam invalide. Max. index = %d\n",Simul.robot_list[r]->webcams.size()-1);       
            else
              Simul.robot_list[r]->webcams[i].start();
        }
      }
      if(error)
        printf("Erreur de format, exemple: cam(0,1)\n");    
    }  
    else if(!strcmp(buff,"viewcam")) 
    {
      Simul.view3D.start();
    }         
    else if(!strcmp(buff,"fps")) 
    {
      printf("Nombre d'images par seconde: %d\n",FPS); 
    }    
    else
      printf("Commande inconnue.\n"); 
    printf("\n");     
  }
}
//------------------------------------------------------------------------------
void monitor(long r)
{
  int sum;
  list<int> moy;
  list<int> :: iterator iter;
  for(int i=0;i<30;i++)
    moy.push_front(0);
    
  initscr();
  clear();
  printw(" Informations générales:\n");
  printw("--------------------------------------\n");
  printw("position: \n");
  printw("vitesse:  \n");
  printw("angle:    \n");
  printw("vit rot:  \n");
  printw("Upince:   \n");  
  printw("batterie: \n\n");
  mvprintw(2,45,"Iteration n");
  mvprintw(3,45,"FPS:");
  mvprintw(9,0," Informations sur les moteurs:\n");
  printw("--------------------------------------\n");
  printw("Moteur gauche:");
  mvprintw(11,30,"Moteur droit:");
  mvprintw(12,0,"codeuse:   \n");
  printw("vit rot: \n");  
  printw("tension: \n");
  printw("courant: \n");
  printw("couple:  \n");
  printw("derape:  \n");  
  mvprintw(12,30,"codeuse: "); 
  mvprintw(13,30,"vit rot: ");  
  mvprintw(14,30,"tension: "); 
  mvprintw(15,30,"courant: "); 
  mvprintw(16,30,"couple:  ");
  mvprintw(17,30,"derape:  ");

  mvprintw(19,0,"Appuie sur entrée pour sortir.");
  cbreak();
  halfdelay(1);
  noecho();
    
  while(true)
  {
    mvprintw(2,57,"%u",Simul.params.Iterations);
    mvprintw(3,50,"%d",FPS);
    mvprintw(2,10,"(%+.3f,%+.3f) m   ",Simul.robot_list[r]->position.x,Simul.robot_list[r]->position.y);
    mvprintw(3,10,"(%+.3f,%+.3f) m/s   ",Simul.robot_list[r]->speed.x,Simul.robot_list[r]->speed.y);   
    mvprintw(4,10,"%+.3f°   ",Simul.robot_list[r]->angle*180/M_PI);
    mvprintw(5,10,"%+.3f°/s  ",Simul.robot_list[r]->omega*180/M_PI);
    mvprintw(6,10,"%+.3f V  ",Simul.robot_list[r]->objects_pos[3].x);//Simul.robot_list[r]->tension_pince*Simul.robot_list[r]->tension/127.);    
    mvprintw(7,10,"%+.3f%%   ",Simul.robot_list[r]->capacite*100./Simul.robot_list[r]->capacite_max);
    mvprintw(12,10,"%+d   ",((int)(Simul.robot_list[r]->moteur[0].a*Simul.robot_list[r]->periodes_codeuse)));    
    mvprintw(13,10,"%+.3f tr/min   ",Simul.robot_list[r]->moteur[0].w*60./(2.*M_PI));
    mvprintw(14,10,"%+.3f V   ",((double)Simul.robot_list[r]->tension_moteur[0])*Simul.robot_list[r]->tension/127.);        
    mvprintw(15,10,"%+.3f A   ",Simul.robot_list[r]->moteur[0].I);
    mvprintw(16,10,"%+.3f Nm   ",Simul.robot_list[r]->moteur[0].C);            
    mvprintw(17,9," %s   ",Simul.robot_list[r]->moteur[0].derape?"oui":"non");                
    mvprintw(12,40,"%+d   ",((int)(Simul.robot_list[r]->moteur[1].a*Simul.robot_list[r]->periodes_codeuse)));    
    mvprintw(13,40,"%+.3f tr/min   ",Simul.robot_list[r]->moteur[1].w*60./(2.*M_PI));
    mvprintw(14,40,"%+.3f V   ",((double)Simul.robot_list[r]->tension_moteur[1])*Simul.robot_list[r]->tension/128.);       
    mvprintw(15,40,"%+.3f A   ",Simul.robot_list[r]->moteur[1].I);                
    mvprintw(16,40,"%+.3f Nm   ",Simul.robot_list[r]->moteur[1].C);       
    mvprintw(17,39," %s   ",Simul.robot_list[r]->moteur[1].derape?"oui":"non"); 
    move(20,0);
    mvprintw(21,4,"total: %dus        ",Simul.totalt.tv_sec*1000000+Simul.totalt.tv_usec);    
    mvprintw(22,4,"calc: %dus        ",Simul.calct.tv_sec*1000000+Simul.calct.tv_usec);
    moy.push_front(Simul.stept.tv_sec*1000000+Simul.stept.tv_usec);
    moy.pop_back();
    sum=0;
    for(iter=moy.begin();iter!=moy.end();iter++)
      sum+=(*iter);
    sum/=30;
    mvprintw(23,4,"step: %dus        ",sum);
    mvprintw(24,4,"counter: %d        ",Simul.counter);
                     
    refresh();
    if(getch()==10) break;
    pthread_yield();
  } 
  endwin();
}  
//------------------------------------------------------------------------------
void launch_simul()
{
  Simul.draw_background();
  Load_SDL_Background();   
  printf("Starting simulation thread...   ");
  fflush(stdout);  
  Simul.params.Iterations=0;    
  init_timer = true;    
  simul_started = true;       
  glutPostRedisplay();
  printf("ok\n\n");
  fflush(stdout);   
}
//------------------------------------------------------------------------------  
void cancel_simul()
{
  printf("Cancelling simulation...        ");
  fflush(stdout);     
  glutDisplayFunc(webcam_t::no_display);  
  pthread_mutex_lock(&Simul.mutex);
  simul_started = false;   
  pthread_mutex_unlock(&Simul.mutex);  
  printf("ok\n");
  fflush(stdout); 
  Simul.clear_objects();
  Free_SDL_Background();
}
//------------------------------------------------------------------------------  
void simulation()
{
  if(!simul_started) return;
  if(init_timer)
  {
    gettimeofday(&Deb,NULL); 
    gettimeofday(&SDL_refresh,NULL);
    gettimeofday(&fps_timer_deb,NULL);  
    init_timer = false;
  }
  
  gettimeofday(&Fin,NULL);    
  if(Fin.tv_sec-fps_timer_deb.tv_sec>=1)
  {
    gettimeofday(&fps_timer_deb,NULL);
    FPS=fps_count;
    fps_count=0;
  }
  double TempsReel = ((double)(Fin.tv_sec-Deb.tv_sec))+(((double)(Fin.tv_usec-Deb.tv_usec)))*0.000001;
  double Refresh   = ((double)(Fin.tv_sec-SDL_refresh.tv_sec))+(((double)(Fin.tv_usec-SDL_refresh.tv_usec)))*0.000001;
  double TempsSimul= Simul.params.Iterations*Simul.params.dt;
  int IterPerFrame=((int)((TempsReel-TempsSimul)/Simul.params.dt));
  
  if(IterPerFrame<0) IterPerFrame=0;
   
  pthread_mutex_lock(&Simul.mutex);  
  int i;
  for(i=0;i<IterPerFrame;i++)
  {
    pthread_barrier_wait(&Simul.barrier);    
    Simul.do_step_simul(); 
    pthread_barrier_wait(&Simul.barrier);    
  }      
  Simul.draw();
  glutPostRedisplay();
  if(Refresh>0.04)
  {
    gettimeofday(&SDL_refresh,NULL);  
    fps_count++;         
    RefreshSDL();    
  }
  pthread_mutex_unlock(&Simul.mutex);      
}
//------------------------------------------------------------------------------
void* serveur_main(void *Args)
{
  int port = *((int*)Args);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  
  start_server(port);
  return NULL;
}
//------------------------------------------------------------------------------
void* console_func(void*)
{
  launch_simul(); 
  console();   
  cancel_simul();
  
  printf("Stopping serveur...             ");
  fflush(stdout); 
  close_connections();  
  pthread_cancel(ServeurThread);
  printf("ok\n");
  fflush(stdout);     
    
  exit(0);
  
  return NULL;
}
//------------------------------------------------------------------------------
void exiting()
{
  reset_console();
  SDL_Quit();
  printf("Leaving...\n");
  fflush(stdout);    
}
//------------------------------------------------------------------------------
