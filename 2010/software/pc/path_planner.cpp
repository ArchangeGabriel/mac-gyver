#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <limits>
#include <queue>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <list>

#define PC_INCLUDE
#include "../common/const.h"
#include "../common/bitmap.h"
#include "path_planner.h"
#include "visualizer.h"

extern "C" void sgetrf_(int*, int*, float*, int*, int*, int*);
extern "C" void sgetri_(int*, float*, int*, int*, float*, int*, int*);

// Resolutions
#define PP_SPATIAL_RESOL  0.005    // Résolution métrique pour la carte des distances
#define PP_ANGLE_RESOL 8        // Résolution angulaire pour la carte des distances

// Marge de sécurité entre le robot et les obstacles en mètres
#define SECURITY_MARGIN  0.1

// Constants for optimisation
#define ALPHA   0.005     // Weight for first derivative 
#define BETA    0.005     // Weight for second derivative
#define GAMMA   0.05      // Converging rate (the bigger, the slower)

const pp_dist pp_map::pix_cost = 1./3.;   // score corresponding to 3*SECURITY_MARGIN
const double pp_map::RAD_ANGLE_RESOL   = M_PI   / double(PP_ANGLE_RESOL);
const double pp_map::RAD_ANGLE_RESOL_2 = M_PI_2 / double(PP_ANGLE_RESOL);

