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
#define WEBCAM1 "/dev/video1"
#define WEBCAM2 "/dev/video2"
#endif

webcam_t video_front(WEBCAM1,WC1_resX,WC1_resY);
webcam_t video_top(WEBCAM2,WC2_resX,WC2_resY);

/*---------------------------------------------------------------------------*/
class orth_camera
{  
	private:
	double Xpixlength, Ypixlength;
	double z, vert_incl;  // négatif vers le bas	
  position_t pos;
	
	public:
  orth_camera(int resol_X, int resol_Y, double cam_z, double inclinaison_verticale, double demi_focale);
    
  void set_position(position_t &p);
  position_t get_position() const;
  
  point_t    pos2point(const vector_t &P, const double &Pz) const;
  vector_t   point2pos(const point_t &P, const double &Pz) const;
};
/*---------------------------------------------------------------------------*/
orth_camera::orth_camera(int resol_X, int resol_Y, double cam_z, double inclinaison_verticale, double demi_focale)
{
	Xpixlength = tan(demi_focale) / resol_X;
	Ypixlength = tan(demi_focale) / resol_Y;
	vert_incl = inclinaison_verticale;
	z = cam_z;
}
/*---------------------------------------------------------------------------*/
void orth_camera::set_position(position_t &p)
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
  
  // Position de P dans le référentiel de la caméra
	vector_t cam2P = P - pos.v();
  cam2P.rotate(-pos.a);
  
  // Composante verticale
  vector_t cam2P_vert(cam2P.norme(), Pz-z);
  cam2P_vert.rotate(-vert_incl);
  cam2P_vert = cam2P_vert / cam2P_vert.x;
  res.y = cam2P_vert.y / Ypixlength;
  
  // Composante horizontale
  double n2 = cam2P.norme2();
  double alpha = sqrt((n2 + pow(z-Pz, 2.)) / n2);
  cam2P = cam2P / cam2P.x;
  res.x = cam2P.y * alpha / Xpixlength;
  
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
void get_cylinder_corner(const orth_camera &cam, const vector_t &cylindrer, double z, double radius, double half_height, point_t &upper_left, point_t &lower_right)
{
	const double angle = cam.get_position().a;

	vector_t T(cos(angle+M_PI_2), sin(angle+M_PI_2));
	
	upper_left  = cam.pos2point(cylindrer+(T*radius), z+half_height);
	lower_right = cam.pos2point(cylindrer-(T*radius), z-half_height);
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
  video_front.do_capture();
  image_t img1 = video_front.get_image2();
  convert_yuv_to_rgb(img1);
  save_buff_to_bitmap("imgFront.bmp", WC1_resX, WC1_resY, img1.data);

  video_top.do_capture();
  image_t img2 = video_top.get_image2();
  convert_yuv_to_rgb(img2);
  save_buff_to_bitmap("imgTop.bmp", WC2_resX, WC2_resY, img2.data);

  return 52;
}
/*---------------------------------------------------------------------------*/


