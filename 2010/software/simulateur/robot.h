#ifndef ROBOTH
#define ROBOTH
#include <pthread.h>
#include "object.h"
#include "motor.h"
#include "captor.h"
#include "camera.h"

typedef struct PIC_DATA
{
  double posX,posY,angle;
  double destX,destY,destA;
} PIC_DATA;

class robot_t : public union_obj_t
{
  private:
  double coeff_frott_n;   // coefficient de frottement dans le sens des déplacements
  double coeff_frott_t;   // coefficient de frottement pour les déplacements orthogonaux aux roues
  
  public:
  vector<camera_t> webcams;
  
  PIC_DATA *data;  
  
  int manual;
  double dimX,dimY,hauteur;    // dimensions du rectangle contenant le robot  
    
  double capacite_max;    // capacité des batteries chargées
  double capacite;        // capacité des batteries restante
  double tension;         // tension d'alimentation des moteurs 
  double periodes_codeuse;// nbr de périodes par tour d'arbre moteur
  double roues_x;         // Abscisse de la droite (fictive) joignant les roues par rapport au centre du robot
  double roues_y;         // Ecartement des roues l'une par rapport à l'autre
  vector_t roue[2];       // position des roues
  double rayon_roue;      // rayon des roues du robot   

  motor_t moteur[2];
  double tension_moteur[2];
  
  int nbr_color_captors;
  vector<color_captor_t> color_captors;
  int nbr_dist_captors;
  vector<dist_captor_t> dist_captors;
  
  int nbr_pics;
    
  robot_t();
  ~robot_t();
  void init_params();
  static int pic_io(robot_t *robot, int captor_type, int captor_id=0, int value=0);
  void calc_forces();
  void calc_frottements();
  void calc_forces_moteur();
  void additional_op_simul();
  void draw();
};

#endif
