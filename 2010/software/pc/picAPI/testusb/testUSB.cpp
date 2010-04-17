// main.cpp //

using namespace std;

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <errno.h>

#include "pic_interface.h"
#include "../../common/console.h"
 
#include <pthread.h>
#include <semaphore.h>

void *console(void *);
void *codeuse(void *);

static sem_t my_mutex;

int set_up_inout;
int set_up_moteur;
int codeusegauche,codeusedroite;
unsigned char sensgauche,sensdroit;

static void sighandler(int sig) {
    shut_usb();
    cerr << "Caught signal " << sig << ".\n";
    exit(-1);
}

int main(int argc, char**argv)
{    
    void* ret;
    pthread_t ConsoleThread;
    pthread_t CodeuseThread;
    int i;
    set_up_inout = 0;
    set_up_moteur = 0;
    i = 1;

    sem_init(&my_mutex, 0, 0);

    /* Catch some signals to properly shut down the hardware */
    signal(SIGHUP, sighandler);
    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGTERM, sighandler);
        
    //if(setup_usb_connexions() == -1) return -1;
    pthread_create(&ConsoleThread, NULL, &console ,NULL);
    pthread_create(&CodeuseThread, NULL, &codeuse ,NULL);

    (void) pthread_join(CodeuseThread,&ret);
    (void) pthread_join(ConsoleThread,&ret);
}

void stop_getting()
{
  sem_trywait(&my_mutex);
}

void start_getting()
{
  int i;
  sem_getvalue(&my_mutex,&i);
  if(i<=0) sem_post(&my_mutex);
}

void* codeuse(void *)
{
    int i;
    while(1)  // ad vitam eternam
    {
        sem_wait(&my_mutex);
        i = get_codeuses(&codeusegauche,&sensgauche,&codeusedroite,&sensdroit);
        start_getting();   // Arrete l'acquisition
        if(i == -1)
        {
          stop_getting();   // Arrete l'acquisition
          printf("Erreur sur les codeuses\n");
        }
    }
}

