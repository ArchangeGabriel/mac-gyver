#define PC_INCLUDE
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include "webcamAPI.h"
#include "webcam.hpp"
#include "webcam_processing.hpp"
#include "webcam_utils.hpp"
#include "common.h"
#include "cinematik.h"
#include "strategie.h"
#include "sdl.h"

#include "../common/const.h"
#include "../common/bitmap.h"
#include "../common/simul.h"

using namespace std;

#ifdef SIMULATION
#define WEBCAM1 "0"
#define WEBCAM2 "1"
#else
#define WEBCAM1 "/dev/video1"
#define WEBCAM2 "/dev/video2"
#endif

/*---------------------------------------------------------------------------*/
class orth_camera : public webcam_t
{  
	protected:
	double focal_length;
	double z, vert_incl;  // négatif vers le bas	
	double offX, offY, offA;
  position_t pos;
	
	public:
  orth_camera(
    const char *name, 
    const std::string& dev, 
    size_t w, 
    size_t h, 
    double dx, 
    double dy, 
    double da,
    double cam_z, 
    double inclinaison_verticale, 
    double demi_focale
  );
    
  void set_robot_pos(const position_t &p);
  position_t get_pos() const;
  
  point_t    pos2point(const vector_t &P, const double &Pz) const;
  vector_t   point2pos(const point_t &P, const double &Pz) const;
};
/*---------------------------------------------------------------------------*/
orth_camera::orth_camera(
    const char *name, 
    const std::string& dev, 
    size_t w, 
    size_t h, 
    double dx, 
    double dy, 
    double da,
    double cam_z, 
    double inclinaison_verticale, 
    double demi_focale
  ):webcam_t(name,dev,w,h),offX(dx),offY(dy),offA(da)
{
  focal_length = m_height / (2. * tan(demi_focale * double(m_height) / double(m_width)));
	vert_incl = inclinaison_verticale;
	z = cam_z;
}
/*---------------------------------------------------------------------------*/
void orth_camera::set_robot_pos(const position_t &p)
{
  double c = cos(p.a);
  double s = sin(p.a);
	pos = p;
	pos.x += offX*c - offY*s;
	pos.y += offX*s + offY*c;
	pos.a += offA;		
}
/*---------------------------------------------------------------------------*/
position_t orth_camera::get_pos() const 
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
  res.x = m_width/2  + p[1] * focal_length / p[0];
  res.y = m_height/2 - p[2] * focal_length / p[0];
  
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

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class config_cylinder
{
  private:
  vector_t pos;
  
  int initial_surf;
  int clip_one(int val, int max);
  double proba_black, proba_white;
  
  public:
  int i,j;  
  point_t upper_left, lower_right;
  int visible_surf;
  enum {color_white, color_black};  
    
  config_cylinder(int _i, int _j);
  const vector_t& get_pos() {return pos;}
  bool clip_cylinder(int w, int h);
  void compute_color(image_t &img);
  double get_score_white();
  double get_score_black();  
  void flip_corners(int w, int h);
};

config_cylinder::config_cylinder(int _i, int _j)
{
  i = _i;
  j = _j;
  pos.x = 0.15 + 0.45*i;
  if(i<4)
    pos.y = 2.1 - 1.378 + 0.5*j + 0.25*i;
  else
    pos.y = 2.1 - 1.378 + 0.5*j + 0.25*(6-i);
}

int config_cylinder::clip_one(int val, int max)
{
  val = val < 0 ? 0 : val;
  val = val > max ? max : val;
  return val;
}

bool config_cylinder::clip_cylinder(int w, int h)
{
  initial_surf = (lower_right.x - upper_left.x + 1) * (lower_right.y - upper_left.y + 1);
 
  upper_left.x  = clip_one(upper_left.x,  w-1);
  upper_left.y  = clip_one(upper_left.y,  h-1);  
  lower_right.x = clip_one(lower_right.x, w-1);
  lower_right.y = clip_one(lower_right.y, h-1);
  
  visible_surf = (lower_right.x - upper_left.x + 1) * (lower_right.y - upper_left.y + 1);
  
  return double(visible_surf) / double(initial_surf) > 0.3;
}

