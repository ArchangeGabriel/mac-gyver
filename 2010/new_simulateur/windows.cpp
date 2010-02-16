#include "string.h"

#include "windows.h"


void (*windows_t::render_world)() = NULL;
map<int, window_t*> windows_t::windows;

/*----------------------------------------------------------------------------*/
window_t::window_t(int w, int h, const char *name, GLfloat *pos, GLfloat *at)
{
  width = w;
  height = h;
  memcpy(position, pos, sizeof(GLfloat)*3);
  memcpy(lookat, at, sizeof(GLfloat)*3);  
  
  attached = false;
  
  glutInitWindowSize(width,height);
  glutCreateWindow(name);
  
  glutDisplayFunc(windows_t::display);
  glutReshapeFunc(windows_t::reshape);  
  glutReshapeWindow(width,height);
}
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
windows_t::windows_t(int *argc, char **argv)
{
  windows.clear();
 
  glutInit(argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
}
/*----------------------------------------------------------------------------*/
windows_t::~windows_t()
{
  for(unsigned int i=0; i<windows.size(); i++)
    delete windows[i];
}
/*----------------------------------------------------------------------------*/
void windows_t::add_window(int width, int height, const char *name, GLfloat *position, GLfloat *lookat)
{
  window_t *w = new window_t(width, height, name, position, lookat);
  windows[glutGetWindow()] = w;
}
/*----------------------------------------------------------------------------*/
void windows_t::init(const QDomDocument &)
{
  glEnable(GL_DEPTH_TEST); 
//  glEnable(GL_BLEND);  
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//  glEnable(GL_POLYGON_SMOOTH); 

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);  

  GLfloat ambient[]    = {0., 0., 0., 1.};
  GLfloat diffuse[]    = {1., 1., 1., 1.};
  GLfloat specular[]   = {1., 1., 1., 1.};  
  GLfloat position0[]  = {1., 1., 1., 0.};  
  
  glLightfv(GL_LIGHT0, GL_POSITION, position0);  
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glEnable(GL_LIGHTING); 
  glEnable(GL_LIGHT0);
  
  glEnable(GL_COLOR_MATERIAL);  
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);  



  /*GLfloat position1[]  = {-1., -1., -1., 0.};  
  GLfloat position2[]  = {-1.,  1., 1., 0.};  
  GLfloat position3[]  = {-1., -1., 1., 0.}; 

  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);  
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT2, GL_POSITION, position2);  
  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT3, GL_POSITION, position3);  
  glEnable(GL_LIGHT3); */  
}
/*----------------------------------------------------------------------------*/
void windows_t::display()
{
  window_t *win = windows[glutGetWindow()];
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  glLoadIdentity(); 	          
  gluLookAt(-1,-1,1,
            1.5,1.05,0.,
            0.,0.,1.);
/*  gluLookAt(1.5,1.05,1,
            1.5,1.05,0.,
            0.,1.,0.);*/
            
            

  render_world();
  
  glutSwapBuffers();  
}
/*----------------------------------------------------------------------------*/
void windows_t::reshape(int w, int h)
{
  window_t *win = windows[glutGetWindow()];

  if(win->attached && (w != win->width || h != win->height))
    glutReshapeWindow(win->width,win->height);
  else
  {
    glViewport(0,0,win->width,win->height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,float(win->width)/float(win->height),0.1,100);
    glMatrixMode(GL_MODELVIEW);	
  }
}
/*----------------------------------------------------------------------------*/
void windows_t::set_idle(void (*idle)())
{
  glutIdleFunc(idle);  
}
/*----------------------------------------------------------------------------*/
void windows_t::set_display(void (*render)())
{
  render_world = render;
}
/*----------------------------------------------------------------------------*/
void windows_t::start()
{
  glutMainLoop();
}
/*----------------------------------------------------------------------------*/
