#ifndef WEBCAMH
#define WEBCAMH

#include "object.h"
#include "parse_params.h"
#include "../common/lin_alg.h"

class webcam_t : public params_t
{
  private: 
  GLfloat offset[3];
  GLfloat direction[3];  
  vector_t *N;  
  vector_t *T;  
  vector_t *G;  
  vector_t *pos;
  double *z;
  bool reshaped;
  GLubyte *pixels;
  bool done;
    
  static void make_scene();
  static void make_table();
    
  public:
  int W,H;  
  static simul_info_t* info;  
    
  webcam_t();
  ~webcam_t(); 
  
  static void init_scene(simul_info_t* _info);
  void create(vector_t *_N, vector_t *_T, vector_t *_G, vector_t *_pos, double *_z, double _h);  
  
  void init_params();
  void start();     // start displaying
  void stop();      // stop displaying
  static void display();
  static void no_display();  
  static void reshape(int width, int height);
  void makePicture();  
  int getPicture(void *pixels);
  GLuint LoadTexture(const char * filename, int width, int height);
  void FreeTexture(GLuint texture); 
};
#endif
