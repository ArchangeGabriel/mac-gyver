#ifndef WINDOWS_H
#define WINDOWS_H

#include <map>
#include <GL/gl.h>
#include <GL/glut.h>
#include "xml.h"

using namespace std;

class window_t
{
  friend class windows_t;
  private:
  int width, height;
  GLfloat position[3];
  GLfloat lookat[3];
  
  bool attached;
    
  public:  
  window_t(int w, int h, const char *name, GLfloat *pos, GLfloat *at);
};

class windows_t
{
  private:
  static map<int, window_t*> windows;
  static void (*render_world)();
  
  public:
  windows_t(int *argc, char **argv);
  ~windows_t();  

  void add_window(int width, int height, const char *name, GLfloat *position, GLfloat *lookat); 
  void init(const QDomDocument &world);
    
  static void display();
  static void reshape(int w, int h);
  void set_idle(void (*idle)());
  void set_display(void (*r)());  

  void start();
};

#endif
