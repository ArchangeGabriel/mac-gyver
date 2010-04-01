#include "dist_transform.h"

int width, height;
double resol;

//------------------------------------------------------------------------------
dt_map_t* dt_create_map(double terrainWidth, double terrainHeight, double pixResol)
{
  return new dt_map_t(terrainWidth, terrainHeight, pixResol);
}
//------------------------------------------------------------------------------
void dt_drawHline(dt_map_t *map, double x0, double y0, double w)
{
  // TODO
}
//------------------------------------------------------------------------------
void dt_drawVline(dt_map_t *map, double x0, double y0, double h)
{
  // TODO
}
//------------------------------------------------------------------------------
void dt_drawline(dt_map_t *map, double x0, double y0, double x1, double y1)
{
  // TODO
}
//------------------------------------------------------------------------------
void dt_drawBox(dt_map_t *map, double x0, double y0, double w, double h)
{
  // TODO
}
//------------------------------------------------------------------------------
void dt_drawDisc(dt_map_t *map, double x0, double y0, double r)
{
  // TODO
}
//------------------------------------------------------------------------------
dt_distmap_t** dt_compute_distmap(dt_map_t *map, int num_orient)
{
  // TODO
  return NULL;
}
//------------------------------------------------------------------------------
