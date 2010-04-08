#ifndef DIST_TRSFRM_H
#define DIST_TRSFRM_H

#include <stdint.h>

//------------------------------------------------------------------------------

typedef float dt_map_pix;

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

class dt_map
{
  private:
  static const dt_map_pix contrast;
  
  double pixResol;
  int width, height;
  int nAngle;
  dt_map_pix** pix;
  
  dt_zone_orientbox **robot;
  
  // 1-dimensional distance transform
  void distance_transform_1D(dt_map_pix *src_pix, dt_map_pix *dest_pix, bool vertical);
  
  // Marque l'intérieur de la zone comme infranchissable
  void fillZone(dt_zone *zone, int iAngle, int x, int y);
  
  public:    
  // Constructeur / Destructeur
  dt_map(double terrainWidth, double terrainHeight, double robotWidth, double robotDepth, double resolXY, int resolA);
  ~dt_map();
      
  // Produit un tableau pouvant être envoyé à save_buff_to_bitmap (see common/bitmap.h)
  uint16_t* to_bitmap(int iAngle, int &w, int &h);
  
  // Calcule la distance en chaque point de la map
  void compute_distance_transform();
  
  // Remplis les cartes avec les zones indiqués en prenant en compte la taille du robot
  void fillBox(double x, double y, double w, double h);
  void fillDisc(double cx, double cy, double radius);
};

#endif
