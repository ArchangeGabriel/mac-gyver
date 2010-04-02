#ifndef DIST_TRSFRM_H
#define DIST_TRSFRM_H

#include <stdint.h>

typedef double dt_map_pix_t;

class dt_map_t
{
  private:
  static const dt_map_pix_t contrast;
  
  double pixResol;
  int width, height;
  dt_map_pix_t* pix; 
  
  void swap_int(int* x, int* y);
  
  // Marque un pixel comme infranchissable
  inline void setPix(int x, int y);
  inline dt_map_pix_t getPix(int x, int y);
  
  // Marque une ligne comme infranchissable 
  inline void fastHline(int x1, int y1, int w);  
  
  // 1-dimensionale distance transform
  void distance_transform_1D(dt_map_pix_t *src_pix, dt_map_pix_t *dest_pix, bool vertical);
  
  public:    
  // Constructeur / Destructeur
  dt_map_t(double terrainWidth, double terrainHeight, double resol);
  ~dt_map_t();
  void free();
  
  //Dessine sur la map (toutes les mesures sont en mètres)
  void drawHline(double x1, double y1, double w);
  void drawVline(double x1, double y1, double h);
  void drawline(double x1, double y1, double x2, double y2);
  void drawBox(double x1, double y1, double w, double h);
  void drawDisc(double x1, double y1, double r);  
    
  // Produit un tableau pouvant être envoyé à save_buff_to_bitmap (see common/bitmap.h)
  uint16_t* to_bitmap(int &w, int &h);
  
  // Calcule la distance en chaque point de la map
  void compute_distance_transform();
};

#endif
