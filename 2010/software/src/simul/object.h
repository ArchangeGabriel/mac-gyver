#ifndef OBJECTH
#define OBJECTH

#include <vector>
#include <map>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../common/lin_alg.h"
#include "sdl.h"
#include "parse_params.h"

using namespace std;

#define _TTL 10

//Matières (pour les coefficients de frottement)
enum {mBois, mMetal, NBR_MATIERES};

class object_t;
//----------------------------------------------------------------------------//
//                                                                            //
//            Classes de base pour les paramètres de simulation               //
//                                                                            //
//----------------------------------------------------------------------------//
class simul_info_t : public params_t
{
  public:
  double dt;
  double scale;
  int config_terrain;
  int nbr_obj;
  vector<object_t*> objets;  
  vector<object_t*> objets_fixes;
  vector<object_t*> objets_mobiles;  
  vector<object_t*> palets;  
  vector< vector<int> > coll_checked;
  void *current_webcam; 
  char* simul_3D_view_file;

  double coeff_frott[NBR_MATIERES][NBR_MATIERES];
  double coeff_frott_roue;
  double g;             // constante de champ gravitationnel
  int nbr_robots;
  char **files_robot;
  unsigned long Iterations;      // nbr d'appels à do_simulation
  int robot_proceed;
  int counter;
  
  simul_info_t();
  ~simul_info_t();  
  void init_params();
  void add_palet(object_t *pal);
  void detach_palet(object_t *pal);
  void rattach_palet(object_t *pal);  
  void add_object(object_t *obj);
  void reset();
  void clean_coll_checked(); 
  void clean_coll_of(int i);   
  void set_coll_checked(int i, int j,bool is_coll);  
  void reset_coll_checked(int i, int j);   
  bool is_coll_checked(int i, int j);
};
//----------------------------------------------------------------------------
class force_t
{
  public:
  vector_t C;         //position du point de contact par rapport au centre de gravité
  vector_t F;         //force
};

//----------------------------------------------------------------------------//
//                                                                            //
//                         Classe de base pour les objets                     //
//                                                                            //
//----------------------------------------------------------------------------//

//Type d'objets
enum{OBJ_ROBOT,OBJ_TABLE,OBJ_ELEMENT};

class object_t : public params_t
{
  protected:
  vector<object_t*> in_collision_with;  
  
  public:
  // constantes:
  int my_id;          // numéro de l'objet
  int type;           // type de l'objet
  vector_t G_init;    // position du centre de gravité par rapport au centre du solide
  vector_t G_rot;     // position du centre de gravité par rapport au centre du solide
  double masse;       // masse de l'objet
  double J;           // moment cinétique par rapport à l'axe vertical passant par le centre de gravité
  int matiere;        // une des matières énoncées ci-dessus

  // variables cinématiques:
  bool hide;          // indique que l'objet ne doit pas être dessiné
  bool fixe;          // indique si l'objet est indéplacable (élément du terrain)
  bool linked;        // indique si l'objet est une partie d'un objet plus complexe.
  bool detached;      // indique si l'objet ne doit pas être pris en compte et affiché  
  object_t* linked_obj;// le cas échéant: l'objet complet
  bool inverse;       // direction des normales (intérieur/extérieur)
  vector_t position;  // position de G dans le référentiel général
  double z;           // cote 
  double hauteur;     // hauteur
  vector_t speed;     // vitesse de G dans le référentiel général
  double angle;       // angle de l'objet: rotation de centre G
  double omega;       // = d(angle)/dt
  vector_t last_position;
  double last_angle;   
  // simulation:
  int TTL;                    // nombre d'itération avant la prochaine détection de collision
  Uint32 couleur;             // pour l'affichage: couleur de l'objet
  GLint texture;              // pour l'affichage: numéro de la texture
  simul_info_t *simul_info;   // pointeur vers le simulateur  
  double cosinus,sinus;   
  vector_t N,T;
  vector_t sum_forces;
  double sum_moments;
  
  object_t();  
  virtual ~object_t();
  virtual void init_params();   // appelée par load_params()
  virtual void maj_const_vars();  // appelée par init_params() pour effecuter les calculs dépendant de paramètres constants
  virtual void maj_dyn_vars();    // appelée par calc_position pour effectuer les calculs dépendant de paramètres variables

  // simulation:
  virtual void reset_last_pos(bool link_ignore=false);
  virtual void set_last_pos(bool link_ignore=false);  
  vector_t vitesse();
  void add_force(vector_t &point, vector_t &force);    // ajoute une force à la liste: point = point d'application dans le référentiel barycentrique.
  bool check_collisions();           // effectue une etape de simulation
  virtual void calc_forces();        // calcule la liste des forces s'appliquant sur l'objet
  virtual void calc_frottements();   // calcule la liste des frottements s'appliquant sur l'objet
  void calc_vitesse();       // applique les théorèmes de la cinématique du solide: met à jour vitesse et omega
  void calc_position();      // met à jour position et angle  
  bool check_collision_with(object_t*); // renvoie vrai en cas de collision avec obj, faux en cas de doute
  virtual bool fast_collision_check(object_t*); // renvoie faux en cas de non collision, vrai en cas de doute
  virtual bool slow_collision_check(object_t*); // renvoie vrai en cas de collision avec obj
  bool calc_impulsion(vector_t &N, vector_t &p1, object_t *obj,vector_t &p2);   // en cas de collision: calcule les forces d'impulsion
  virtual double distance_to(vector_t &point, vector_t &dir, double capt_z);  // distance de "point" à l'objet, suivant la direction dir
  virtual void draw();                 // dessine l'objet
  virtual void draw3D();               // dessine l'objet en 3D 