void config_cylinder::compute_color(image_t &img)
{
  int _white = 0;
  int _black = 0;
  int _red = 0;   
  
  for(int x = upper_left.x; x <= lower_right.x; x++)
    for(int y = upper_left.y; y <= lower_right.y; y++)
    {       
      if(filter_red(img.my_pixel(x, y)))
        _red++;
      else if(filter_black(img.my_pixel(x, y)))
        _black++;
      else if(filter_white(img.my_pixel(x, y)))
        _white++;
      //convert_yuv_to_rgb(img.my_pixel(x, y));
    } 
       
  double new_proba_white, new_proba_black;
  if(_red == visible_surf)
  {
    new_proba_white = 0;
    new_proba_black = 0;
  }
  else
  {
    new_proba_white = double(_white) / double(visible_surf - _red);
    new_proba_black = double(_black) / double(visible_surf - _red);
  }
  
  if(fabs(proba_white - proba_black) < fabs(new_proba_white - new_proba_black))
  {
    proba_white = new_proba_white;
    proba_black = new_proba_black;
  }  
}

double config_cylinder::get_score_white()
{
/*  if(proba_white > proba_black)
    return proba_white;
  else
    return 1 - proba_black;*/
  if(proba_white > proba_black)
    return 1;
  else
    return 0;      
}

