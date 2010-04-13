#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <limits>
#include <queue>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <list>

#include "dist_transform.h"
#include "visualizer.h"

extern "C" void sgetrf_(int*, int*, float*, int*, int*, int*);
extern "C" void sgetri_(int*, float*, int*, int*, float*, int*, int*);

// Marge de sécurité entre le robot et les obstacles en mètres
#define SECURITY_MARGIN  0.1

// Constants for optimisation
#define ALPHA   0.001     // Weight for first derivative 
#define BETA    0.005    // Weight for second derivative
#define GAMMA   0.01      // Converging rate

const dt_dist dt_map::pix_cost = 1./3.;   // score corresponding to 3*SECURITY_MARGIN
const double dt_map::RAD_ANGLE_RESOL   = M_PI   / double(DT_ANGLE_RESOL);
const double dt_map::RAD_ANGLE_RESOL_2 = M_PI_2 / double(DT_ANGLE_RESOL);

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
void dt_zone_orientbox::line(int *array, int x1, int y1, int x2, int y2)
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
dt_map::dt_map(double terrainWidth, double terrainHeight, double robotWidth, double robotDepth)
{
  width = terrainWidth / DT_DIST_RESOL;
  height = terrainHeight / DT_DIST_RESOL;
  int robotW = robotWidth / DT_DIST_RESOL;
  int robotD = robotDepth / DT_DIST_RESOL;
  
  robot = new dt_zone_orientbox*[DT_ANGLE_RESOL];
  for(int i=0; i<DT_ANGLE_RESOL; i++)
    robot[i] = new dt_zone_orientbox(robotW, robotD, double(i)*M_PI/double(DT_ANGLE_RESOL));
  
  pix = new dt_dist*[DT_ANGLE_RESOL];
  pix_gradX = new dt_dist*[DT_ANGLE_RESOL];
  pix_gradY = new dt_dist*[DT_ANGLE_RESOL];
  for(int i=0; i<DT_ANGLE_RESOL; i++)
  {
    pix[i] = new dt_dist[width * height];
    pix_gradX[i] = new dt_dist[width * height];
    pix_gradY[i] = new dt_dist[width * height];
    for(int k=0; k<width*height; k++)
      pix[i][k] = numeric_limits<dt_dist>::infinity();
  }
}
//------------------------------------------------------------------------------
dt_map::~dt_map()
{
  for(int i=0; i<DT_ANGLE_RESOL; i++)
  {
    delete[] pix[i];
    delete[] robot[i];
  }
  delete[] robot;   
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
    memset(&pix[iAngle][y*width+x1], 0, (x2-x1+1)*sizeof(dt_dist));
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
      int c = pix[iAngle][y*width+x] * 255.;
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
  dt_dist *tmppix = new dt_dist[width * height];
  
  for(int i=0; i<DT_ANGLE_RESOL; i++)
  {
    // Col transformation
    distance_transform_1D(pix[i], tmppix, true);
    
    // Row transformation
    distance_transform_1D(tmppix, pix[i], false);
     
    // Taking square root to have distance  
    for(int x=0; x<width; x++)
      for(int y=0; y<height; y++)
        pix[i][y*width+x] = score(DT_DIST_RESOL*sqrt(pix[i][y*width+x]));       
  }
  
  // Computes energy gradient
  compute_gradient();
  
  delete[] tmppix;
}
//------------------------------------------------------------------------------
void dt_map::distance_transform_1D(dt_dist *src_pix, dt_dist *dest_pix, bool vertical)
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
  dt_dist *z = new dt_dist[n+3];
  dt_dist s;
  
  for(p=0; p<m; p++)
  {
    k = 0;
    v[0] = -1;
    z[0] = -numeric_limits<dt_dist>::infinity();
    z[1] = +numeric_limits<dt_dist>::infinity();
    
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
            z[k+1] = +numeric_limits<dt_dist>::infinity();
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
void dt_map::compute_gradient()
{
  for(int i=0; i<DT_ANGLE_RESOL; i++)
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
void dt_map::fillBox(double x, double y, double w, double h)
{
  int xi = x / DT_DIST_RESOL; 
  int yi = y / DT_DIST_RESOL; 
  int wi  = w  / DT_DIST_RESOL;
  int hi  = h  / DT_DIST_RESOL;
  
  dt_zone_hbox box(wi, hi);
  
  for(int i=0; i<DT_ANGLE_RESOL; i++)
  {
    dt_zone *zone = box * (*robot[i]);
    fillZone(zone, i, xi+wi/2, yi+hi/2);     
    delete zone;
  }
}
//------------------------------------------------------------------------------
void dt_map::fillDisc(double cx, double cy, double radius)
{
  int cxi = cx / DT_DIST_RESOL; 
  int cyi = cy / DT_DIST_RESOL; 
  int r = radius / DT_DIST_RESOL;
    
  dt_zone_disc disc(r);
  
  for(int i=0; i<DT_ANGLE_RESOL; i++)
  {
    dt_zone *zone = disc * (*robot[i]);
    fillZone(zone, i, cxi, cyi);
    delete zone;
  }
}
//------------------------------------------------------------------------------
dt_dist dt_map::score(double distance)
{
  return 1./pow(1.+distance/SECURITY_MARGIN, 2.);
}
//------------------------------------------------------------------------------
dt_path dt_map::find_path(const position_t &from, const position_t &to)
{
  const int xs = clip(from.x / DT_DIST_RESOL, width);
  const int ys = clip(from.y / DT_DIST_RESOL, height);
  const int xt = clip(to.x / DT_DIST_RESOL, width);
  const int yt = clip(to.y / DT_DIST_RESOL, height);

  priority_queue<dt_pix> boundary;
  priority_queue<dt_pix> boundary_dont_cross;
  list<dt_pix> pix_poped;
  bool allow_crossing = false;
  
  dt_dist *dist = new dt_dist[width * height];
  bool *processed = new bool[width * height];
  for(int x=0; x<width; x++)
    for(int y=0; y<height; y++)
    {
      dist[y*width+x] = numeric_limits<dt_dist>::infinity();
      processed[y*width+x] = false;
    }
      
  dist[ys*width+xs] = 0;
  
  boundary.push(dt_pix(xs, ys, 0, 0, NULL));
    
  while(true)
  {
    while(!boundary.empty())
    {
      pix_poped.push_back(boundary.top());
      boundary.pop();
      dt_pix &current = pix_poped.back();

      if(processed[current.y*width+current.x]) continue;
      if(current.x == xt && current.y == yt) 
      {
        delete[] processed;
        delete[] dist;
        return build_path(&current, from.a);
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
dt_dist dt_map::estimate_path_energy(int x1, int y1, int x2, int y2)
{
  return pix_cost * sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
//------------------------------------------------------------------------------
int dt_map::clip(int c, int max)
{
  return c<0?0:(c>=max?max-1:c);
}
//------------------------------------------------------------------------------
void dt_map::add_to_boundary(dt_dist *dist, bool *processed, priority_queue<dt_pix> &boundary, priority_queue<dt_pix> &boundary_dont_cross, bool allow_crossing, dt_pix &p, int dx, int dy, int xt, int yt)
{
  int x = p.x + dx;
  int y = p.y + dy;
  if(x >= 0 && x<width && y >= 0 && y<height && !processed[y*width+x])
  {
    dt_dist energy = get_pix_energy(x, y, 0.);  /// !!!!!!
    dt_dist new_dist = p.dist + energy;
    if(new_dist < dist[y*width+x])
    {
      dist[y*width+x] = new_dist;
      dt_dist new_score = new_dist + estimate_path_energy(x, y, xt, yt);
      if(energy == 1. && !allow_crossing)
        boundary_dont_cross.push(dt_pix(x, y, new_dist, new_score, &p));
      else
        boundary.push(dt_pix(x, y, new_dist, new_score, &p));
    }
  } 
}
//------------------------------------------------------------------------------
dt_dist dt_map::get_pix_energy(int x, int y, double a)
{
  int k = a / RAD_ANGLE_RESOL;
  if(a - k*RAD_ANGLE_RESOL > RAD_ANGLE_RESOL_2) k++;
  return pix[k][y*width+x];
}
//------------------------------------------------------------------------------
void dt_map::get_pix_energy_gradient(int x, int y, double a, dt_dist &dEdx, dt_dist &dEdy)
{
  int k = a / RAD_ANGLE_RESOL;
  if(a - k*RAD_ANGLE_RESOL > RAD_ANGLE_RESOL_2) k++;
  dEdx = pix_gradX[k][y*width+x];
  dEdy = pix_gradY[k][y*width+x];
}
//------------------------------------------------------------------------------
dt_path dt_map::build_path(dt_pix *current, double initial_angle)
{ 
  int resol = 0.05 / DT_DIST_RESOL;
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
    
  // Convert to dt_path
  dt_path path = pix_path2dt_path(n_points, X, Y, initial_angle);
  
  delete[] X;
  delete[] Y;
  
  return path;
}
//------------------------------------------------------------------------------
void dt_map::optimize_path(int N, float *X, float *Y)
{
  int info;
  int size;
  float *A = new float[N*N];
  float lwork;
  float *work;
  int *pivot = new int[N];
  
  for(int i = 0; i<N; i++)
  {
    if(i == 0 || i == N-1) // First and last point doesn't move
    {
      for(int j = 0; j<N; j++) 
        A[j*N+i] = 0.;
      A[i*N+i] = GAMMA;        
    } else if(i == 1 || i == N-2)   // only first derivative for i == 1 && i == N-2
    {
      for(int j = 0; j<N; j++) 
        A[j*N+i] = 0.;
      A[i*N+i] = 2.*ALPHA + GAMMA;
      A[(i+1)*N+i] = -ALPHA;
      A[(i-1)*N+i] = -ALPHA;
    } else
    {
      for(int j = 0; j<N; j++)
      {
        if(i == j)             A[j*N+i] = 2.*ALPHA + 6.*BETA + GAMMA;
        else if(abs(i-j) == 1) A[j*N+i] =   -ALPHA - 4.*BETA;
        else if(abs(i-j) == 2) A[j*N+i] =               BETA;
        else                   A[j*N+i] = 0.;
      }    
    }
  }
    
  // LU decomposition (gets pivot array)
  sgetrf_(&N, &N, A, &N, pivot, &info);
  
  // Inverses matrix A
  size = -1;
  sgetri_(&N, A, &N, pivot, &lwork, &size, &info);
  size = lwork;
  work = new float[size];
  sgetri_(&N, A, &N, pivot, work, &size, &info);  
  
  // Optimizes path
  float *Fx = new float[N];
  float *Fy = new float[N];  
  int n_iter = 100;
  Fx[0] = Fx[N-1] = 0.;
  Fy[0] = Fy[N-1] = 0.;  
   
  for(int n=0; n<n_iter; n++)
  {
    dt_path path = pix_path2dt_path(N, X, Y, 0.);
    visu_draw_dt_path(path);
    usleep(10000);

    // gets gradients
    for(int i=0; i<N; i++)
      if(i>0 && i<N-1) 
        get_pix_energy_gradient(X[i], Y[i], 0., Fx[i], Fy[i]);   // !!!!!!
    
    // Compute x_(t+1) from x_t (the same for y)
    optimize_path_iter(N, A, X, Fx);
    optimize_path_iter(N, A, Y, Fy);
  }
  
  delete[] Fy;
  delete[] Fx;  
  delete[] pivot;
  delete[] A;    
}
//------------------------------------------------------------------------------
void dt_map::optimize_path_iter(int N, float *A, float *V, float *F)
{
  float *tmp = new float[N];
  for(int i=0; i<N; i++)
    tmp[i] = GAMMA * V[i] - F[i];
  for(int i=0; i<N; i++)
  {
    V[i] = 0;
    for(int j=0; j<N; j++)
      V[i] += A[j*N+i]*tmp[j];
  }
  delete[] tmp;
}
//------------------------------------------------------------------------------
dt_path dt_map::pix_path2dt_path(int N, float *X, float *Y, double initial_angle)
{
  dt_path path;
  path.resize(N);
  
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
    path[i] = position_t(X[i]*DT_DIST_RESOL, Y[i]*DT_DIST_RESOL, angle);
  }
  
  return path;
}
//------------------------------------------------------------------------------