  // pour les collisions
  vector_t get_speed(vector_t &point);     // renvoit la vitesse de 'point' (donné dans le réf. barycentrique) dans le réf. général
  virtual void get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max);  
  virtual bool is_point_in_obj(vector_t&,object_t*&); // renvoit vrai si 'point' (donné dans le réf. barycentrique) est à l'intérieur de l'objet
  virtual double dist_of_obj(vector_t& /*point*/, vector_t& /*u*/,object_t*&); // renvoit la distance de 'point' (donné dans le réf. barycentrique)
                                                                              // à l'objet
                                                                              // u sera initialisé avec la direction de la projection de 'point'
                                                                              // sur l'objet
  virtual vector_t get_normal(vector_t &/*point*/); // normale en un point de l'objet
};
//----------------------------------------------------------------------------//
//                                                                            //
//                                     Pavé                                   //
//                                                                            //
//----------------------------------------------------------------------------//
class ObjBox : public object_t
{
  public:
  ~ObjBox();
  void init_params();
  void maj_const_vars();
  void maj_dyn_vars();  
  
  double dimX,dimY;    // dimensions du rectangle

  vector_t coins[4];                // les coins du rectangle sans rotation
  vector_t coins_rot[4];
  
  bool fast_collision_check(object_t*); // cf class object_t
  bool slow_collision_check(object_t*); // cf class object_t
  void draw();   
  void draw3D();                
  // pour les collisions
  void get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max);  
  bool is_point_in_obj(vector_t &point,object_t*&);  // cf class object_t
  double dist_of_obj(vector_t &point, vector_t &u,object_t*&);  // cf class object_t
  double distance_to(vector_t &point, vector_t &dir, double capt_z);  
  vector_t get_normal(vector_t &point);   // cf class object_t
};

//----------------------------------------------------------------------------//
//                                                                            //
//                                  Cylindre                                  //
//                                                                            //
//----------------------------------------------------------------------------//
class ObjCylinder : public object_t
{
  public:
  ~ObjCylinder();
  void init_params();  
    
  double rayon;        // dimensions du cyclindre

  bool fast_collision_check(object_t*); // cf class object_t
  bool slow_collision_check(object_t*); // cf class object_t
  void draw();   
  void draw3D();             
  // pour les collisions
  void get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max);  
  bool is_point_in_obj(vector_t &point,object_t*&);  // cf class object_t
  double dist_of_obj(vector_t &point, vector_t &u,object_t*&);  // cf class object_t
  double distance_to(vector_t &point, vector_t &dir, double capt_z);  
  vector_t get_normal(vector_t &point);   // cf class object_t
};

//----------------------------------------------------------------------------//
//                                                                            //
//                                   Sphere                                   //
//                                                                            //
//----------------------------------------------------------------------------//
class ObjSphere : public object_t
{
  public:
  ~ObjSphere();
  void init_params();  
    
  double rayon;        // dimensions de la sphère

  bool fast_collision_check(object_t*); // cf class object_t
  bool slow_collision_check(object_t*); // cf class object_t
  void draw();   
  void draw3D();             
  // pour les collisions
  void get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max);  
  bool is_point_in_obj(vector_t &point,object_t*&);  // cf class object_t
  double dist_of_obj(vector_t &point, vector_t &u,object_t*&);  // cf class object_t
  double distance_to(vector_t &point, vector_t &dir, double capt_z);  
  vector_t get_normal(vector_t &point);   // cf class object_t
};


//----------------------------------------------------------------------------//
//                                                                            //
//                              Union de primitives                           //
//                                                                            //
//----------------------------------------------------------------------------//
class union_obj_t : public object_t
{  
  public:
  ~union_obj_t();
  void init_params();
  void maj_const_vars();    
  void maj_dyn_vars();
  void add_object(object_t *obj);
  void attach_object(object_t *obj, vector_t pos);
  void detach_object(object_t *obj);
    
  vector<object_t*> objects;        // liste des objects primitifs
  vector<vector_t> objects_pos;     // positions
  vector<vector_t> objects_last_pos;// last_positions  

  void reset_last_pos(bool link_ignore);           // cf class object_t
  void set_last_pos(bool link_ignore);             // cf class object_t
  bool fast_collision_check(object_t*); // cf class object_t
  bool slow_collision_check(object_t*); // cf class object_t
  void draw();              // cf class object_t
  void draw3D();            // cf class object_t
  // pour les collisions
  void get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max);  
  bool is_point_in_obj(vector_t &point,object_t*&);  // cf class object_t
  double dist_of_obj(vector_t &point, vector_t &u,object_t*&);  // cf class object_t
  double distance_to(vector_t &point, vector_t &dir, double capt_z);  
  vector_t get_normal(vector_t &point);   // cf class object_t
};
#endif
