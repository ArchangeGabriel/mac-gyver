#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <unistd.h>

#include "camera.h"
#include "the_cup/the_playground.h"
#include "../common/const.h"

simul_info_t* camera_t::info;  

//------------------------------------------------------------------------------
camera_t::camera_t()
{
  pixels = NULL;
}
//------------------------------------------------------------------------------
camera_t::~camera_t()
{
  if(pixels)
    delete pixels;
}
//------------------------------------------------------------------------------
void camera_t::init_params()
{
  printf("  --> Loading camera's params...");
  fflush(stdout);
  W            = int_param_value("ResolWidth");  
  H            = int_param_value("ResolHeight"); 
  offset[0]    = float_param_value("OffsetX");  
  offset[1]    = float_param_value("OffsetY");   
  offset[2]    = float_param_value("OffsetZ");    
  direction[0] = float_param_value("DirX");  
  direction[1] = float_param_value("DirY");   
  direction[2] = float_param_value("DirZ");
  h_focal      = float_param_value("Hfocal");   
  
  reshaped = false;
  done = false;
  capture = false;
  
  int size = 3*W*H;
  pixels = new uint16_t[size];   
  
  printf("ok\n");
  fflush(stdout);   
}
//------------------------------------------------------------------------------
void camera_t::init_scene(simul_info_t* _info)
{
  printf("GLUT initialisation...          ");
  
  info = _info;

  GLfloat ambient[]   = {0.15, 0.15, 0.15, 1.0};
  GLfloat diffuse[]   = {0.2, 0.2, 0.2, 1.0};
  GLfloat position0[]  = { 1.,  1., 5., 0.};        
  GLfloat position1[]  = { 1., -1., 5., 0.};  
  GLfloat position2[]  = {-1.,  1., 5., 0.};  
  GLfloat position3[]  = {-1., -1., 5., 0.}; 

  glEnable(GL_DEPTH_TEST);   	
//  glEnable(GL_BLEND);  
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//  glEnable(GL_POLYGON_SMOOTH); 

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

  glEnable(GL_COLOR_MATERIAL);  
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
     
  glEnable(GL_LIGHTING); 
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position0);  
  glEnable(GL_LIGHT0);
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
  glEnable(GL_LIGHT3);
	   
  printf("ok\n");
  fflush(stdout);  
}
//------------------------------------------------------------------------------
void camera_t::create(vector_t *_N, vector_t *_T, vector_t *_G, vector_t *_pos, double *_z, double /*_h*/)
{ 
  N = _N;
  T = _T;
  G = _G;
  pos = _pos; 
  z = _z;
  //offset[2] -= _h/2.;
}
//------------------------------------------------------------------------------
void camera_t::start()
{
  printf("Camera initialisation (GLUT)... ");
  fflush(stdout); 

  info -> current_webcam = this;
      
  glutReshapeWindow(W,H);  
  reshaped = false;
  done = false;  

  printf("ok\n");
  fflush(stdout);  

  glutDisplayFunc(camera_t::display); 
  glutReshapeFunc(camera_t::reshape);
}
//------------------------------------------------------------------------------
void camera_t::stop()
{
  glutDisplayFunc(camera_t::no_display);     
}
//------------------------------------------------------------------------------
void camera_t::reshape(int , int )
{
  if(!info -> current_webcam) return;
  camera_t *WC = (camera_t*) info -> current_webcam;

  glViewport(0,0,WC->W,WC->H);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(WC->h_focal*2./float(WC->W)*float(WC->H),float(WC->W)/float(WC->H),0.1,100); 	//Pour les explications, lire le tutorial sur OGL et win
  glMatrixMode(GL_MODELVIEW);	
  
  WC->reshaped = true; 
}
//------------------------------------------------------------------------------
void camera_t::display()
{  
  make_scene();
  
  glutSwapBuffers();
  
  camera_t *WC = (camera_t*) info -> current_webcam;
  if(WC->capture)
  {
    glReadPixels(0,0,WC->W,WC->H,GL_RGB,GL_UNSIGNED_SHORT,WC->pixels);
    WC->capture = false;
    WC->done = true;
  }
}
//------------------------------------------------------------------------------
void camera_t::no_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	//Efface le frame buffer et le Z-buffer
  glMatrixMode(GL_MODELVIEW); 	                        //Choisit la matrice MODELVIEW
  glLoadIdentity(); 	                                  //Réinitialise la matrice
  gluLookAt(0.,0.,0.,
            1.,0.,0.,
            0.,0.,1.);
 
  glutSwapBuffers();
}
//------------------------------------------------------------------------------
void camera_t::make_scene()
{
  if(!info -> current_webcam) return;
  camera_t *WC = (camera_t*) info -> current_webcam;
  if(WC->reshaped)
  {
    WC->reshaped = false;
    glutReshapeWindow(WC->W,WC->H);
  }
   
  vector_t C = *WC->pos - *WC->G;
  
  GLfloat x = C.x + WC->offset[0]*WC->N->x + WC->offset[1]*WC->T->x;
  GLfloat y = C.y + WC->offset[0]*WC->N->y + WC->offset[1]*WC->T->y;
  GLfloat z = -WC->offset[2]; // -*WC->z // distance au sol  
  GLfloat dx = WC->direction[0]*WC->N->x + WC->direction[1]*WC->T->x;
  GLfloat dy = WC->direction[0]*WC->N->y + WC->direction[1]*WC->T->y;
  GLfloat dz = -WC->direction[2];    
  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	//Efface le frame buffer et le Z-buffer
  	
  glMatrixMode(GL_MODELVIEW); 	                        //Choisit la matrice MODELVIEW
  glLoadIdentity(); 	                                  //Réinitialise la matrice
  gluLookAt(x,y,z,
            x+dx,y+dy,z+dz,
            0.,0.,-1.);
            
  draw_playground_3D();
  
 for(unsigned int i=0;i<info->objets_mobiles.size();i++)
    if(info->objets_mobiles[i]->type != OBJ_ROBOT &&
       !info->objets_mobiles[i]->hide)
      info->objets_mobiles[i]->draw3D();  

  for(unsigned int i=0;i<info->objets_mobiles.size();i++)
    if(info->objets_mobiles[i]->type == OBJ_ROBOT &&
       !info->objets_mobiles[i]->hide)    
      info->objets_mobiles[i]->draw3D();   

  for(unsigned int i=0;i<info->objets_fixes.size();i++)
    if(!info->objets_fixes[i]->hide)    
      info->objets_fixes[i]->draw3D();     
                       
  WC->done = true;
}
//------------------------------------------------------------------------------
void camera_t::makePicture()
{ 
  vector_t C = *pos - *G;
  
  GLfloat x = C.x + offset[0]*N->x + offset[1]*T->x;
  GLfloat y = C.y + offset[0]*N->y + offset[1]*T->y;
  GLfloat z = -offset[2]; // - z;  // distance au sol  
  GLfloat dx = direction[0]*N->x + direction[1]*T->x;
  GLfloat dy = direction[0]*N->y + direction[1]*T->y;
  GLfloat dz = -direction[2];    
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	//Efface le frame buffer et le Z-buffer
  glMatrixMode(GL_MODELVIEW); 	                        //Choisit la matrice MODELVIEW
  glLoadIdentity(); 	                                  //Réinitialise la matrice
  gluLookAt(x,y,z,
            x+dx,y+dy,z+dz,
            0.,0.,-1.);

  make_scene(); 
}
//------------------------------------------------------------------------------
int camera_t::getPicture(uint16_t **pix)
{
  if(info -> current_webcam != this)
    start();
    
  if(!done)  
  {
    capture = true;
    return -1;
  }
  else
  {
    int size = 3*W*H*sizeof(uint16_t); 
    *pix = (uint16_t*)malloc(size);
    memcpy(*pix, pixels, size);   
    done = false;
    return size;
  }
}
//------------------------------------------------------------------------------
GLuint camera_t::LoadTexture(const char * filename, int width, int height)
{
    GLuint texture;
    unsigned char * data;
    FILE * file;

    //The following code will read in our RAW file
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    fclose( file );

    glGenTextures( 1, &texture ); //generate the texture with the loaded data
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters

    //here we are setting what textures to use and when. The MIN filter is which quality to show
    //when the texture is near the view, and the MAG filter is which quality to show when the texture
    //is far from the view.

    //The qualities are (in order from worst to best)
    //GL_NEAREST
    //GL_LINEAR
    //GL_LINEAR_MIPMAP_NEAREST
    //GL_LINEAR_MIPMAP_LINEAR

    //And if you go and use extensions, you can use Anisotropic filtering textures which are of an
    //even better quality, but this will do for now.
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //Here we are setting the parameter to repeat the texture instead of clamping the texture
    //to the edge of our shape.
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    //Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free( data ); //free the texture
    return texture; //return whether it was successfull
}
//------------------------------------------------------------------------------
void camera_t::FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture );
}
