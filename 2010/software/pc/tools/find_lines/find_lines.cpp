#include <assert.h>
#include <vector>
#include <string.h>
#include "../../common.h"
#include "../../webcam_common.hpp"
#include "../../webcam_filter.hpp"
#include "../../webcam_utils.hpp"
#include "../../../common/bitmap.h"

using namespace std;

class gray_img
{
  private:
  int width, height;
   
  public:  
  typedef double pixel_t;  
  pixel_t **pix;
  
  void make(int w, int h)
  {
    width = w;
    height = h;
    pix = new pixel_t*[w];
    for(int i=0; i<w; i++)
    {
      pix[i] = new pixel_t[h];
      memset(pix[i], 0, sizeof(pixel_t)*h);
    }
  }  
  
  gray_img() {}
  gray_img(int w, int h) {make(w,h);}
  gray_img(const image_t &img, int compo) {load_from(img, compo);}
    
  inline int get_width()  const {return width;} 
  inline int get_height() const {return height;}
  
  void load_from(const image_t &img, int compo)
  {
    make(img.w, img.h);
    
    for(int x=0; x<width; x++) 
      for(int y=0; y<height; y++)    
        pix[x][y] = img.pixel(x,height-y-1)[compo];
  }
  
  double interpolate(double x, double y) const
  {
    if(x<0. || y<0. || x>width-1 || y>height-1) return 0;
    
    int _x = x;
    int _y = y;
    
    double wx = 1. - double(x-_x); 
    double wy = 1. - double(y-_y);
    
    if(wx == 1. && wy == 1.)
      return pix[_x][_y];
    else if(wx == 1.)
      return pix[_x][_y]*wy + pix[_x][_y+1]*(1.-wy);
    else if(wy == 1.)
      return pix[_x][_y]*wx + pix[_x+1][_y]*(1.-wx)*wy;
    else
      return pix[_x][_y]*wx*wy + pix[_x+1][_y]*(1.-wx)*wy + pix[_x][_y+1]*wx*(1.-wy) + pix[_x+1][_y+1]*(1.-wx)*(1.-wy);
  }
  
  void save_to_file(const char *file)
  {
    FILE *F= fopen(file,"w+");
    save_bitmap_header(F, width, height);

    int max = 0;
    for(int y=0; y<height; y++)
      for(int x=0; x<width; x++)
        if(pix[x][height-1-y] > max)
          max = pix[x][height-1-y];    
          
    for(int y=0; y<height; y++)
    {
      for(int x=0; x<width; x++)
      {
        unsigned char p= pix[x][height-1-y]*255./max;
        fwrite(&p,1,1,F);   // 2
        fwrite(&p,1,1,F);   // 1
        fwrite(&p,1,1,F);   // 0
      }
      unsigned int x = 3 * width;
      int n = 0;
      while(x%4)
      {
        fwrite(&n,1,1,F);
        x++;
      }
    }              
    fclose(F);  
  }
};

/*----------------------------------------------------------------------------*/

void compute_gradient(const gray_img &from, gray_img &grad_x, gray_img &grad_y, gray_img &grad_norm);
vector<point_t> compute_max(const gray_img &grad_x, const gray_img &grad_y, const gray_img &grad_norm, gray_img &dest, double small_th, double high_th);
void compute_contours(const gray_img &source, vector<point_t> &max, double threshold, gray_img &dest);
void follow_level_line(const gray_img &source, gray_img &dest, double threshold, int x, int y, int from_x, int from_y);
void compute_lines(const gray_img &source, const gray_img &dest);


#define SMALL_TH  10.
#define HIGH_TH  40.

