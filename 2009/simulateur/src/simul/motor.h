#ifndef MOTEURH
#define MOTEURH
#include <math.h>
#include <ncurses.h>
#include "parse_params.h"

class motor_t : public params_t
{
  private:
  double dt_sur_L;
  double Kc_k_n_0_98_2;
  double r_sur_k;
  double J_sur_dt;
  double m_sur_J_plus_m_r_carre;
  double m_g_e;
  double dt_k_sur_J;
  
  public:
  // Variables:
  double a;     // angle du moteur  
  double w;     // Vitesse de rotation du moteur
  double I;     // Courant dans le moteur
  double C;     // Couple fourni

  //Constantes moteur:
  double L;     // Inductance
  double R;     // résistance
  double Kc;    // constante de couple
  double Kv;    // constante de vitesse
  double d;     // pente couple/vitesse

  //Constantes réducteur:
  double k;     // rapport de réduction
  double n;     // rendement
  
  //Constantes simulateur:
  double dt;    // temps de simulation
  double m;     // masse sur la roue
  double r;     // rayon de la roue  
  double g;     // constante de gravitation
  double e;     // coefficient de frottement de la roue sur le sol
  double J;     // inertie du moteur et de la roue
  bool derape;
  
  motor_t();
  void init_params();
  double calc_force(double U, double _V);  // U est la tension appliquée
                                          // V est la vitesse de translation de la roue
                                          // renvoie la norme de la force sur le robot
};

#endif
