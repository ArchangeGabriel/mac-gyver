#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "webcam.h"

simul_info_t* webcam_t::info;  

//------------------------------------------------------------------------------
webcam_t::webcam_t()
{
  pixels = NULL;
}
//------------------------------------------------------------------------------
webcam_t::~webcam_t()
{
  if(pixels)
    delete pixels;
}
//------------------------------------------------------------------------------
void webcam_t::init_params()
{
  printf("  --> Loading webcam's params...");
  fflush(stdout);
  W            = int_param_value("ResolWidth");  
  H            = int_param_value("ResolHeight"); 
  offset[0]    = float_param_value("OffsetX");  
  offset[1]    = float_param_value("OffsetY");   
  offset[2]    = float_param_value("OffsetZ");    
  direction[0] = float_param_value("DirX");  
  direction[1] = float_param_value("DirY");   
  direction[2] = float_param_value("DirZ");  
  
  reshaped = false;
  done = false;
  
  int size = 3*W*H;
  pixels = new GLubyte[size];   
  
  printf("ok\n");
  fflush(stdout);   
}
//------------------------------------------------------------------------------
void webcam_t::init_scene(simul_info_t* _info)
{
  printf("GLUT initialisation...          ");
  
  info = _info;

  GLfloat ambient[]   = {0.1, 0.1, 0.1, 1.};
  GLfloat diffuse[]   = {0.5, 0.5, 0.5, 0.5};
  GLfloat position0[]  = {0, 0, -3., 1.};        
  GLfloat position1[]  = {info->longueur_terrain/2., 0, -3., 1.};  
  GLfloat position2[]  = {0, info->largeur_terrain/2., -3., 1.};  
  GLfloat position3[]  = {info->longueur_terrain/2., info->largeur_terrain/2., -3., 1.}; 
  
  glEnable(GL_DEPTH_TEST);  
  
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

  glEnable(GL_LIGHTING); 
  
  glEnable(GL_COLOR_MATERIAL);  
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
  glReadBuffer(GL_FRONT);
     
  printf("ok\n");
  fflush(stdout);  
}
//------------------------------------------------------------------------------
void webcam_t::create(vector_t *_N, vector_t *_T, vector_t *_G, vector_t *_pos, double *_z, double _h)
{ 
  N = _N;
  T = _T;
  G = _G;
  pos = _pos; 
  z = _z;
  offset[2] -= _h/2.;
}
//------------------------------------------------------------------------------
void webcam_t::start()
{
  printf("Webcam initialisation (GLUT)... ");
  fflush(stdout); 

  info -> current_webcam = this;
      
  glutReshapeWindow(W,H);  
  reshaped = false;
  done = false;  

  printf("ok\n");
  fflush(stdout);  

  glutDisplayFunc(webcam_t::display); 
  glutReshapeFunc(webcam_t::reshape);
}
//------------------------------------------------------------------------------
void webcam_t::stop()
{
  glutDisplayFunc(webcam_t::no_display);     
}
//------------------------------------------------------------------------------
void webcam_t::reshape(int , int )
{
  if(!info -> current_webcam) return;
  webcam_t *WC = (webcam_t*) info -> current_webcam;

  glViewport(0,0,WC->W,WC->H);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45,float(WC->W)/float(WC->H),0.1,100); 	//Pour les explications, lire le tutorial sur OGL et win
  glMatrixMode(GL_MODELVIEW);	
  
  WC->reshaped = true; 
}
//------------------------------------------------------------------------------
void webcam_t::display()
{  
  make_scene();
  
  glutSwapBuffers();
}
//------------------------------------------------------------------------------
void webcam_t::no_display()
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
void webcam_t::make_scene()
{
  if(!info -> current_webcam) return;
  webcam_t *WC = (webcam_t*) info -> current_webcam;
  if(WC->reshaped)
  {
    WC->reshaped = false;
    glutReshapeWindow(WC->W,WC->H);
  }
   
  vector_t C = *WC->pos - *WC->G;
  
  GLfloat x = C.x + WC->offset[0]*WC->N->x + WC->offset[1]*WC->T->x;
  GLfloat y = C.y + WC->offset[0]*WC->N->y + WC->offset[1]*WC->T->y;
  GLfloat z = -*WC->z - WC->offset[2];  
  GLfloat dx = WC->direction[0]*WC->N->x + WC->direction[1]*WC->T->x;
  GLfloat dy = WC->direction[0]*WC->N->y + WC->direction[1]*WC->T->y;
  GLfloat dz = -WC->direction[2];    
  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	//Efface le frame buffer et le Z-buffer

  glEnable(GL_BLEND);  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glEnable(GL_POLYGON_SMOOTH);  
  	
  glMatrixMode(GL_MODELVIEW); 	                        //Choisit la matrice MODELVIEW
  glLoadIdentity(); 	                                  //Réinitialise la matrice
  gluLookAt(x,y,z,
            x+dx,y+dy,z+dz,
            0.,0.,-1.);
            
  make_table();
  
  for(unsigned int i=0;i<info->objets_mobiles.size();i++)
    if(info->objets_mobiles[i]->type != OBJ_ROBOT)
      info->objets_mobiles[i]->draw3D();  

  for(unsigned int i=0;i<info->objets_mobiles.size();i++)
    if(info->objets_mobiles[i]->type == OBJ_ROBOT)
      info->objets_mobiles[i]->draw3D();   

  for(unsigned int i=0;i<info->objets_fixes.size();i++)
    if(!info->objets_fixes[i]->inverse)
      info->objets_fixes[i]->draw3D();     
      
  glReadBuffer(GL_BACK);

  glReadPixels(0,0,WC->W,WC->H,GL_RGB,
                  GL_UNSIGNED_BYTE,
                  WC->pixels);  
                  
  WC->done = true;
}
//------------------------------------------------------------------------------
void webcam_t::make_table()
{
  GLfloat w = info->longueur_terrain;
  GLfloat h = info->largeur_terrain;
  GLfloat e = 0.03;
  GLfloat z = 0.07;
  
  /* Table bleue */
  glColor3f(0.204, 0.380, 0.714);  
  glNormal3f(0., 0., -1.);  
  glBegin(GL_QUADS); 
  glVertex3f(0.,0.,0.);   glVertex3f(w,0.,0.);   glVertex3f(w,h,0.);   glVertex3f(0.,h,0.);  
  glEnd();     
  
  /* Marquages au sol */
  glPushMatrix(); 
  /* Départs rouge et vert */
  glTranslatef(0.,0.,-0.0001); 
  glColor3f(0.318, 0.659, 0.278);   
  glRectf(0., 0., 0.5, 0.5);
  glColor3f(0.776, 0., 0.);   
  glRectf(w-0.5, 0., w, 0.5);
  
  /* Zones de niveau 1 */
  glColor3f(0.392, 0.275, 0.157); 
  glRectf(w/2.-0.9, h-0.1, w/2.+0.9, h);
  
  /* Bande de guidage */  
  glColor3f(0., 0., 0.);   
  for(int i=0;i<3;i++)
    for(int j=0;j<4;j++)  
    {
      GLfloat x0 = w/2-0.9+0.105-0.0075+i*0.6+j*0.13;
      glRectf(x0, h-0.35, x0+0.015, h-0.1);
    }

  for(int i=0;i<4;i++)     
  {
    GLfloat x0 = w/2-0.6-0.0075+i*0.4;
    glRectf(x0, 0, x0+0.015, 0.25);
  }  
  
  for(int i=0;i<3;i++)              
    for(int j=0;j<4;j++)  
    {
      const int n = 4;
      GLfloat cx1 = w/2-0.4-i*0.25;
      GLfloat cx2 = w/2+0.4+i*0.25;
      GLfloat cy = h/2+0.125-j*0.2;
      GLfloat r  = 0.005;
      GLfloat x0,y0;
      glBegin(GL_POLYGON);
      for(int k = 0; k< n; k++)
      {
        x0 = r * cos(2*k*M_PI/n) + cx1;
        y0 = r * sin(2*k*M_PI/n) + cy; 
        glVertex3f(x0,y0,0.);       
      }
      glEnd();
      glBegin(GL_POLYGON);
      for(int k = 0; k< n; k++)
      {
        x0 = r * cos(2*k*M_PI/n) + cx2;
        y0 = r * sin(2*k*M_PI/n) + cy; 
        glVertex3f(x0,y0,0.);       
      }
      glEnd();                   
    }        
  glPopMatrix();   

  glColor3f(1., 1., 1.);
  
  glBegin(GL_QUADS); 
  glNormal3f(0., 0., -1.);
  glVertex3f(0.,0.,-z);  glVertex3f(0.,-e,-z);  glVertex3f(w,-e,-z);    glVertex3f(w,0.,-z);      
  glVertex3f(0.,h,-z);   glVertex3f(w,h,-z);    glVertex3f(w,h+e,-z);   glVertex3f(0.,h+e,-z); 
  glVertex3f(w,-e,-z);   glVertex3f(w+e,-e,-z); glVertex3f(w+e,h+e,-z); glVertex3f(w,h+e,-z);
  glVertex3f(0.,-e,-z);  glVertex3f(0.,h+e,-z); glVertex3f(-e,h+e,-z);  glVertex3f(-e,-e,-z);
  glEnd(); 

  glBegin(GL_QUADS); 
  glNormal3f(0., 1., 0.);
  glVertex3f(0.,0.,0.);  glVertex3f(0.,0.,-z);  glVertex3f(w,0.,-z);    glVertex3f(w,0.,0.);
  glVertex3f(-e,h+e,0.); glVertex3f(-e,h+e,-z); glVertex3f(w+e,h+e,-z); glVertex3f(w+e,h+e,0.);       
  glEnd();   

  glBegin(GL_QUADS); 
  glNormal3f(0., -1., 0.);
  glVertex3f(-e,-e,0.);  glVertex3f(w+e,-e,0.); glVertex3f(w+e,-e,-z);  glVertex3f(-e,-e,-z);  
  glVertex3f(0.,h,0.);   glVertex3f(w,h,0.);    glVertex3f(w,h,-z);     glVertex3f(0.,h,-z);   
  glEnd();     
   
  glBegin(GL_QUADS); 
  glNormal3f(1., 0., 0.); 
  glVertex3f(w+e,-e,-z); glVertex3f(w+e,-e,0.); glVertex3f(w+e,h+e,0.); glVertex3f(w+e,h+e,-z);  
  glVertex3f(0.,0.,0.);  glVertex3f(0.,h,0.);   glVertex3f(0.,h,-z);    glVertex3f(0.,0.,-z);
  glEnd(); 
  
  glBegin(GL_QUADS); 
  glNormal3f(-1., 0., 0.);   
  glVertex3f(w,0.,0.);   glVertex3f(w,0.,-z);   glVertex3f(w,h,-z);     glVertex3f(w,h,0.);
  glVertex3f(-e,-e,-z);  glVertex3f(-e,h+e,-z); glVertex3f(-e,h+e,0.);  glVertex3f(-e,-e,0.); 
  glEnd();    
}
//------------------------------------------------------------------------------
void webcam_t::makePicture()
{ 
  vector_t C = *pos - *G;
  
  GLfloat x = C.x + offset[0]*N->x + offset[1]*T->x;
  GLfloat y = C.y + offset[0]*N->y + offset[1]*T->y;
  GLfloat z = -z - offset[2];  
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
int webcam_t::getPicture(void *pix)
{
  if(info -> current_webcam == this)
  {
    if(done)
    {
      int size = 3*W*H; 
      *((GLubyte**)pix) = (GLubyte*) malloc(sizeof(GLubyte) * size);
      memcpy(*((GLubyte**)pix), pixels, size);   
      return size; 
    }
    else
      return -1;
  }
  else
  {
    start();
    done = false;      
    return -1;    
  }
}
//------------------------------------------------------------------------------
GLuint webcam_t::LoadTexture(const char * filename, int width, int height)
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
void webcam_t::FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture );
}
