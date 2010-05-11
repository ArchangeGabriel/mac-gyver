#ifndef CAMERAH
#define CAMERAH

#include <stdint.h>
#include "object.h"
#include "parse_params.h"
#include "../common/lin_alg.h"

class camera_t : public params_t
{
  private: 
  vector_t *N;  
  vector_t *T;  
  vector_t *G;  
  vector_t *pos;
  double *z;
  bool reshaped;
  bool capture;
  bool done;
    
  static void make_scene();
    
  public:
  GLfloat offset[3];
  GLfloat direction[3]; 
  double h_focal;     
  uint16_t *pixels;
  int W,H;  
  static simul_info_t* info;  
    
  camera_t();
  ~camera_t(); 
  
  static void init_scene(simul_info_t* _info);
  void create(vector_t *_N, vector_t *_T, vector_t *_G, vector_t *_pos, double *_z, double _h);  
  
  void init_params();
  void start();     // start displaying
  void stop();      // stop displaying
  static void display();
  static void no_display();  
  static void reshape(int width, int height);
  void makePicture();  
  int getPicture(uint16_t **pix);
  GLuint LoadTexture(const char * filename, int width, int height);
  void FreeTexture(GLuint texture); 
};
#endif
