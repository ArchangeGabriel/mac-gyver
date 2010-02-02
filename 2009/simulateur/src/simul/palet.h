#ifndef PALETH
#define PALETH

#include "sdl.h"
#include "object.h"

enum {RED,GREEN};
enum {stTAKEN,stFREE};

class palet_t : public circle_obj_t
{
  private:
  object_t *bottom;   // objet situé immédiatement en dessous
  double obj_z;       // hauteur max atteinte par les objets en dessous
  
  double z_speed;     // vitesse verticale
  
  void find_bottom(); // calcule bottom
  void calc_vert_move(); // bouge le palet verticalement
  double just_dist_of_obj(object_t *obj); // distance du palet au ième objet
  
  public:
  int color;
  int state;          // tenu ou libre?
  
  palet_t(int c);
  void calc_frottements();  
  void calc_forces();
  void draw();
  void set_free();
  void set_taken();
};



#endif
