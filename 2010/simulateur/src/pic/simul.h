#ifndef SIMULH
#define SIMULH

#include "../simul/robot.h"

typedef struct vect_t
{
  double x,y;
} vect_t;

typedef struct ROBOT_INFO
{

  //Les paramètres qui peuvent être utiles:
  double largeur_ter;   // en mètre: longueur du terrain
  double longueur_ter;  // en mètre: largeur du terrain
  double dimension_x;   // en mètres: longueur (ou largeur)
  double dimension_y;   // en mètres: largeur  (ou longueur)
  double hauteur;       // en mètres: hauteur
  double roues_x;       // en mètres: Abscisse de la droite (fictive) joignant les roues par rapport au centre du robot
  double roues_y;       // en mètres: Ecartement des roues l'une par rapport à l'autre
  double rayon_roue;    // en mètres: rayon des roues
  vect_t pos_init;      // en mètres: position initiale
  vect_t vit_init;      // en m/s: vitesse initiale    
  double angle_init;    // en radians: angle initial
  double vit_angle_init;// en rad/s: vitesse angulaire initiale
  double masse;         // en kg: masse du robot
  double g;             // en m/s²: constante de champ gravitationnel
  double Umax;          // en V:  tension en sortie des batteries
  double periodes_codeuse; // nbr de périodes de codeuse pour un tour d'arbre moteur
  double moteur_L;      // en H: inductance du moteur
  double moteur_R;      // en ohms: résistance
  double moteur_Kc;     // en N/A: constante de couple
  double moteur_Kv;     // en rad/s/V: constante de vitesse
  double moteur_r;      // en mètres: rayon de la roue
  double moteur_k;      // rapport de réduction
  double moteur_n;      // rendement
  double moteur_e;      // coefficient de frottement de la roue sur le sol 
  double moteur_J;      // en kg.m²: moment d'inertie du moteur et de la roue
} ROBOT_INFO;

typedef struct PIC_ARG 
{
  int my_id;
  int my_nbr;
  
  pthread_barrier_t *barrier;
  int *STOP;
  int *alive_thread;

  robot_t *robot;   
  ROBOT_INFO info;
} PIC_ARG;

#endif

