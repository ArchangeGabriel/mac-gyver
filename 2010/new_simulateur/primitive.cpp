#include <math.h>
#include <stdlib.h>

#include "primitive.h"

void glutSolidCylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks)
{
  const GLfloat hh = height/2.;
  const GLfloat hstep = height/(GLfloat)(stacks);

  GLfloat *x = (GLfloat*) malloc(sizeof(GLfloat)*slices);
  GLfloat *y = (GLfloat*) malloc(sizeof(GLfloat)*slices);
  GLfloat *nx = (GLfloat*) malloc(sizeof(GLfloat)*slices);
  GLfloat *ny = (GLfloat*) malloc(sizeof(GLfloat)*slices);  
 
  for(int i=0; i<slices; i++)
  {
    nx[i] = cos(((double)i)*2.0*M_PI/((double)slices));
    ny[i] = sin(((double)i)*2.0*M_PI/((double)slices));    
    x[i] = radius * nx[i];
    y[i] = radius * ny[i];    
  }
  
  glBegin(GL_POLYGON);  
  glNormal3f(0., 0., 1.);
  for(int i=0; i<slices; i++)
    glVertex3f(x[i],y[i],hh);
  glEnd();

  glBegin(GL_POLYGON);  
  glNormal3f(0., 0., -1.);
  for(int i=slices-1; i>=0; i--)
    glVertex3f(x[i],y[i],-hh);
  glEnd();
  
  glBegin(GL_QUADS);
  for(int j=0; j<stacks; j++)
  {
    GLfloat h = j*hstep - hh;  
    for(int i=0; i<slices; i++)
    {
      glNormal3f(nx[i], ny[i], 0.);
      glVertex3f(x[i],y[i],h+hstep);       
      glVertex3f(x[i],y[i],h);
      glNormal3f(nx[(i+1)%slices], ny[(i+1)%slices], 0.);          
      glVertex3f(x[(i+1)%slices],y[(i+1)%slices],h);  
      glVertex3f(x[(i+1)%slices],y[(i+1)%slices],h+hstep);     
    }
  }
  glEnd();
      
  free(x);
  free(y);
  free(nx);
  free(ny);
}


void glutSolidBox(GLfloat width, GLfloat height, GLfloat depth)
{
  const GLfloat w = width/2.; 
  const GLfloat h = height/2.; 
  const GLfloat d = depth/2.;     
  GLfloat x[8]={ w,-w,-w, w, w,-w,-w, w};
  GLfloat y[8]={ h, h,-h,-h, h, h,-h,-h};
  GLfloat z[8]={ d, d, d, d,-d,-d,-d,-d};
     
  glNormal3f(0.,0.,1.);   
  glBegin(GL_QUADS); 
  glVertex3f(x[0],y[0],z[0]); glVertex3f(x[1],y[1],z[1]); glVertex3f(x[2],y[2],z[2]); glVertex3f(x[3],y[3],z[3]); 
  glEnd();
     
  glNormal3f(0.,0.,-1.);   
  glBegin(GL_QUADS);   
  glVertex3f(x[4],y[4],z[4]); glVertex3f(x[7],y[7],z[7]); glVertex3f(x[6],y[6],z[6]); glVertex3f(x[5],y[5],z[5]); 
  glEnd();
    
  glNormal3f(1.,0.,0.);   
  glBegin(GL_QUADS);   
  glVertex3f(x[0],y[0],z[0]); glVertex3f(x[3],y[3],z[3]); glVertex3f(x[7],y[7],z[7]); glVertex3f(x[4],y[4],z[4]); 
  glEnd();
    
  glNormal3f(-1.,0.,0.);    
  glBegin(GL_QUADS);
  glVertex3f(x[1],y[1],z[1]); glVertex3f(x[5],y[5],z[5]); glVertex3f(x[6],y[6],z[6]); glVertex3f(x[2],y[2],z[2]); 
  glEnd();  

  glNormal3f(0.,1.,0.);   
  glBegin(GL_QUADS);
  glVertex3f(x[1],y[1],z[1]); glVertex3f(x[0],y[0],z[0]); glVertex3f(x[4],y[4],z[4]); glVertex3f(x[5],y[5],z[5]); 
  glEnd();

  glNormal3f(0.,-1.,0.);  
  glBegin(GL_QUADS);
  glVertex3f(x[3],y[3],z[3]); glVertex3f(x[2],y[2],z[2]); glVertex3f(x[6],y[6],z[6]); glVertex3f(x[7],y[7],z[7]);
  glEnd();
}