double config_cylinder::get_score_black()
{
/*  if(proba_white > proba_black)
    return 1 - proba_white;
  else
    return proba_black; */ 
  if(proba_white > proba_black)
    return 0;
  else
    return 1;     
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class config_decide
{
  vector<point_t> c1;
  vector<point_t> c2;
  int config;
  
  public:
  void setup_config_lateral()
  {
    config = 0;
    c1.resize(9);
    c2.resize(9);
    for(int i=0; i<9; i++)
      switch(i+1)
      {
        case 1:
          c1[i].x = 0;
          c1[i].y = 1;
          c2[i].x = 1;
          c2[i].y = 1;
        break;
        case 2:
          c1[i].x = 0;
          c1[i].y = 0;
          c2[i].x = 1;
          c2[i].y = 1;      
        break;
        case 3:
          c1[i].x = 0;
          c1[i].y = 2;
          c2[i].x = 1;
          c2[i].y = 1;      
        break;
        case 4:
          c1[i].x = 0;
          c1[i].y = 2;
          c2[i].x = 1;
          c2[i].y = 0;      
        break;
        case 5:
          c1[i].x = 0;
          c1[i].y = 0;
          c2[i].x = 1;
          c2[i].y = 0;            
        break;
        case 6:
          c1[i].x = 0;
          c1[i].y = 1;
          c2[i].x = 1;
          c2[i].y = 0;            
        break;
        case 7:
          c1[i].x = 0;
          c1[i].y = 1;
          c2[i].x = 2;
          c2[i].y = 0;                  
        break;
        case 8:
          c1[i].x = 0;
          c1[i].y = 0;
          c2[i].x = 2;
          c2[i].y = 0;                  
        break;
        case 9:                                          
          c1[i].x = 0;
          c1[i].y = 2;
          c2[i].x = 2;
          c2[i].y = 0;                  
        break;
      }
  }

  void setup_config_central()
  {
    config = 1;
    c1.resize(4);
    c2.resize(4);
    for(int i=0; i<4; i++)
      switch(i+1)
      {
        case 1:
          c1[i].x = 3;
          c1[i].y = 0;
          c2[i].x = 1;
          c2[i].y = 2;
        break;
        case 2:
          c1[i].x = 3;
          c1[i].y = 0;
          c2[i].x = 2;
          c2[i].y = 1;      
        break;
        case 3:
          c1[i].x = 3;
          c1[i].y = 1;
          c2[i].x = 1;
          c2[i].y = 2;      
        break;
        case 4:
          c1[i].x = 3;
          c1[i].y = 1;
          c2[i].x = 2;
          c2[i].y = 1;      
        break; 
      }
  } 
  
  int get_config(vector<config_cylinder> &cylinders, double &proba)
  {
    double proba_lat, proba_cen;
    
    setup_config_lateral();
    int config_lat = get_part_config(cylinders, proba_lat);
    setup_config_central();
    int config_cen = get_part_config(cylinders, proba_cen);
    proba = proba_lat*proba_cen;
    return config_lat*10+config_cen;
  }
  
  int get_part_config(vector<config_cylinder> &cylinders, double &proba)
  {
    int n;
    int x,y;
    double *scores = new double[c1.size()];
        
    for(unsigned int k=0; k<c1.size(); k++)
    {
      scores[k] = 0;
      n = 0;
      for(unsigned int i=0; i<cylinders.size(); i++)
      {
        x = cylinders[i].i > 3 ? (6 - cylinders[i].i) : cylinders[i].i;
        y = cylinders[i].j;
        if((x == c1[k].x && y == c1[k].y) ||
           (x == c2[k].x && y == c2[k].y))
        {
          scores[k] += cylinders[i].get_score_black();
          n++;
        } 
        else 
        {
          if((config == 0 && y < 3-x) || (config == 1 && y >= 3-x))
          {
            scores[k] += cylinders[i].get_score_white();
            n++;
          }
        }
      }
      scores[k] /= n;
    }
    
    double max = scores[0];
    int config = 0;
//    printf("config %d: %f\n", 0, scores[0]);
    for(unsigned int k=1; k<c1.size(); k++)
    {
//      printf("config %d: %f\n", k, scores[k]);
      if(scores[k] > max)
      {
        max = scores[k];
        config = k;
      }
    }
    
    delete[] scores;
    
    proba = max;
    return config+1;
  }
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

orth_camera *WC[2] = {new orth_camera("front",
                                WEBCAM1,
                                WC1_resX,
                                WC1_resY,
                                WC1_offsetX,
                                WC1_offsetY,
                                atan(WC1_dirY/WC1_dirX),
                                WC1_offsetZ,
                                atan(WC1_dirZ/WC1_dirX),
                                WC1_Hfocal/180.*M_PI
                               ), 
                   new orth_camera("top",
                                WEBCAM2,
                                WC2_resX,
                                WC2_resY,
                                WC2_offsetX,
                                WC2_offsetY,
                                atan(WC2_dirY/WC2_dirX),
                                WC2_offsetZ,
                                atan(WC2_dirZ/WC2_dirX),
                                WC2_Hfocal/180.*M_PI
                               )};
enum {FRONT, TOP, wc_count};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* Utils
*/
void get_cylinder_corner(const orth_camera &cam, const vector_t &cylindrer, double z, double half_height, double radius, point_t &upper_left, point_t &lower_right)
{
	const double angle = cam.get_pos().a;

	vector_t T(cos(angle-M_PI_2), sin(angle-M_PI_2));

  vector_t v = cylindrer+(T*radius);

	upper_left  = cam.pos2point(cylindrer+(T*radius), z+half_height);
	lower_right = cam.pos2point(cylindrer-(T*radius), z-half_height);
	
	const int w = cam.get_width();
	const int h = cam.get_height();	
  const int flip_x = w - upper_left.x;   // les webcam sont montées à l'envers
  const int flip_y = h - upper_left.y;
  upper_left.x = w - lower_right.x;
  upper_left.y = h - lower_right.y;
  lower_right.x = flip_x;
  lower_right.y = flip_y;  	
}

void draw_rect(const point_t &upper_left, const point_t &lower_right, int w, int h, uint16_t *data)
{   
  const int Y = 180;
  const int U = 20;
  const int V = 250;
  for(int x = upper_left.x; x <= lower_right.x; x++)
  {
    int i0 = 3*((h-upper_left.y-1) * w + x); 
    data[i0+0] = Y;
    data[i0+1] = U;
    data[i0+2] = V;        
    
    i0 = 3*((h-lower_right.y-1) * w + x); 
    data[i0+0] = Y;
    data[i0+1] = U;
    data[i0+2] = V;        
  }
  
  for(int y = upper_left.y; y <= lower_right.y; y++)
  {
    int i0 = 3*((h-y-1) * w + upper_left.x); 
    data[i0+0] = Y;
    data[i0+1] = U;
    data[i0+2] = V;        
    
    i0 = 3*((h-y-1) * w + lower_right.x); 
    data[i0+0] = Y;
    data[i0+1] = U;
    data[i0+2] = V;   
  }  
}
/*---------------------------------------------------------------------------*/
int wc_reco_config()
{
  return 22;
  position_t robot_pos = cine_get_position();
  image_t img(image_t::yuv_format,0,0);
  double proba;
  config_decide cd;
  int config;

  vector<config_cylinder> cylinders;
  
  vector<double> config_lat;
  config_lat.resize(9);
  vector<double> config_cen;  
  config_cen.resize(4);
    
  if(get_color() == clBLUE)
  {
    cylinders.push_back(config_cylinder(0,0));
    cylinders.push_back(config_cylinder(0,1));
    cylinders.push_back(config_cylinder(0,2));    
    cylinders.push_back(config_cylinder(1,1));     
    cylinders.push_back(config_cylinder(1,2));         
    cylinders.push_back(config_cylinder(2,0));     
    cylinders.push_back(config_cylinder(2,1));         
    cylinders.push_back(config_cylinder(3,0));     
    cylinders.push_back(config_cylinder(3,1));         
    cylinders.push_back(config_cylinder(4,1));    
  } 
  else
  {
    cylinders.push_back(config_cylinder(6,0));
    cylinders.push_back(config_cylinder(6,1));
    cylinders.push_back(config_cylinder(5,0));     
    cylinders.push_back(config_cylinder(5,1));             
    cylinders.push_back(config_cylinder(4,0));         
    cylinders.push_back(config_cylinder(4,1));     
    cylinders.push_back(config_cylinder(3,0));
    cylinders.push_back(config_cylinder(3,1));         
  }   
  
  for(int k=0; k<wc_count; k++)
  {  
    if(!WC[k]->started)
      return 0;
    WC[k]->set_robot_pos(robot_pos);
    position_t p = WC[k]->get_pos();
    WC[k]->capture(img);
    for(unsigned int i=0; i<cylinders.size(); i++)
    {
      get_cylinder_corner(*WC[k], cylinders[i].get_pos(), 0.075, 0.075, 0.025, cylinders[i].upper_left, cylinders[i].lower_right);
      if(cylinders[i].clip_cylinder(WC[k]->get_width(), WC[k]->get_height()))
      {
        cylinders[i].compute_color(img);
        //printf("wc: %d coord(%d,%d) score: (%f,%f)\n", k, cylinders[i].i, cylinders[i].j, cylinders[i].get_score_white(),cylinders[i].get_score_black());
        //draw_rect(cylinders[i].upper_left, cylinders[i].lower_right, WC[k]->get_width(), WC[k]->get_height(), img.data);
      }
    }
    
    char file[400];
    sprintf(file, "img_%s.bmp", WC[k]->get_name());
    convert_yuv_to_rgb(img);
    save_buff_to_bitmap(file, WC[k]->get_width(), WC[k]->get_height(), img.data);
  }
  
  config = cd.get_config(cylinders, proba);  
  printf("Detected config = %d\n", config);
  return config;
}
/*---------------------------------------------------------------------------*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void* wc_MainLoop(void* _path)
{
  init_utils();

  for(int i=0; i<wc_count; i++)
  {
    WC[i]->start();
    if(!WC[i]->started) 
    {
      printf("WEBCAM INIT FAILED!\n");
      return NULL;
    }
  }
  
  position_t pos;
  char *path = (char*)_path;
  int img_num;
  
  if(path)
  {
    if(mkdir(path, 766) && errno != EEXIST)
    {
      fprintf(stderr, "ECHEC DE LA CREATION DU REPERTOIRE DE SAUVEGARDE DES PHOTOS! (code erreur: %d)\n", errno);
      exit(errno);
    }
    if(path[strlen(path)-1] == '/')
      path[strlen(path)-1] = '\0';
  }
  
  // Attend que le robot soit prêt
  while(!strat_is_ready()) usleep(10000);

  while(true) 
  {
    if(path && strat_is_started())
    {
      char file[400];
      image_t img(image_t::yuv_format,0,0);
      for(int i=0; i<wc_count; i++)
      {
        sprintf(file, "%s/%s_%03d.bmp", path, WC[i]->get_name(), img_num);
        WC[i]->capture(img);
        WC[i]->set_robot_pos(cine_get_position());
        pos = WC[i]->get_pos();
        convert_yuv_to_rgb(img);
        save_buff_to_bitmap(file, WC[i]->get_width(), WC[i]->get_height(), img.data);
        //save_image(img, file); 
        
        sprintf(file, "%s/%s_%03d.info", path, WC[i]->get_name(), img_num);
        ofstream f;
        f.open(file);
        f << pos.x << endl << pos.y << endl << pos.a;
        f.close();
      }
      img_num++;
    }
    sleep(2);
  }    
  return NULL;
}
/*---------------------------------------------------------------------------*/
void wc_capture(int id, image_t &img, int &w, int &h)
{
  WC[id]->capture(img);
  w = WC[id]->get_width();
  h = WC[id]->get_height();  
}
/*---------------------------------------------------------------------------*/
int wc_nb_cam()
{
  return wc_count;
}
/*---------------------------------------------------------------------------*/

