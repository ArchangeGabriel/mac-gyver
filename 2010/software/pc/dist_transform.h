#ifndef DIST_TRSFRM_H
#define DIST_TRSFRM_H

#include <stdint.h>
#include <queue>
#include <vector>
#include <utility>

#include "types.h"

#define DT_SPATIAL_RESOL  0.005    // Résolution métrique pour la carte des distances
#define DT_ANGLE_RESOL 8        // Résolution angulaire pour la carte des distances

using namespace std;

//------------------------------------------------------------------------------

typedef float dt_dist;

//------------------------------------------------------------------------------

class dt_map;

class dt_zone
{
  friend class dt_map;
  
  private:
  void allocate();
  
  protected:
  int height;
  int *left;
  int *right;
  int cy;
  
  public:
  dt_zone(int h);
  dt_zone(int h, int c);  
  ~dt_zone();
  
  dt_zone* operator * (const dt_zone &A);
};

//------------------------------------------------------------------------------

class dt_zone_disc : public dt_zone
{
  public:
  dt_zone_disc(int radius);
};

class dt_zone_hbox : public dt_zone
{
  public:
  dt_zone_hbox(int w, int h);
};

class dt_zone_orientbox : public dt_zone
{
  private:
  void line(int *array, int x1, int y1, int x2, int y2);
  inline void swap_int(int* x, int* y);
  
  public:
  dt_zone_orientbox(int width, int depth, double angle);
};

//------------------------------------------------------------------------------

typedef vector<position_t> dt_path;

//------------------------------------------------------------------------------

class dt_map
{
  private:
  static const dt_dist pix_cost;
  static const double RAD_ANGLE_RESOL;
  static const double RAD_ANGLE_RESOL_2;  
  
  int width, height;
  dt_dist** pix;
  dt_dist** pix_gradX;
  dt_dist** pix_gradY;
  
  dt_zone_orientbox **robot;
  
  typedef struct dt_pix
  {
    int x,y;
    dt_dist dist;
    dt_dist score;
    dt_pix *parent;
    
    dt_pix() {};
    dt_pix(int _x, int _y, dt_dist _dist, dt_dist _score, dt_pix *_parent) : x(_x), y(_y), dist(_dist), score(_score), parent(_parent) {}
    bool operator < (const dt_pix &b) const {return score>b.score;}
  } dt_pix;
  
  // Dessine des formes sur les cartes 
  void fillBox(double x, double y, double w, double h);
  void fillDisc(double cx, double cy, double radius);
  
  // 1-dimensional distance transform
  void distance_transform_1D(dt_dist *src_pix, dt_dist *dest_pix, bool vertical);
  
  // Calcule les dérivés en X et Y de pix
  void compute_gradient();
  
  // Marque l'intérieur de la zone comme infranchissable
  void fillZone(dt_zone *zone, int iAngle, int x, int y);
  
  // Transforme une distance en score entre 0 et 1
  static inline dt_dist score(double distance);

  // Renvoie l'énergie associée au pixel
  dt_dist get_pix_energy(int x, int y, double a);
  
  // Renvoie la dérivée de l'énergie associée a pixel
  void get_pix_energy_gradient(int x, int y, double a, dt_dist &dEdx, dt_dist &dEdy);  
  
  // Estime l'énergie associée au chemin de (x1,y1) à (x2,y2)
  dt_dist estimate_path_energy(int x1, int y1, int x2, int y2);
  
  // Vérifie que la coordonnée est dans le terrain
  static inline int clip(int c, int max);
  
  // Ajoute un pixel à la frontière
  void add_to_boundary(dt_dist *dist, bool *processed, priority_queue<dt_pix> &boundary, priority_queue<dt_pix> &boundary_dont_cross, bool allow_crossing, dt_pix &p, int dx, int dy, int xt, int yt);
  
  // Construit le chemin et l'optimise
  dt_path build_path(dt_pix *current, double initial_angle);
  
  // Optimise le chemin
  void optimize_path(int N, float *X, float *Y);
  void optimize_path_iter(int N, float *A, float *V, float *F);
  
  // Translate a pixel coordinate path into a terrain's position path
  dt_path pix_path2dt_path(int N, float *X, float *Y, double initial_angle);
   
  // Produit un tableau pouvant être envoyé à save_buff_to_bitmap (see common/bitmap.h)
  void to_bitmap(uint16_t* pixbmp, int iAngle, int &w, int &h);
  
  public:    
  // Constructeur / Destructeur
  dt_map(double terrainWidth, double terrainHeight, double robotWidth, double robotDepth);
  ~dt_map();
        
  // Calcule la distance en chaque point de la map
  void compute_distance_transform();
  
  // Remplis les cartes avec les obstacles en prenant en compte la taille du robot  
  void draw_config(int configE, int configI);
    
  // Renvoie un chemin 
  dt_path find_path(const position_t &from, const position_t &to);
  
  // Charge/Sauve dans un fichier
  bool load_from_file(char* file);
  void save_to_file(char* file);  
  
  // Sauve dans un bmp
  void save_to_bmp(char *file);
};

#endif
