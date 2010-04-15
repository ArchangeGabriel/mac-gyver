#include "main.h"

#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "pc_main.h"
#include "types.h"
#include "picAPI.h"
#include "../common/console.h"
#include "../common/const.h"
#include "../common/comm.h"
#include "picAPI.h"
#include "webcamAPI.h"
#include "cinematik.h"
#include "path_tracker.h"
#include "strategie.h"
#include "cinematik.h"
#include "sdl.h"

#define VERSION     0
#define SUBVERSION  2

void change_dest(position_t *d, int type=0);

#ifdef SIMULATION

extern char *addr;
extern long port;
extern long robot_id;

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  addr = (char*)malloc(sizeof(char)*30);
  strcpy(addr,"127.0.0.1");
  port=11111;
  robot_id=0;
  int i;
    
  while((i = getopt(argc, argv, "a:p:i:rg")) > 0)
    switch(i)
    {
      case 'a':
      free(addr);
      addr=(char*)malloc(sizeof(char)*(strlen(optarg)+1));
      strcpy(addr,optarg);
      break;
      case 'p':
      port = strtol(optarg, NULL, 10);
      break;
      case 'i':
      robot_id = strtol(optarg, NULL, 10);
      break;
      case 'r':
      case 'g':
      break;
      default:
      printf("La syntaxe est la suivante: ./client -a adresse -p port -i robot_id [-r/-g]\n");
      return -1;
    }

  pc_main(argc,argv);
   
  return 0;
}  
//------------------------------------------------------------------------------


#endif


