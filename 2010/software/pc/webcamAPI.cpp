#define PC_INCLUDE
#include "webcamAPI.h"
#include "webcam.hpp"
#include "webcam_processing.hpp"
#include "webcam_utils.hpp"
#include "common.h"
#include "cinematik.h"
#include "sdl.h"

#include "../common/const.h"
#include "../common/bitmap.h"
#include "../common/simul.h"

#ifdef SIMULATION
#define WEBCAM1 "0"
#define WEBCAM2 "1"
#else
#define WEBCAM1 "/dev/video0"
#define WEBCAM2 "/dev/video1"
#endif

webcam_t video_front(WEBCAM1,WC1_resX,WC1_resY);
webcam_t video_top(WEBCAM2,WC2_resX,WC2_resY);

/*---------------------------------------------------------------------------*/
class orth_camera
{  
	private:
	int rX, rY;
	double focal_length;
	double z, vert_incl;  // négatif vers le bas	
  position_t pos;
	
	public:
  orth_camera(int resol_X, int resol_Y, double cam_z, double inclinaison_verticale, double demi_focale);
    
  void set_position(const position_t &p);
  position_t get_position() const;
  
  point_t    pos2point(const vector_t &P, const double &Pz) const;
  vector_t   point2pos(const point_t &P, const double &Pz) const;
};
/*---------------------------------------------------------------------------*/
orth_camera::orth_camera(int resol_X, int resol_Y, double cam_z, double inclinaison_verticale, double demi_focale)
{
  rX = resol_X;
  rY = resol_Y;
  focal_length = rY / (2. * tan(demi_focale * double(rY) / double(rX)));
	vert_incl = inclinaison_verticale;
	z = cam_z;
}
/*---------------------------------------------------------------------------*/
void orth_camera::set_position(const position_t &p)
{
	pos = p;
}
/*---------------------------------------------------------------------------*/
position_t orth_camera::get_position() const 
{
	return pos;
}
/*---------------------------------------------------------------------------*/
/*
Position d'un point P du terrain sur la webcam
*/
point_t orth_camera::pos2point(const vector_t &P, const double &Pz) const
{
  point_t res;
  
  double c,s;
  double p[3] = {P.x - pos.v().x,
                 P.y - pos.v().y,
                 Pz - z};
                 
  // XY - rotation
  c = cos(-pos.a);
  s = sin(-pos.a);
  double x1 = c * p[0] - s * p[1];
  double y1 = s * p[0] + c * p[1];
  p[0] = x1;
  p[1] = y1;
  
  // XZ - rotation
  c = cos(-vert_incl);
  s = sin(-vert_incl);  
  double x2 = c * p[0] - s * p[2];
  double z2 = s * p[0] + c * p[2];
  p[0] = x2;
  p[2] = z2;
  
  // Image projection
  res.x = rX/2 + p[1] * focal_length / p[0];
  res.y = rY/2 - p[2] * focal_length / p[0];
  
  return res;
}
/*---------------------------------------------------------------------------*/
/*
Position d'un point P de la webcam sur le terrain
*/
vector_t orth_camera::point2pos(const point_t &, const double &) const
{
	return vector_t(0,0);
}
/*---------------------------------------------------------------------------*/




/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* Utils
*/
void get_cylinder_corner(const orth_camera &cam, const vector_t &cylindrer, double z, double half_height, double radius, point_t &upper_left, point_t &lower_right)
{
	const double angle = cam.get_position().a;

	vector_t T(cos(angle-M_PI_2), sin(angle-M_PI_2));

  vector_t v = cylindrer+(T*radius);

	upper_left  = cam.pos2point(cylindrer+(T*radius), z+half_height);
	lower_right = cam.pos2point(cylindrer-(T*radius), z-half_height);
}

void draw_rect(const point_t &upper_left, const point_t &lower_right, int w, int h, uint16_t *data)
{
  int x1 = upper_left.x  < 0 ? 0 : upper_left.x;    x1 = x1 >= w ? (w-1) : x1;
  int x2 = lower_right.x < 0 ? 0 : lower_right.x;   x2 = x2 >= w ? (w-1) : x2;
  
  int y1 = upper_left.y  < 0 ? 0 : upper_left.y;    y1 = y1 >= h ? (h-1) : y1;
  int y2 = lower_right.y < 0 ? 0 : lower_right.y;   y2 = y2 >= h ? (h-1) : y2;
    
  for(int x = x1; x <= x2; x++)
  {
    int i0 = 3*((h-y1-1) * w + x); 
    data[i0+0] = 255;
    data[i0+1] = 0;
    data[i0+2] = 0;        
    
    i0 = 3*((h-y2-1) * w + x); 
    data[i0+0] = 255;
    data[i0+1] = 0;
    data[i0+2] = 0;        
  }
  
  for(int y = y1; y <= y2; y++)
  {
    int i0 = 3*((h-y-1) * w + x1); 
    data[i0+0] = 255;
    data[i0+1] = 0;
    data[i0+2] = 0;        
    
    i0 = 3*((h-y-1) * w + x2); 
    data[i0+0] = 255;
    data[i0+1] = 0;
    data[i0+2] = 0;   
  }  
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void webcam_init()
{
  #ifndef SIMULATION
  init_utils();
  #endif
    
  video_front.start();
  video_top.start();
}
/*---------------------------------------------------------------------------*/
int wc_reco_config()
{
  position_t robot_pos = cine_get_position();
  point_t upper_left, lower_right, middle;  

  // *** Front ***  
  position_t wc_front_pos(robot_pos.x+WC1_offsetX, 
                          robot_pos.y+WC1_offsetY,
                          robot_pos.a+atan(WC1_dirY/WC1_dirX));
  orth_camera wc_front(WC1_resX, WC1_resY, WC1_offsetZ, atan(WC1_dirZ/WC1_dirX), WC1_Hfocal/180.*M_PI); 
  wc_front.set_position(wc_front_pos);
  video_front.do_capture();
  image_t img1 = video_front.get_image2(); 
  convert_yuv_to_rgb(img1);  
  save_buff_to_bitmap("imgFront.bmp", WC1_resX, WC1_resY, img1.data);

  // Build camera
  position_t wc_top_pos(robot_pos.x+WC2_offsetX, 
                        robot_pos.y+WC2_offsetY,
                        robot_pos.a+atan(WC2_dirY/WC2_dirX));
  orth_camera wc_top(WC2_resX, WC2_resY, WC2_offsetZ, atan(WC2_dirZ/WC2_dirX), WC2_Hfocal/180.*M_PI);  
  wc_top.set_position(wc_top_pos);

  
  // Retrieve picture
  video_top.do_capture();
  image_t img2 = video_top.get_image2();
  
  // Retrieve the zone to look for
  vector_t cylinder(1.95,1.722);  // 3
  //vector_t cylinder(1.95,1.222);  // 4
  get_cylinder_corner(wc_top, cylinder, 0.075, 0.075, 0.025, upper_left, lower_right);
  middle = wc_top.pos2point(cylinder, 0.075);
  printf("m %d %d\n", middle.x, middle.y);
  printf("ul %d %d\n", upper_left.x, upper_left.y);
  printf("lr %d %d\n", lower_right.x, lower_right.y);    
  
  // Draw output
  convert_yuv_to_rgb(img2);
  draw_rect(upper_left, lower_right, WC2_resX, WC2_resY, img2.data);  
  draw_rect(middle, middle, WC2_resX, WC2_resY, img2.data);  
  save_buff_to_bitmap("imgTop.bmp", WC2_resX, WC2_resY, img2.data);
  
  return 52;
}
/*---------------------------------------------------------------------------*/