//------------------------------------------------------------------------------
//                                                                            //
//                                 pp_zone                                    //
//                                                                            //
//------------------------------------------------------------------------------
pp_zone::pp_zone(int h)
{
  cy = h/2;
  height = h;
  allocate();
}
//------------------------------------------------------------------------------
pp_zone::pp_zone(int h, int c)
{
  cy = c;
  height = h;
  allocate();
}
//------------------------------------------------------------------------------
void pp_zone::allocate()
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
pp_zone::~pp_zone()
{
  delete[] left;
  delete[] right;
}
//------------------------------------------------------------------------------
pp_zone* pp_zone::operator * (const pp_zone &A)
{
  pp_zone* res = new pp_zone(height+A.height-1, cy+A.cy);
  
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
pp_zone_disc::pp_zone_disc(int radius) : pp_zone(2*radius+1)
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
pp_zone_hbox::pp_zone_hbox(int w, int h) : pp_zone(h)
{
  int w2 = w/2;
  for(int y=0; y<h; y++)
  {
    left[y] = - w2;
    right[y] = w - w2 - 1;
  }
}
//------------------------------------------------------------------------------
pp_zone_orientbox::pp_zone_orientbox(int width, int depth, double angle) : pp_zone(width*fabs(cos(angle)) + depth*fabs(sin(angle)) + 1)
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

  for(int i=0; i<4; i++)
  {
    px[i] = d[i]*C-w[i]*S;
    py[i] = d[i]*S+w[i]*C + h_offset;    
  }
  
  const int i_top = (C > 0) ? ((S>0)?2:3) : ((S>0)?1:0);
  const int i_left = (i_top+3)%4;
  const int i_right = (i_top+1)%4;
  const int i_bottom = (i_top+2)%4;
  
  line(left,  px[i_bottom], py[i_bottom], px[i_left],   py[i_left]);
  line(left,  px[i_left],   py[i_left],   px[i_top],    py[i_top]);
  line(right, px[i_top],    py[i_top],    px[i_right],  py[i_right]);
  line(right, px[i_right],  py[i_right],  px[i_bottom], py[i_bottom]);
}
//------------------------------------------------------------------------------
void pp_zone_orientbox::line(int *array, int x1, int y1, int x2, int y2)
{ 
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
void pp_zone_orientbox::swap_int(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                                                                            //
//                                  pp_map                                    //
//                                                                            //
//------------------------------------------------------------------------------
pp_map::pp_map(double terrainWidth, double terrainHeight, double robotWidth, double robotDepth)
{
  width = terrainWidth / PP_SPATIAL_RESOL;
  height = terrainHeight / PP_SPATIAL_RESOL;
  int robotW = robotWidth / PP_SPATIAL_RESOL;
  int robotD = robotDepth / PP_SPATIAL_RESOL;
  
  robot = new pp_zone_orientbox*[PP_ANGLE_RESOL];
  pix = new pp_dist*[PP_ANGLE_RESOL];
  pix_gradX = new pp_dist*[PP_ANGLE_RESOL];
  pix_gradY = new pp_dist*[PP_ANGLE_RESOL];
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    robot[i] = new pp_zone_orientbox(robotW, robotD, double(i)*M_PI/double(PP_ANGLE_RESOL));
    pix[i] = new pp_dist[width * height];
    pix_gradX[i] = new pp_dist[width * height];
    pix_gradY[i] = new pp_dist[width * height];
    for(int k=0; k<width*height; k++)
      pix[i][k] = numeric_limits<pp_dist>::infinity();
  }
}
//------------------------------------------------------------------------------
pp_map::~pp_map()
{
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    delete robot[i];  
    delete[] pix[i];
    delete[] pix_gradX[i];    
    delete[] pix_gradY[i];        
  }
  delete[] robot;   
  delete[] pix;
  delete[] pix_gradX;
  delete[] pix_gradY;  
}
//------------------------------------------------------------------------------
void pp_map::fillZone(pp_zone *zone, int iAngle, int cx, int cy)
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
    memset(&pix[iAngle][y*width+x1], 0, (x2-x1+1)*sizeof(pp_dist));
  } 
}
//------------------------------------------------------------------------------
void pp_map::to_bitmap(uint16_t *pixbmp, int iAngle, int &w, int &h)
{
  w = width;
  h = height;
  
  for(int y=0;y<height; y++)
    for(int x=0;x<width; x++)
    {
      int c = pix[iAngle][y*width+x] * 255.;
      int i = 3*(y*width+x);
      pixbmp[i+0] = c;
      pixbmp[i+1] = c;
      pixbmp[i+2] = c;
    }
}
//------------------------------------------------------------------------------
void pp_map::compute_distance_transform()
{
  pp_dist *tmppix = new pp_dist[width * height];
  
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    // Col transformation
    distance_transform_1D(pix[i], tmppix, true);
    
    // Row transformation
    distance_transform_1D(tmppix, pix[i], false);
     
    // Taking square root to have distance  
    for(int x=0; x<width; x++)
      for(int y=0; y<height; y++)
        pix[i][y*width+x] = score(PP_SPATIAL_RESOL*sqrt(pix[i][y*width+x]));       
  }
  
  // Computes energy gradient
  compute_gradient();
  
  delete[] tmppix;
}
//------------------------------------------------------------------------------
void pp_map::distance_transform_1D(pp_dist *src_pix, pp_dist *dest_pix, bool vertical)
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
  pp_dist *z = new pp_dist[n+3];
  pp_dist s;
  
  for(p=0; p<m; p++)
  {
    k = 0;
    v[0] = -1;
    z[0] = -numeric_limits<pp_dist>::infinity();
    z[1] = +numeric_limits<pp_dist>::infinity();
    
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
            z[k+1] = +numeric_limits<pp_dist>::infinity();
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
void pp_map::compute_gradient()
{
  for(int i=0; i<PP_ANGLE_RESOL; i++)
    for(int x=0; x<width; x++)
      for(int y=0; y<height; y++)
      {
        if(x == 0)             pix_gradX[i][y*width]   =  pix[i][y*width+1]     - pix[i][y*width];
        else if(x == width-1)  pix_gradX[i][y*width+x] =  pix[i][(y+1)*width-1] - pix[i][(y+1)*width-2];
        else                   pix_gradX[i][y*width+x] = (pix[i][y*width+x+1]   - pix[i][y*width+x-1]) / 2.;
        
        if(y == 0)             pix_gradY[i][x]         =  pix[i][width+x]            - pix[i][x];
        else if(y == height-1) pix_gradY[i][y*width+x] =  pix[i][(height-1)*width+x] - pix[i][(height-2)*width+x];
        else                   pix_gradY[i][y*width+x] = (pix[i][(y+1)*width+x]      - pix[i][(y-1)*width+x]) / 2.;
      }
}
//------------------------------------------------------------------------------
void pp_map::fillBox(double x, double y, double w, double h)
{
  int xi = x / PP_SPATIAL_RESOL; 
  int yi = y / PP_SPATIAL_RESOL; 
  int wi  = w  / PP_SPATIAL_RESOL;
  int hi  = h  / PP_SPATIAL_RESOL;
  
  pp_zone_hbox box(wi, hi);
  
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    pp_zone *zone = box * (*robot[i]);
    fillZone(zone, i, xi+wi/2, yi+hi/2);     
    delete zone;
  }
}
//------------------------------------------------------------------------------
void pp_map::fillDisc(double cx, double cy, double radius)
{
  int cxi = cx / PP_SPATIAL_RESOL; 
  int cyi = cy / PP_SPATIAL_RESOL; 
  int r = radius / PP_SPATIAL_RESOL;
    
  pp_zone_disc disc(r);
  
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    pp_zone *zone = disc * (*robot[i]);
    fillZone(zone, i, cxi, cyi);
    delete zone;
  }
}
//------------------------------------------------------------------------------
void pp_map::draw_config(int configE, int configI)
{ 
  if(configE != 0 && configI !=0)
  {
    bool FakeCornE[3][3];
    bool FakeCornI[2];
    FakeCornE[0][0] = (configE==2 || configE==5 || configE==8);
    FakeCornE[1][0] = (configE==4 || configE==5 || configE==6);  
    FakeCornE[2][0] = (configE==7 || configE==8 || configE==9);    
    FakeCornE[0][1] = (configE==1 || configE==6 || configE==7);
    FakeCornE[1][1] = (configE==1 || configE==2 || configE==3);  
    FakeCornE[2][1] = (configI==2 || configI==4);
    FakeCornE[0][2] = (configE==3 || configE==4 || configE==9);
    FakeCornE[1][2] = (configI==1 || configI==3);
    FakeCornE[2][2] = false;      
    FakeCornI[0] = (configI==1 || configI==2);
    FakeCornI[1] = (configI==3 || configI==4);
    
    for(int i=0;i<3;++i)  
      for(int j=0;j<3;++j)    
        if(i != 2 || j != 2)
        {
          if(FakeCornE[i][j])
          {
            double x,y;
            x = 0.15 + 0.45*i;
            y = 2.1 - 1.378 + 0.5*j + 0.25*i;   
            fillDisc(x, y, 0.025);

            x = _LONGUEUR_TER - 0.15 - 0.45*i;
            y = 2.1 - 1.378 + 0.5*j + 0.25*i;
            fillDisc(x, y, 0.025);
          }
        }
        
    for(int i=0;i<2;++i) 
    {
      if(FakeCornI[i])
      {
        double x = _LONGUEUR_TER/2.;
        double y = 2.1 - 0.628 + 0.5*i; 
        fillDisc(x, y, 0.025);
      }
    }
  }   
  fillBox(0.74, 0., 1.519, 0.52); 
  fillBox(0., 0., _LONGUEUR_TER, PP_SPATIAL_RESOL); 
  fillBox(0., 0.,  PP_SPATIAL_RESOL, _LARGEUR_TER);
  fillBox(0., _LARGEUR_TER, _LONGUEUR_TER, PP_SPATIAL_RESOL); 
  fillBox(_LONGUEUR_TER, 0.,  PP_SPATIAL_RESOL, _LARGEUR_TER);
}
//------------------------------------------------------------------------------
pp_dist pp_map::score(double distance)
{
  return 1./pow(1.+distance/SECURITY_MARGIN, 2.);
}
//------------------------------------------------------------------------------
pp_path pp_map::find_path(const position_t &from, const position_t &to)
{
  int angle_pix_step = 0.05 / PP_SPATIAL_RESOL;
  if(angle_pix_step<1) angle_pix_step = 1;  
  const int xs = clip(from.x / PP_SPATIAL_RESOL, width);
  const int ys = clip(from.y / PP_SPATIAL_RESOL, height);
  const int xt = clip(to.x / PP_SPATIAL_RESOL, width);
  const int yt = clip(to.y / PP_SPATIAL_RESOL, height);

  priority_queue<pp_pix> boundary;
  priority_queue<pp_pix> boundary_dont_cross;
  list<pp_pix> pix_poped;
  bool allow_crossing = false;
  
  pp_dist *dist = new pp_dist[width * height];
  bool *processed = new bool[width * height];
  for(int x=0; x<width; x++)
    for(int y=0; y<height; y++)
    {
      dist[y*width+x] = numeric_limits<pp_dist>::infinity();
      processed[y*width+x] = false;
    }
      
  dist[ys*width+xs] = 0;
  
  boundary.push(pp_pix(xs, ys, from.a, angle_pix_step));
    
  while(true)
  {
    while(!boundary.empty())
    {
      pix_poped.push_back(boundary.top());
      boundary.pop();
      pp_pix &current = pix_poped.back();

      if(processed[current.y*width+current.x]) continue;
      if(current.x == xt && current.y == yt) 
      {
        delete[] processed;
        delete[] dist;
        return build_path(&current, from.a, to.a);
      }
      processed[current.y*width+current.x] = true; 
      
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current, -1,  0, xt, yt); 
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current, +1,  0, xt, yt); 
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current,  0, -1, xt, yt); 
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current,  0, +1, xt, yt);
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current, -1, -1, xt, yt); 
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current, -1, +1, xt, yt); 
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current, +1, -1, xt, yt); 
      add_to_boundary(dist, processed, boundary, boundary_dont_cross, allow_crossing, current, +1, +1, xt, yt);      
    }
    // On n'a pas trouvé de chemin, on permet d'aller dans l'infranchissable (l'objectif est peut-être dedans)
    boundary = boundary_dont_cross;
    allow_crossing = true;
  }  
}
//------------------------------------------------------------------------------
pp_dist pp_map::estimate_path_energy(int x1, int y1, int x2, int y2)
{
  return pix_cost * sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
//------------------------------------------------------------------------------
int pp_map::clip(int c, int max)
{
  return c<0?0:(c>=max?max-1:c);
}
//------------------------------------------------------------------------------
void pp_map::add_to_boundary(pp_dist *dist, bool *processed, priority_queue<pp_pix> &boundary, priority_queue<pp_pix> &boundary_dont_cross, bool allow_crossing, pp_pix &p, int dx, int dy, int xt, int yt)
{
  int x = p.x + dx;
  int y = p.y + dy;
  if(x >= 0 && x<width && y >= 0 && y<height && !processed[y*width+x])
  {
    pp_dist energy = get_pix_energy(x, y, p.angle);
    pp_dist new_dist = p.dist + energy;
    if(new_dist < dist[y*width+x])
    {
      dist[y*width+x] = new_dist;
      pp_dist new_score = new_dist + estimate_path_energy(x, y, xt, yt);
      if(energy == 1. && !allow_crossing)
        boundary_dont_cross.push(pp_pix(x, y, new_dist, new_score, &p));
      else
        boundary.push(pp_pix(x, y, new_dist, new_score, &p));
    }
  } 
}
//------------------------------------------------------------------------------
pp_dist pp_map::get_pix_energy(int x, int y, double a)
{
  if(a < 0) a += 2*M_PI;
  int k = a / RAD_ANGLE_RESOL;
  if(a - k*RAD_ANGLE_RESOL > RAD_ANGLE_RESOL_2) k++;
  while(k>=PP_ANGLE_RESOL)  // symétrie du robot
    k -= PP_ANGLE_RESOL;
  return pix[k][y*width+x];
}
//------------------------------------------------------------------------------
void pp_map::get_pix_energy_gradient(int x, int y, double a, pp_dist &dEdx, pp_dist &dEdy)
{
  if(a < 0) a += 2*M_PI;
  int k = a / RAD_ANGLE_RESOL;
  if(a - k*RAD_ANGLE_RESOL > RAD_ANGLE_RESOL_2) k++;
  while(k>=PP_ANGLE_RESOL)  // symétrie du robot
    k -= PP_ANGLE_RESOL;
  dEdx = pix_gradX[k][y*width+x];
  dEdy = pix_gradY[k][y*width+x];
}
//------------------------------------------------------------------------------
pp_path pp_map::build_path(pp_pix *current, double initial_angle, double final_angle)
{ 
  int resol = 0.05 / PP_SPATIAL_RESOL;
  if(resol<1) resol = 1;
  int count = resol;
  
  list<pair<int, int> > points;
  
  // Add the last pixel
  points.push_front(pair<int, int>(current->x, current->y));
    
  while(current->parent)
  {
    if(!count) 
    {
      // Add intermediate pixels
      points.push_front(pair<int, int>(current->x, current->y));      
      count = resol;
    }
    count--;
    current = current->parent;
  }
    
  // Add the first pixel
  points.push_front(pair<int, int>(current->x, current->y));
  
  // Transform into float arrays
  const int n_points = points.size();
  float *X = new float[n_points];
  float *Y = new float[n_points];  
    
  list<pair<int, int> >::iterator iter = points.begin();
  for(int i=0; iter != points.end(); iter++, i++)
  {
    X[i] = iter->first;
    Y[i] = iter->second;    
  }

  // Optimize path
  optimize_path(n_points, X, Y);
    
  // Convert to pp_path
  pp_path path = pix_path2pp_path(n_points, X, Y, initial_angle, final_angle);
  
  delete[] X;
  delete[] Y;
  
  return path;
}
//------------------------------------------------------------------------------
void pp_map::optimize_path(int N, float *X, float *Y)
{
  const int n_iter_fast = 150;
  const int n_iter_slow = 50;
  const float slow_gamma = GAMMA;
  const float fast_gamma = GAMMA/10;
  
  int info;
  int size;
  float *slow_conv = new float[N*N];
  float *fast_conv = new float[N*N];
  float lwork;
  float *work;
  int *pivot = new int[N];
     
  for(int i = 0; i<N; i++)
  {
    if(i == 0 || i == N-1) // First and last point doesn't move
    {
      for(int j = 0; j<N; j++) 
        slow_conv[j*N+i] = 0.;      
    } else if(i == 1 || i == N-2)   // only first derivative for i == 1 && i == N-2
    {
      for(int j = 0; j<N; j++) 
        slow_conv[j*N+i] = 0.;
      slow_conv[i*N+i] = 2.*ALPHA;
      slow_conv[(i+1)*N+i] = -ALPHA;
      slow_conv[(i-1)*N+i] = -ALPHA;
    } else
    {
      for(int j = 0; j<N; j++)
      {
        if(i == j)             slow_conv[j*N+i] = 2.*ALPHA + 6.*BETA;
        else if(abs(i-j) == 1) slow_conv[j*N+i] =   -ALPHA - 4.*BETA;
        else if(abs(i-j) == 2) slow_conv[j*N+i] =               BETA;
        else                   slow_conv[j*N+i] = 0.;
      }    
    }

    for(int j = 0; j<N; j++)
      fast_conv[j*N+i] = slow_conv[j*N+i];
    slow_conv[i*N+i] += slow_gamma;
    fast_conv[i*N+i] += fast_gamma;    
  }
     
  // LU decomposition (gets pivot array)
  sgetrf_(&N, &N, slow_conv, &N, pivot, &info);
  
  // Inverses matrix slow_conv
  size = -1;
  sgetri_(&N, slow_conv, &N, pivot, &lwork, &size, &info);
  size = lwork;
  work = new float[size];
  sgetri_(&N, slow_conv, &N, pivot, work, &size, &info);
  
  // The same for fast_conv
  sgetrf_(&N, &N, fast_conv, &N, pivot, &info);  
  sgetri_(&N, fast_conv, &N, pivot, work, &size, &info);    
  
  // Prepare energy derivatives
  float *Fx = new float[N];
  float *Fy = new float[N]; 
  Fx[0] = Fx[N-1] = 0.;
  Fy[0] = Fy[N-1] = 0.;  
  
  // Optimizes path
  float gamma = fast_gamma;
  float *A = fast_conv;
  int n_iter = n_iter_fast + n_iter_slow;
   
  for(int n=0; n<n_iter; n++)
  {
    //pp_path path = pix_path2pp_path(N, X, Y, 0.); visu_draw_path(path); usleep(10000);
    
    // Check for new convergence rate  
    if(n == n_iter_fast)
    {
      gamma = slow_gamma;
      A = slow_conv;
    }
    
    // gets gradients
    for(int i=0; i<N; i++)
      if(i>0 && i<N-1) 
      {
        vector_t v(X[i+1] - X[i-1], Y[i+1] - Y[i-1]);
        get_pix_energy_gradient(X[i], Y[i], v.to_angle(), Fx[i], Fy[i]);
      }
    
    // Compute x_(t+1) from x_t (the same for y)
    optimize_path_iter(N, gamma, A, X, Fx);
    optimize_path_iter(N, gamma, A, Y, Fy);
  }
  
  delete[] Fy;
  delete[] Fx;  
  delete[] pivot;
  delete[] slow_conv;    
  delete[] fast_conv;      
  delete[] work;
}
//------------------------------------------------------------------------------
void pp_map::optimize_path_iter(int N, const float &gamma, float *A, float *V, float *F)
{
  float *tmp = new float[N];
  for(int i=0; i<N; i++)
    tmp[i] = gamma * V[i] - F[i];
  for(int i=0; i<N; i++)
  {
    V[i] = 0;
    for(int j=0; j<N; j++)
      V[i] += A[j*N+i]*tmp[j];
  }
  delete[] tmp;
}
//------------------------------------------------------------------------------
pp_path pp_map::pix_path2pp_path(int N, float *X, float *Y, double initial_angle, double final_angle)
{
  pp_path path;
  path.resize(N+1);
  
  double angle;
  for(int i=0; i<N; i++)
  {
    if(i==0)
     angle = initial_angle;
    else
    {
      vector_t v(X[i] - X[i-1], Y[i] - Y[i-1]);
      angle = v.to_angle();
    }
    path[i] = position_t(X[i]*PP_SPATIAL_RESOL, Y[i]*PP_SPATIAL_RESOL, angle);
  }
  path[N] = position_t(X[N-1]*PP_SPATIAL_RESOL, Y[N-1]*PP_SPATIAL_RESOL, final_angle);
  
  return path;
}
//------------------------------------------------------------------------------
bool pp_map::load_from_file(const char* file)
{
  FILE *File = fopen(file, "rb");
  if(!File) return false;
  
  int r;
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    r = fread(pix[i], sizeof(pp_dist), width * height, File);
    if(r)
    {
      r = fread(pix_gradX[i], sizeof(pp_dist), width * height, File);
      if(r)
        r = fread(pix_gradY[i], sizeof(pp_dist), width * height, File);        
      else
        break;
    }
    else
      break;
  }
  fclose(File);
  return r;
}
//------------------------------------------------------------------------------
void pp_map::save_to_file(const char* file)
{
  FILE *File = fopen(file, "wb+");
  for(int i=0; i<PP_ANGLE_RESOL; i++)
  {
    fwrite(pix[i], sizeof(pp_dist), width * height, File);
    fwrite(pix_gradX[i], sizeof(pp_dist), width * height, File);
    fwrite(pix_gradY[i], sizeof(pp_dist), width * height, File);        
  }
  fclose(File);
}
//------------------------------------------------------------------------------
void pp_map::save_to_bmp(const char *file)
{
  int w, h;
  uint16_t *bmp = new uint16_t[PP_ANGLE_RESOL*3*width*height];
  
  for(int i=0; i<PP_ANGLE_RESOL; i++)
    to_bitmap(&bmp[i*3*width*height], i, w, h);
  
  save_buff_to_bitmap(file, width, PP_ANGLE_RESOL*height, bmp);
}
//------------------------------------------------------------------------------