void* console(void*)
{
  char Buff[CONSOLE_MAX_LEN_LINE];
  printf("--- CONSOLE DU CLIENT %d.%d ---\n",VERSION,SUBVERSION);
  printf("Tapez 'help' pour afficher les commandes disponibles\n");
  while(true)
  { 
    printf(">>> ");
    fflush(stdout);  
    read_line(Buff);
    if(Buff[0]=='\0') continue;    
    if(!strcmp(Buff,"help"))
    {
      printf("Commandes:\n");
      printf("- q: pour quitter le simulateur\n");
      printf("- version: affiche la version du simulateur\n");
      printf("- goto(x,y,a): change la destination en (x,y) avec un angle a\n");
      printf("- gotoX(x): change l'abscisse de destination en x\n");     
      printf("- gotoY(y): change l'ordonnée de destination en y\n");   
      printf("- gotoA(a): change l'angle de destination en a\n");  
      printf("- keycontrol: pilote le robot avec le clavier\n");                                              
      printf("- a: changer la constante pour l'asservissement\n");           
      printf("- b: changer la constante pour l'asservissement\n");                 
    }
    else if(!strcmp(Buff,"q"))
    {
      exit(0);
    }
    else if(!strcmp(Buff,"version"))
    {
      printf("Client version %d.%d\n",VERSION,SUBVERSION);
    }
    else if(!strcmp(Buff,"keycontrol"))
    {
      printf("Les flèches pour diriger. Espace pour stopper.\nAppuie sur entrée pour quitter.\n");
    
      struct termios tios;

      tcgetattr(STDIN_FILENO, &tios);
      tcflag_t old_c_lflag;
      // Sauve l'ancien flag "c_lflag"
      old_c_lflag = tios.c_lflag;
  
      // Passe en mode de saisie non canonique
      // (lecture d'1 caractère à la fois par défaut, car MIN=1 et TIME=0) 
      tios.c_lflag &= ~ICANON;
      tios.c_lflag &= ~ECHO;	  
      tcsetattr(STDIN_FILENO, TCSANOW, &tios);
      // UTILISATION DU TERMINAL
      while(true)
      {
	      // Lit un caractère sur l'entrée standard
	      unsigned char c;
	      read(STDIN_FILENO, &c, 1);
	      // Quitte si touche "Entrée"
	      if (c=='\n')
		      break;
		    if(c==' ')
		    {
          picMotorsPower(0., 0.);        
		    }
		    if(c=='8')
          picMotorsPower(1., 1.);        
		    else if(c==27)     // échappement
		    {
		      read(STDIN_FILENO, &c, 1);
		      if(c=='[')
		      {
		        read(STDIN_FILENO, &c, 1);
		        switch(c)
		        {
		          case 'A':
		          //haut
              picMotorsPower(0.7, 0.7);
		          break;
		          case 'B': 
		          //bas
              picMotorsPower(-0.2, -0.2); 					          
		          break;
		          case 'C':
		          //droite
              picMotorsPower(0.3, -0.3); 	
		          break;              				          
		          case 'D':
		          //gauche
              picMotorsPower(-0.5, 0.5); 				          
		          break;              
		        } 
		      }  
		    }
      }
      // Restaure l'ancien flag "c_lflag"
      tios.c_lflag = old_c_lflag;
      tcsetattr(STDIN_FILENO, TCSANOW, &tios);  
    }    
    else if(!memcmp(Buff,"goto(",5))
    {
      double x,y,a;
      char *Ptr1,*Ptr2;
      bool error=false;
      Ptr1=&Buff[5];
      Ptr2=&Buff[strlen(Buff)];
      x=strtod(Ptr1,&Ptr2);
      error=Ptr1==Ptr2;
      if(!error)
      {
        Ptr2++;
        Ptr1=&Buff[strlen(Buff)];
        y=strtod(Ptr2,&Ptr1);
        error=Ptr1==Ptr2;
        if(!error)
        {
          Ptr1++;          
          Ptr2=&Buff[strlen(Buff)];
          a=strtod(Ptr1,&Ptr2);
          error=Ptr1==Ptr2;
          if(!error)
          {                
            position_t dest;        
            dest.x=x;        
            dest.y=y;            
            dest.a=((float)a)*M_PI/180.;
            change_dest(&dest);
            printf("Destination mise à jour\n");              
          }
        }
      }
      if(error)
        printf("Erreur de format, exemple: goto(2.3,1.4,78)\n");
    }  
    else if(!memcmp(Buff,"gotoX(",6) || !memcmp(Buff,"gotoY(",6) || !memcmp(Buff,"gotoA(",6)) 
    {
      double c;
      char *Ptr1,*Ptr2;
      bool error=false;
      Ptr1=&Buff[6];
      Ptr2=&Buff[strlen(Buff)];
      c=strtod(Ptr1,&Ptr2);
      error=Ptr1==Ptr2;
      if(!error)
      {
        position_t dest;
        int type;
        switch(Buff[4])
        {
          printf("Destination mise à jour\n");
          case 'X': dest.x=c; type=1; break;
          case 'Y': dest.y=c; type=2; break;
          case 'A': dest.a=((float)c)*M_PI/180.; type=3; break;        
        }
        change_dest(&dest,type);
      }
      else
      {
        Buff[6]='\0';
        printf("Erreur de format, exemple: %s2.3)\n",Buff);
      }
    }   
    else if(!memcmp(Buff,"jack",4))
    {
      strat_lets_go();        
    }  
    else if(!memcmp(Buff,"speed",5))
    {
      float l = cine_get_speed_wheel(0);
      float r = cine_get_speed_wheel(1);      
      
      printf("s: (%f,%f)\n",l,r);
    }                      
    else    
      printf("Commande inconnue: %s\n",Buff);    
  }
  return NULL;
}
//------------------------------------------------------------------------------
void change_dest(position_t *pos, int type)
{
  position_t p = cine_get_position();
  switch(type)
  {
    case 0:
    p = *pos;
    break;
    case 1:
    p.x = pos->x;
    break;
    case 2:
    p.y = pos->y;
    break;
    case 3:
    p.a = pos->a;
    break;
  }
  pt_go_to(p);
}
//------------------------------------------------------------------------------
void exiting()
{
  reset_console();
}
//------------------------------------------------------------------------------

