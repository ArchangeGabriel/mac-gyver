#include <stdlib.h>
#include <iostream>
#include <limits>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "dist_transform.h"

const dt_map_pix_t dt_map_t::contrast = 5.;

//------------------------------------------------------------------------------
dt_map_t::dt_map_t(double terrainWidth, double terrainHeight, double resol):pixResol(resol)
{
  width = terrainWidth / resol;
  height = terrainHeight / resol;

  pix = new dt_map_pix_t[width * height];
  for(int i=0; i<width*height; i++)
    pix[i] = std::numeric_limits<dt_map_pix_t>::infinity();
}
//------------------------------------------------------------------------------
dt_map_t::~dt_map_t()
{
  free();
}
//------------------------------------------------------------------------------
void dt_map_t::free()
{
  delete[] pix;
}
//------------------------------------------------------------------------------
void dt_map_t::setPix(int x, int y)
{
  if(x>=0 && y>=0 && x<width && y<height)
    pix[y*width + x] = 0.;
}
//------------------------------------------------------------------------------
dt_map_pix_t dt_map_t::getPix(int x, int y)
{
  if(x>=0 && y>=0 && x<width && y<height)
    return pix[y*width + x];
  else
    return 0.;    
}
//------------------------------------------------------------------------------
void dt_map_t::fastHline(int x1, int y1, int w)
{
  if(x1 < 0) x1 = 0;
  if(x1+w > width) w = width - x1;
  memset(&pix[y1*width + x1], 0., sizeof(dt_map_pix_t)*w);
}
//------------------------------------------------------------------------------
uint16_t* dt_map_t::to_bitmap(int &w, int &h)
{
  w = width;
  h = height;
  uint16_t *pixbmp = new uint16_t[3*width*height];
  
  for(int y=height-1;y>=0; y--)
    for(int x=0;x<width; x++)
    {
      int c = (1. - (1. / (1. + contrast*pix[y*width+x]))) * 255.;
      int i = 3*((height-1-y)*width+x);
      pixbmp[i+0] = c;
      pixbmp[i+1] = c;
      pixbmp[i+2] = c;
    }
    
  return pixbmp;
}
//------------------------------------------------------------------------------
void dt_map_t::swap_int(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}
//------------------------------------------------------------------------------
void dt_map_t::drawHline(double x1, double y1, double w)
{
  int xi1 = x1 / pixResol;
  int yi1 = y1 / pixResol;
  int wi  = w  / pixResol;
  
  fastHline(xi1,yi1,wi);
}
//------------------------------------------------------------------------------
void dt_map_t::drawVline(double x1, double y1, double h)
{
  int xi1 = x1 / pixResol;
  int yi1 = y1 / pixResol;
  int hi  = h  / pixResol;
  
  int yi2 = yi1 + hi;
  for(int y=yi1; y<yi2; y++)
    setPix(xi1,y);
}
//------------------------------------------------------------------------------
void dt_map_t::drawline(double x1, double y1, double x2, double y2)
{
  int xi1 = x1 / pixResol;
  int yi1 = y1 / pixResol;
  int xi2 = x2 / pixResol;
  int yi2 = y2 / pixResol;  
  
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
  
  if (abs(xi2 - xi1) < abs(yi2 - yi1))
  {
    /* parcours par l'axe vertical */

    if (yi1 > yi2)
    {
      swap_int(&xi1, &xi2);
      swap_int(&yi1, &yi2);
    }

    xincr = xi2 > xi1 ? 1 : -1;
    dy = yi2 - yi1;
    dx = abs(xi2 - xi1);
    d = 2 * dx - dy;
    aincr = 2 * (dx - dy);
    bincr = 2 * dx;
    x = xi1;
    y = yi1;
    
    setPix(x, y);

    for (y = yi1+1; y <= yi2; ++y)
    {
      if (d >= 0)
      {
	      x += xincr;
	      d += aincr;
      }
      else
	      d += bincr;

      setPix(x, y);
    }
  }
  else 
  {
    /* parcours par l'axe horizontal */
    if (xi1 > xi2)
    {
      swap_int(&xi1, &xi2);
      swap_int(&yi1, &yi2);
    }

    yincr = yi2 > yi1 ? 1 : -1;
    dx = xi2 - xi1;
    dy = abs(yi2 - yi1);
    d = 2 * dy - dx;
    aincr = 2 * (dy - dx);
    bincr = 2 * dy;
    x = xi1;
    y = yi1;

    setPix(x, y);

    for (x = xi1+1; x <= xi2; ++x)
    {
      if (d >= 0)
      {
       	y += yincr;
       	d += aincr;
      }
      else
      	d += bincr;

      setPix(x, y);
    }
  }    
}
//------------------------------------------------------------------------------
void dt_map_t::drawBox(double x1, double y1, double w, double h)
{
  int xi1 = x1 / pixResol;
  int yi1 = y1 / pixResol;
  int wi  = w  / pixResol;
  int hi  = h  / pixResol;
  
  for(int y=yi1; y<hi; y++)
    fastHline(xi1,y,wi);
}
//------------------------------------------------------------------------------
void dt_map_t::drawDisc(double x1, double y1, double r)
{
  int cxi = x1 / pixResol;
  int cyi = y1 / pixResol;
  int ri  = r  / pixResol;
  
  int d, y, x;

  d = 3 - (2 * ri);
  x = 0;
  y = ri;

  while (y >= x) {
    fastHline(cxi - x, cyi - y, 2 * x + 1);
    fastHline(cxi - x, cyi + y, 2 * x + 1);    
    fastHline(cxi - y, cyi - x, 2 * y + 1);
    fastHline(cxi - y, cyi + x, 2 * y + 1);    
    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }
}
//------------------------------------------------------------------------------
void dt_map_t::compute_distance_transform()
{
  dt_map_pix_t *tmppix = new dt_map_pix_t[width * height];
  
  // Col transformation
  distance_transform_1D(pix, tmppix, true);
    
  // Row transformation
  distance_transform_1D(tmppix, pix, false);
    
  delete[] tmppix;
  
  // Taking sq root to have distance  
  for(int x=0; x<width; x++)
    for(int y=0; y<height; y++)
      pix[y*width+x] = pixResol*sqrt(getPix(x,y));
}
//------------------------------------------------------------------------------
void dt_map_t::distance_transform_1D(dt_map_pix_t *src_pix, dt_map_pix_t *dest_pix, bool vertical)
{
  int x,y;
  int n,m;
  int *i, *j;
  if(vertical)
  {
    n = height;
    m = width;
    i = &y;
    j = &x;
  }
  else
  {
    n = width;
    m = height;
    i = &x;
    j = &y;
  }
  int &q = *i;
  int &p = *j;
  
  double f_q_p_q2, f_vk;
  int k;
  int *v = new int[n+2];
  dt_map_pix_t *z = new dt_map_pix_t[n+3];
  dt_map_pix_t s;
  
  for(p=0; p<m; p++)
  {
    k = 0;
    v[0] = -1;
    z[0] = -std::numeric_limits<dt_map_pix_t>::infinity();
    z[1] = +std::numeric_limits<dt_map_pix_t>::infinity();
    
    for(q = 0; q <= n; q++)
    {
      f_q_p_q2 = ((q==n)?0.:src_pix[y*width+x]) + double(q*q);

      while(true)
      {  
        f_vk = (v[k]==-1)?0.:(vertical?src_pix[v[k]*width+x]:src_pix[y*width+v[k]]);
        s = ( f_q_p_q2 - (f_vk + double(v[k]*v[k])) ) / (2. * double(q - v[k])); 
           
        if(isfinite(s))
        {
          if(s <= z[k])
            k--;
          else
          {
            k++;
            v[k] = q;
            z[k] = s;
            z[k+1] = +std::numeric_limits<dt_map_pix_t>::infinity();
            break;
          }
        }
        else
          break;
      }    
    }
    
    k = 0;
    f_vk = 0.;
     for(q=0; q<n; q++)
    {
      while(z[k+1]<q)
      {
        k++;    
        f_vk = (v[k]==n)?0.:(vertical?src_pix[v[k]*width+x]:src_pix[y*width+v[k]]);
      }
      dest_pix[y*width+x] = f_vk + pow(q-v[k], 2);
    }
  }
   
  delete[] v;
  delete[] z;
}
