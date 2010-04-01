#ifndef DIST_TRSFRM_H
#define DIST_TRSFRM_H

#include <string.h>

typedef bool dt_map_pix_t;
class dt_map_t
{
  private:
  int width, height;
  double pixResol;
  dt_map_pix_t* pix;
  
  public:
  dt_map_t(double terrainWidth, double terrainHeight, double resol):pixResol(resol)
  {
    width = terrainWidth / resol;
    height = terrainHeight / resol;
  
    pix = new dt_map_pix_t[width * height];
    memset(pix, true, sizeof(dt_map_pix_t)*width*height);
  }
  
  void free()
  {
    delete[] pix;
  }
};

typedef double dt_distmap_t;

//------------------------------------------------------------------------------
/*
Renvoie une map des distances initialisée à true
- terrainWidth et terrainHeight sont les dimensions du terrain
- pixResol contrôle la distance en mètre qui sépare deux pixels voisins de la map
*/
dt_map_t* dt_create_map(double terrainWidth, double terrainHeight, double pixResol);

//------------------------------------------------------------------------------
/*
Dessine sur la map (toutes les mesures sont en mètres)
*/
void dt_drawHline(dt_map_t *map, double x0, double y0, double w);
void dt_drawVline(dt_map_t *map, double x0, double y0, double h);
void dt_drawline(dt_map_t *map, double x0, double y0, double x1, double y1);
void dt_drawBox(dt_map_t *map, double x0, double y0, double w, double h);
void dt_drawDisc(dt_map_t *map, double x0, double y0, double r);

//------------------------------------------------------------------------------
/*
Calcule les cartes des distances pour 'num_orient' orientations
*/
dt_distmap_t** dt_compute_distmap(dt_map_t *map, int num_orient);
#endif
