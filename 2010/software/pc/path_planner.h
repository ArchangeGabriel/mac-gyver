#ifndef DIST_TRSFRM_H
#define DIST_TRSFRM_H

#include <stdint.h>
#include <queue>
#include <vector>
#include <utility>

#include "types.h"

using namespace std;

//------------------------------------------------------------------------------

typedef float pp_dist;

//------------------------------------------------------------------------------

class pp_map;

class pp_zone
{
  friend class pp_map;
  
  private:
  void allocate();
  
  protected:
  int height;
  int *left;
  int *right;
  int cy;
  
  public:
  pp_zone(int h);
  pp_zone(int h, int c);  
  ~pp_zone();
  
  pp_zone* operator * (const pp_zone &A);
};

//------------------------------------------------------------------------------

class pp_zone_disc : public pp_zone
{
  public:
  pp_zone_disc(int radius);
};

class pp_zone_hbox : public pp_zone
{
  public:
  pp_zone_hbox(int w, int h);
};

class pp_zone_orientbox : public pp_zone
{
  private:
  void line(int *array, int x1, int y1, int x2, int y2);
  inline void swap_int(int* x, int* y);
  
  public:
  pp_zone_orientbox(int width, int depth, double angle);
};

//------------------------------------------------------------------------------

typedef vector<position_t> pp_path;

//------------------------------------------------------------------------------

class pp_map
{
  private:
  static const pp_dist pix_cost;
  static const double RAD_ANGLE_RESOL;
  static const double RAD_ANGLE_RESOL_2;  
  
  int width, height;
  pp_dist** pix;
  pp_dist** pix_gradX;
  pp_dist** pix_gradY;
  
  pp_zone_orientbox **robot;
  
  typedef struct pp_pix
  {
    int x,y;
    pp_dist dist;
    pp_dist score;
    pp_pix *parent;
    pp_pix *angle_next_ref;  // reference for computing angle
    int angle_next_cd;       // pixel to skip before computing angle
    double angle;            // robot's angle at this point
    
    pp_pix() {};
    pp_pix(int _x, int _y, double _angle, int _angle_next_cd) : 
      x(_x), 
      y(_y), 
      dist(0.), 
      score(0.), 
      parent(NULL),
      angle_next_ref(this),
      angle_next_cd(_angle_next_cd),
      angle(_angle)
    {}    
    pp_pix(int _x, int _y, pp_dist _dist, pp_dist _score, pp_pix *_parent) : 
      x(_x), 
      y(_y), 
      dist(_dist), 
      score(_score), 
      parent(_parent)
    {
      if(parent->angle_next_cd == 1)
      {
        vector_t v(x-parent->x, y-parent->y);
        angle_next_ref = this;
        angle_next_cd = parent->angle_next_ref->angle_next_cd;
        angle = v.to_angle();
      }
      else
      {
        angle_next_ref = parent->angle_next_ref;
        angle_next_cd = parent->angle_next_cd - 1;
        angle = parent->angle;   
      }
    }
    bool operator < (const pp_pix &b) const {return score>b.score;}
  } pp_pix;
  
  // Dessine des formes sur les cartes 
  void fillBox(double x, double y, double w, double h);
  void fillDisc(double cx, double cy, double radius);
  
  // 1-dimensional distance transform
  void distance_transform_1D(pp_dist *src_pix, pp_dist *dest_pix, bool vertical);
  
  // Calcule les dérivés en X et Y de pix
  void compute_gradient();
  
  // Marque l'intérieur de la zone comme infranchissable
  void fillZone(pp_zone *zone, int iAngle, int x, int y);
  
  // Transforme une distance en score entre 0 et 1
  static inline pp_dist score(double distance);

  // Renvoie l'énergie associée au pixel
  pp_dist get_pix_energy(int x, int y, double a);
  
  // Renvoie la dérivée de l'énergie associée a pixel
  void get_pix_energy_gradient(int x, int y, double a, pp_dist &dEdx, pp_dist &dEdy);  
  
  // Estime l'énergie associée au chemin de (x1,y1) à (x2,y2)
  pp_dist estimate_path_energy(int x1, int y1, int x2, int y2);
  
  // Vérifie que la coordonnée est dans le terrain
  static inline int clip(int c, int max);
  
  // Ajoute un pixel à la frontière
  void add_to_boundary(pp_dist *dist, bool *processed, priority_queue<pp_pix> &boundary, priority_queue<pp_pix> &boundary_dont_cross, bool allow_crossing, pp_pix &p, int dx, int dy, int xt, int yt);
  
  // Construit le chemin et l'optimise
  pp_path build_path(pp_pix *current, double initial_angle);
  
  // Optimise le chemin
  void optimize_path(int N, float *X, float *Y);
  void optimize_path_iter(int N, const float &gamma, float *A, float *V, float *F);
  
  // Translate a pixel coordinate path into a terrain's position path
  pp_path pix_path2pp_path(int N, float *X, float *Y, double initial_angle);
   
  // Produit un tableau pouvant être envoyé à save_buff_to_bitmap (see common/bitmap.h)
  void to_bitmap(uint16_t* pixbmp, int iAngle, int &w, int &h);
  
  public:    
  // Constructeur / Destructeur
  pp_map(double terrainWidth, double terrainHeight, double robotWidth, double robotDepth);
  ~pp_map();
        
  // Calcule la distance en chaque point de la map
  void compute_distance_transform();
  
  // Remplis les cartes avec les obstacles en prenant en compte la taille du robot  
  void draw_config(int configE, int configI);
    
  // Renvoie un chemin 
  pp_path find_path(const position_t &from, const position_t &to);
  
  // Charge/Sauve dans un fichier
  bool load_from_file(const char* file);
  void save_to_file(const char* file);  
  
  // Sauve dans un bmp
  void save_to_bmp(const char *file);
};

#endif
