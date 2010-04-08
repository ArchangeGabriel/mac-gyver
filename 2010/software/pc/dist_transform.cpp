#include <stdlib.h>
#include <iostream>
#include <limits>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "dist_transform.h"

const dt_map_pix dt_map::contrast = 7.;


//------------------------------------------------------------------------------
//                                                                            //
//                                 dt_zone                                    //
//                                                                            //
//------------------------------------------------------------------------------
dt_zone::dt_zone(int h)
{
  cy = h/2;
  height = h;
  allocate();
}
//------------------------------------------------------------------------------
dt_zone::dt_zone(int h, int c)
{
  cy = c;
  height = h;
  allocate();
}
//------------------------------------------------------------------------------
void dt_zone::allocate()
{
  left  = new int[height];
  right = new int[height];  
  
  for(int y=0; y<height; y++)
  {
    left[y] = INT_MAX;
    right[y] = INT_MIN;   
  }
}
//------------------------------------------------------------------------------
dt_zone::~dt_zone()
{
  delete[] left;
  delete[] right;
}
//------------------------------------------------------------------------------
dt_zone* dt_zone::operator * (const dt_zone &A)
{
  dt_zone* res = new dt_zone(height+A.height-1, cy+A.cy);
  
  int v;
  for(int i=0; i<height; i++)
    for(int j=0; j<A.height; j++)
    {
      v = left[i] + A.left[j];
      if(v < res->left[i+j])
        res->left[i+j] = v;
      v = right[i] + A.right[j];
      if(v > res->right[i+j])
        res->right[i+j] = v;      
    }
  
  return res;
}
//------------------------------------------------------------------------------
dt_zone_disc::dt_zone_disc(int radius) : dt_zone(2*radius+1)
{  
  int d, y, x;

  d = 3 - (2 * radius);
  x = 0;
  y = radius;

  while (y >= x) {
    left [cy - y] = -x;
    right[cy - y] = +x;
    left [cy + y] = -x;
    right[cy + y] = +x;
    left [cy - x] = -y;
    right[cy - x] = +y;
    left [cy + x] = -y;
    right[cy + x] = +y;
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
dt_zone_hbox::dt_zone_hbox(int w, int h) : dt_zone(h)
{
  int w2 = w/2;
  for(int y=0; y<h; y++)
  {
    left[y] = - w2;
    right[y] = w - w2 - 1;
  }
}
//------------------------------------------------------------------------------
dt_zone_orientbox::dt_zone_orientbox(int width, int depth, double angle) : dt_zone(width*fabs(cos(angle)) + depth*fabs(sin(angle)) + 1)
{
  const double C = cos(angle);
  const double S = sin(angle);
  const double h_offset = (width*fabs(cos(angle)) + depth*fabs(sin(angle))) / 2.;
  const double hw = double(width)/2.;
  const double hd = double(depth)/2.;

  double w[] = {hw, hw, -hw, -hw};
  double d[] = {hd, -hd, -hd, hd};  

  int px[4];
  int py[4];

  printf("off: %f  height: %d\n", h_offset, height);

  for(int i=0; i<4; i++)
  {
    px[i] = d[i]*C-w[i]*S;
    py[i] = d[i]*S+w[i]*C + h_offset;    
    
    printf("i: %d, (%d, %d)\n", i, px[i], py[i]);
  }
  fflush(stdout);
  
  const int i_top = (C > 0) ? ((S>0)?2:3) : ((S>0)?1:0);
  const int i_left = (i_top+3)%4;
  const int i_right = (i_top+1)%4;
  const int i_bottom = (i_top+2)%4;
  
  line(left,  px[i_bottom], py[i_bottom], px[i_left],   py[i_left]);
  line(left,  px[i_left],   py[i_left],   px[i_top],    py[i_top]);
  line(right, px[i_top],    py[i_top],    px[i_right],  py[i_right]);
  line(right, px[i_right],  py[i_right],  px[i_bottom], py[i_bottom]);
  
  printf("left[0]: %d right[0]: %d\n", left[0], right[0]);
}
//------------------------------------------------------------------------------
void dt_zone_orientbox::line(int *array, int x1, int y1, int x2, int y2)
{ 
  printf("line: (%d,%d) (%d, %d)\n", x1, y1, x2, y2);
  if(y1 == y2)
    array[y1] = x1;
  else
  {  
    int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
    
    if (abs(x2 - x1) < abs(y2 - y1))
    {
      /* parcours par l'axe vertical */

      if (y1 > y2)
      {
        swap_int(&x1, &x2);
        swap_int(&y1, &y2);
      }

      xincr = x2 > x1 ? 1 : -1;
      dy = y2 - y1;
      dx = abs(x2 - x1);
      d = 2 * dx - dy;
      aincr = 2 * (dx - dy);
      bincr = 2 * dx;
      x = x1;
      y = y1;
      
      array[y] = x;

      for (y = y1+1; y <= y2; ++y)
      {
        if (d >= 0)
        {
	        x += xincr;
	        d += aincr;
        }
        else
	        d += bincr;

        array[y] = x;
      }
    }
    else 
    {
      /* parcours par l'axe horizontal */
      if (x1 > x2)
      {
        swap_int(&x1, &x2);
        swap_int(&y1, &y2);
      }

      yincr = y2 > y1 ? 1 : -1;
      dx = x2 - x1;
      dy = abs(y2 - y1);
      d = 2 * dy - dx;
      aincr = 2 * (dy - dx);
      bincr = 2 * dy;
      x = x1;
      y = y1;

      array[y] = x;

      for (x = x1+1; x <= x2; ++x)
      {
        if (d >= 0)
        {
         	y += yincr;
         	d += aincr;
        }
        else
        	d += bincr;

        array[y] = x;
      }
    } 
  }
}
//------------------------------------------------------------------------------
void dt_zone_orientbox::swap_int(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                                                                            //
//                                  dt_map                                    //
//                                                                            //
//------------------------------------------------------------------------------
dt_map::dt_map(double terrainWidth, double terrainHeight, double robotWidth, double robotDepth, double resolXY, int resolA)
{
  nAngle = resolA;
  pixResol = resolXY;
  width = terrainWidth / pixResol;
  height = terrainHeight / pixResol;
  int robotW = robotWidth / pixResol;
  int robotD = robotDepth / pixResol;
  
  if(resolA<1)
    resolA = 1;
  
  robot = new dt_zone_orientbox*[resolA];
  for(int i=0; i<resolA; i++)
    robot[i] = new dt_zone_orientbox(robotW, robotD, double(i)*M_PI/double(resolA));
  
  pix = new dt_map_pix*[nAngle];
  for(int i=0; i<nAngle; i++)
  {
    pix[i] = new dt_map_pix[width * height];
    for(int k=0; k<width*height; k++)
      pix[i][k] = std::numeric_limits<dt_map_pix>::infinity();
  }
}
//------------------------------------------------------------------------------
dt_map::~dt_map()
{
  for(int i=0; i<nAngle; i++)
    delete[] pix[i];
  delete[] pix;
}
//------------------------------------------------------------------------------
void dt_map::fillZone(dt_zone *zone, int iAngle, int cx, int cy)
{
  int y,x1,x2;
  for(int i=0; i<zone->height; i++)
  {
    if((y = i+cy-zone->cy) < 0)
      continue;
    if(y >= height)
      break;
    x1 = cx + zone->left[i];
    x2 = cx + zone->right[i];
    if(x1<0) x1 = 0;
    if(x2>=width) x2 = width-1;
    memset(&pix[iAngle][y*width+x1], 0, (x2-x1+1)*sizeof(dt_map_pix));
  } 
}
//------------------------------------------------------------------------------
uint16_t* dt_map::to_bitmap(int iAngle, int &w, int &h)
{
  w = width;
  h = height;
  uint16_t *pixbmp = new uint16_t[3*width*height];
  
  for(int y=height-1;y>=0; y--)
    for(int x=0;x<width; x++)
    {
      int c = (1. - (1. / (1. + contrast*pix[iAngle][y*width+x]))) * 255.;
      int i = 3*((height-1-y)*width+x);
      pixbmp[i+0] = c;
      pixbmp[i+1] = c;
      pixbmp[i+2] = c;
    }
    
  return pixbmp;
}
//------------------------------------------------------------------------------
void dt_map::compute_distance_transform()
{
  dt_map_pix *tmppix = new dt_map_pix[width * height];
  
  for(int i=0; i<nAngle; i++)
  {
    // Col transformation
    distance_transform_1D(pix[i], tmppix, true);
    
    // Row transformation
    distance_transform_1D(tmppix, pix[i], false);
     
    // Taking square root to have distance  
    for(int x=0; x<width; x++)
      for(int y=0; y<height; y++)
        pix[i][y*width+x] = pixResol*sqrt(pix[i][y*width+x]);
  }
  delete[] tmppix;
}
//------------------------------------------------------------------------------
void dt_map::distance_transform_1D(dt_map_pix *src_pix, dt_map_pix *dest_pix, bool vertical)
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
  dt_map_pix *z = new dt_map_pix[n+3];
  dt_map_pix s;
  
  for(p=0; p<m; p++)
  {
    k = 0;
    v[0] = -1;
    z[0] = -std::numeric_limits<dt_map_pix>::infinity();
    z[1] = +std::numeric_limits<dt_map_pix>::infinity();
    
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
            z[k+1] = +std::numeric_limits<dt_map_pix>::infinity();
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
//------------------------------------------------------------------------------
void dt_map::fillBox(double x, double y, double w, double h)
{
  int xi = x / pixResol; 
  int yi = y / pixResol; 
  int wi  = w  / pixResol;
  int hi  = h  / pixResol;
  
  dt_zone_hbox box(wi, hi);
  
  for(int i=0; i<nAngle; i++)
  {
    dt_zone *zone = box * (*robot[i]);
    fillZone(zone, i, xi+wi/2, yi+hi/2);     
    delete zone;
  }
}
//------------------------------------------------------------------------------
void dt_map::fillDisc(double cx, double cy, double radius)
{
  int cxi = cx / pixResol; 
  int cyi = cy / pixResol; 
  int r = radius / pixResol;
    
  dt_zone_disc disc(r);
  
  for(int i=0; i<nAngle; i++)
  {
    dt_zone *zone = disc * (*robot[i]);
    fillZone(zone, i, cxi, cyi);
    delete zone;
  }
}
//------------------------------------------------------------------------------