void* console(void*)
{
  char Buff[128];
  stop_getting();
  printf("--- USBtest ---\n");
  printf("Tapez 'help' pour afficher les commandes disponibles\n");
  while(true)
  { 
    printf(">>> ");
    fflush(stdout);  
    read_line(Buff);
    if(Buff[0]=='\0') continue;    
    if(!strcmp(Buff,"help"))
    {
      printf("Commandes generales :\n");
      printf("- q : pour quitter USBtest\n");
      printf("- setup : initialise les connections usb");
      if(!set_up_inout && !set_up_moteur) printf(" - plus de commandes seront accessibles");
      printf("\n- shut : ferme les connections usb\n");              
      printf("- repare : tente de reparer les connections usb\n\n");
      
      if(set_up_moteur)
      {
        printf("Commandes de la carte moteur :\n");
        printf("- set_speed(v1,v2) : change les vitesses des PWM (1 arriere,128 arret,255 avant)\n");
        printf("- get_codeuses : affiche les valeurs des codeuses\n\n");
      }
      if(set_up_inout)
      {
        printf("Commandes de la carte InOut :\n");
        printf("- get_digital_in : affiche les entrees numeriques\n");
        printf("- init_analog_in(n) : configure n entrees analogiques\n");
        printf("- get_analog_in(n) : affiche les valeurs des entrees analogiques 0 a n-1\n");
        printf("- set_DC_motor(n,s) : fait tourner le moteur n (1 a 4) dans le sens s (-1,0 ou 1)\n");
        printf("- set_servo(n,p) : met le servo n (1 a 4) en position p (0 a 255)\n\n");
      }
    }
    else if(!strcmp(Buff,"q"))
    {
      printf("Shut USB\n");
      shut_usb();
      exit(0);
    }
    else if(!strcmp(Buff,"setup"))
    {
      int a;
      a = setup_usb_connexions();
      set_up_inout = 0;
      set_up_moteur = 0;
      stop_getting();// Verrouille le mutex
      if(a <= -2) printf("Erreur USB : Carte InOut absente\n");
      else
      {
        printf("Carte InOut connectee. Tapez 'help' pour voir les nouvelles commandes accessibles.\n");
        set_up_inout = 1;
      }
      if((a == -1)||(a == -3)) printf("Erreur USB : Carte moteur absente\n");
      else
      {
        printf("Carte moteur connectee. Tapez 'help' pour voir les nouvelles commandes accessibles.\n");
        start_getting(); // deverrouille le mutex
        set_up_moteur = 1;
      }
    }
    else if(!strcmp(Buff,"shut"))
    {
      set_up_inout = 0;
      set_up_moteur = 0;
      stop_getting(); // Verrouille le mutex
      shut_usb();
      printf("Connections USB fermees\n");
    }
    else if(!strcmp(Buff,"repare"))
    {
      int a;
      a = repare_usb();
      set_up_inout = 0;
      set_up_moteur = 0;
      stop_getting(); // Verrouille le mutex
      if(a <= -2) printf("Erreur USB : Carte InOut absente\n");
      else
      {
        printf("Carte InOut connectee\n");
        set_up_inout = 1;
      }
      if((a == -1)||(a == -3)) printf("Erreur USB : Carte moteur absente\n");
      else
      {
        printf("Carte moteur connectee\n");
        start_getting(); // Verrouille le mutex
        set_up_moteur = 1;
      }
    }
    else if((set_up_moteur) && (!memcmp(Buff,"set_speed(",10)))
    {
      char *buffer;
      buffer = &Buff[10];
      unsigned char v1,v2;
      errno = 0;
      v1 = (unsigned char)strtol(buffer,&buffer,10);
      buffer ++;
      v2 = (unsigned char)strtol(buffer,&buffer,10);
      if(errno) printf("Erreur de format. Exemple : set_speed(130,255)\n");
      else if(set_speed(v1,v2) == -1) printf("Erreur USB\n");
      else printf("Nouvelles vitesses : %3d %3d \n",v1,v2);
    }
    else if((set_up_moteur) && (!strcmp(Buff,"get_codeuses")))
    {
      printf("Codeuse droite : %8d Sens : %d\n",codeusedroite,sensdroit/2);
      printf("Codeuse gauche : %8d Sens : %d\n",codeusegauche,sensgauche);
    }
    else if((set_up_inout) && (!strcmp(Buff,"get_digital_in")))
    {
      int c;
      c = get_digital_in();
      if(c == -1) printf("Erreur USB\n");
      else printf("Entrees numeriques : %x \n",(unsigned char) c);
    }
    else if((set_up_inout) && (!memcmp(Buff,"init_analog_in(",15)))
    {
      unsigned char n;
      errno = 0;
      n = (unsigned char)strtol(&Buff[15],NULL, 10);
      if(errno) printf("Erreur de format. Exemple : init_analog_in(4)\n");
      else
      {
        int c;
        c = init_analog_in(n);
        if(c == -1) printf("Erreur USB\n");
        else printf("%3d entrees analogiques configurees\n",c);
      }
    }
    else if((set_up_inout) && (!memcmp(Buff,"get_analog_in(",14)))
    {
      unsigned char n;
      errno = 0;
      n = (unsigned char)strtol(&Buff[14],NULL, 10);
      if(errno) printf("Erreur de format. Exemple : get_analog_in(4)\n");
      else
      {
        int i,c;
        unsigned short an_in[16];
        c = get_analog_in(an_in,n);
        if(c == -1) printf("Erreur USB\n");
        else 
        {
          printf("%3d entrees analogiques acquises :\n",c);
          for(i=0;i<c;i++) printf("AN%2d : %d\n",i,an_in[i]);
        }
      }
    }
    else if((set_up_inout) && (!memcmp(Buff,"set_DC_motor(",13)))
    {
      unsigned char n;
      char s;
      char * buffer;
      buffer = &Buff[13];
      errno = 0;
      n = (unsigned char)strtol(buffer,&buffer,10);
      buffer ++;
      s = (char)strtol(buffer,NULL,10);
      printf("tata : %x\n",(int)s);
      if(errno) printf("Erreur de format. Exemple : set_DC_motor(2,-1)\n");
      else if((!n)||(n>4)) printf("Erreur : moteur inexistant. L'intervalle est 1 a 4.\n");
      else if((s&0xfe)&&(s!=-1)) printf("Erreur : sens invalide. Il faut -1, 0 ou 1.\n");
      else if(set_DC_motor(n,s) == -1) printf("Erreur USB\n");
      else printf("Moteur %d dans le sens %d\n",n,s);
    }
    else if((set_up_inout) && (!memcmp(Buff,"set_servo(",10)))
    {
      unsigned char n,s;
      char * buffer;
      buffer = &Buff[10];
      errno = 0;
      n = (unsigned char)strtol(buffer,&buffer,10);
      buffer ++;
      s = (unsigned char)strtol(buffer,NULL,10);
      if(errno) printf("Erreur de format. Exemple : set_servo(2,143)\n");
      else if((!n)||(n>4)) printf("Erreur : servo inexistant. L'intervalle est 1 a 4.\n");
      else if(set_servo(n,s) == -1) printf("Erreur USB\n");
      else printf("Servo %d en position %3d\n",n,s);
    }
    else printf("Commande inconnue : %s\nTapez 'help' pour l'aide.\n",Buff);
  }
  return NULL;
}