/*----------------------------------------------------------------------------*/
int main(int , char**)
{
  init_utils();
  image_t img(image_t::rgb_format, 0, 0);
  
  if(!load_image(img, "img_cropped.bmp"))
  {
    fprintf(stderr, "Could not find the image!\n");
    return 1;
  }

  image_t blurred_img(image_t::rgb_format, img.w, img.h);  
  filter_image(img,blurred_img, blur_filter);  

  gray_img blurred(blurred_img,1);
  blurred.save_to_file("blurred.bmp");
  
  gray_img grad_x, grad_y, grad_norm;
  compute_gradient(blurred, grad_x, grad_y, grad_norm);
  grad_x.save_to_file("grad_x.bmp");
  grad_y.save_to_file("grad_y.bmp");
  grad_norm.save_to_file("grad_norm.bmp");    
    
  gray_img nms;
  vector<point_t> max = compute_max(grad_x, grad_y, grad_norm, nms, SMALL_TH, HIGH_TH);
  nms.save_to_file("nms.bmp");   
  
  gray_img contours;
  compute_contours(grad_norm, max, SMALL_TH, contours);
  contours.save_to_file("contours.bmp");   
      
  return 0;
}
/*----------------------------------------------------------------------------*/
void compute_gradient(const gray_img &from, gray_img &grad_x, gray_img &grad_y, gray_img &grad_norm)
{
  const int w = from.get_width();
  const int h = from.get_height();  
  
  grad_x.make(w,h);
  grad_y.make(w,h);
  grad_norm.make(w,h);    
  
  for(int x=0; x<w; x++)
    for(int y=0; y<h; y++)
    {
      int x1 = x<(w-1) ? x+1 : w-1;
      int x2 = x>0     ? x-1 : 0;
      int dx = from.pix[x1][y] - from.pix[x2][y];
      
      int y1 = y<(h-1) ? y+1 : y-1;
      int y2 = y>0     ? y-1 : 0;
      int dy = from.pix[x][y1] - from.pix[x][y2];
      
      grad_x.pix[x][y] = dx;
      grad_y.pix[x][y] = dy;      
      grad_norm.pix[x][y] = sqrt(dx*dx + dy*dy);
    }
}
/*----------------------------------------------------------------------------*/
vector<point_t> compute_max(const gray_img &grad_x, const gray_img &grad_y, const gray_img &grad_norm, gray_img &dest, double small_th, double high_th)
{
  const int w = grad_norm.get_width();
  const int h = grad_norm.get_height();  
  double _x, _y, _v;  
  
  dest.make(w,h);
  
  vector<point_t> max;
  
  for(int x=0; x<w; x++)
    for(int y=0; y<h; y++)
    {
      if(grad_norm.pix[x][y] > 0)
      {
        _x = double(x) + grad_x.pix[x][y]/grad_norm.pix[x][y];
        _y = double(y) + grad_y.pix[x][y]/grad_norm.pix[x][y];
        _v = grad_norm.interpolate(_x, _y);
        
        if(_v > grad_norm.pix[x][y])
        {
          dest.pix[x][y] = 0;
          continue;
        }

        _x = double(x) - grad_x.pix[x][y]/grad_norm.pix[x][y];
        _y = double(y) - grad_y.pix[x][y]/grad_norm.pix[x][y];
        _v = grad_norm.interpolate(_x, _y);      

        if(_v > grad_norm.pix[x][y])
        {
          dest.pix[x][y] = 0;
          continue;
        }

        if(grad_norm.pix[x][y] >= high_th)
        {
          dest.pix[x][y] = 1;
          max.push_back(point_t(x,y));
        }
        else if(grad_norm.pix[x][y] >= small_th)
          dest.pix[x][y] = 0.2;
        else
          dest.pix[x][y] = 0.;
      }
      else
        dest.pix[x][y] = 0.;
    }
  return max;
}
/*----------------------------------------------------------------------------*/
void compute_contours(const gray_img &source, vector<point_t> &max, double threshold, gray_img &dest)
{
  const int w = source.get_width();
  const int h = source.get_height();  
  dest.make(w,h);  

  for(unsigned int i=0; i<max.size(); i++)
  {
    if(dest.pix[max[i].x][max[i].y] == 0.)
      follow_level_line(source, dest, threshold, max[i].x, max[i].y, max[i].x, max[i].y);
  }
}
/*----------------------------------------------------------------------------*/
void follow_level_line(const gray_img &source, gray_img &dest, double threshold, int x, int y, int from_x, int from_y)
{
  if(dest.pix[x][y] == 1.)
    return;
  if(source.pix[x][y] < threshold)
    return;
  dest.pix[x][y] = 1;

  const int dx[] = {-1, 0, 1, 1, 1, 0,-1,-1};
  const int dy[] = {-1,-1,-1, 0, 1, 1, 1, 0};  
  
  int x1, y1;
  int xm = -1;
  int ym = -1;
  double max = -1.;
  for(int k=0; k<8; k++)
  {
    x1 = x+dx[k];
    y1 = y+dy[k];
    if(x1 < 0 || y1 < 0 || x1 >= source.get_width() || y1 >= source.get_height())
      continue;
    if(x1 == from_x && y1 == from_y)
      continue;
    if(source.pix[x1][y1] == 0.)
      continue;
    if(source.pix[x1][y1] > max)
    {
      xm = x1;
      ym = y1;      
      max = source.pix[x1][y1];
    }
  }
  
  if(xm != -1)
    follow_level_line(source, dest, threshold, xm, ym, x, y);
}



/*----------------------------------------------------------------------------*/
#define maxTHETA  85   // maximum de verticalité
#define resolA    1000 // résolution en a (y=ax+b)
#define resolB    3000 // résolution en b (y=ax+b)

point_t get_hough_coord(int a, int b, int maxA, int minB, int maxB)
{
  point_t c;
  c.x = double(a+maxA)/double(2*maxA)*double(resolA-1);
  c.y = double(b-minB)/(2.*double(maxA))*double(resolA-1);
}

/*----------------------------------------------------------------------------*/
void compute_lines(const gray_img &source, const gray_img &dest)
{
  const int w = source.get_width();
  const int h = source.get_height(); 
  const int maxA = ceil(tan(maxTHETA));
  const int minB = -maxA*(w-1);
  const int maxB =  maxA*(w-1)+(h-1);
  
  dest.make(resolA, resolB);
  
  for(int x=0; x<w; x++)
    for(int y=0; y<h; y++)
      if(source.pix[x][y])
      {
        int b = (-x)*(-maxA)
      }
}
/*----------------------------------------------------------------------------*/
