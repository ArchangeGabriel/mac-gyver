#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "the_playground.h"
#include "the_elements.h"
#include "colors.h"
#include "../sdl.h"



//------------------------------------------------------------------------------
void add_object(simul_info_t *info, object_t *obj)
{
  obj->simul_info=info;
  obj->maj_const_vars();
  info->add_object(obj);
}
//------------------------------------------------------------------------------
void add_obstacles(simul_info_t *info)
{
  //On rajoute les obstacles
  ObjBox *bord_terrain=new ObjBox;
  bord_terrain->type=OBJ_TABLE;
  bord_terrain->hide=true;
  bord_terrain->fixe=true;
  bord_terrain->inverse=true;
  bord_terrain->dimX=pgWidth;
  bord_terrain->dimY=2.6;
  bord_terrain->position.x=pgWidth/2.;
  bord_terrain->position.y=2.6/2.;   
  bord_terrain->hauteur=0.07;
  bord_terrain->z=0.07/2;   
  bord_terrain->couleur=clBlack;
  bord_terrain->matiere=mBois;
  add_object(info,bord_terrain);
  //On rajoute les obstacles
  bord_terrain=new ObjBox;
  bord_terrain->type=OBJ_TABLE;
  bord_terrain->hide=true;
  bord_terrain->fixe=true;
  bord_terrain->inverse=false;
  bord_terrain->dimX=2.;
  bord_terrain->dimY=0.5;
  bord_terrain->position.x=pgWidth/2.;
  bord_terrain->position.y=2.35;   
  bord_terrain->hauteur=0.07;
  bord_terrain->z=0.07/2;   
  bord_terrain->couleur=clBlack;
  bord_terrain->matiere=mBois;
  add_object(info,bord_terrain);  

  // La colline
  ObjBox *hill=new ObjBox;
  hill->type=OBJ_TABLE;
  hill->hide=true;
  hill->fixe=true;
  hill->inverse=false;
  hill->dimX=1.520;
  hill->dimY=0.522;
  hill->position.x=pgWidth/2.;
  hill->position.y=0.261;   
  hill->hauteur=0.2;
  hill->z=0.2/2.;   
  hill->couleur=clBlack;
  hill->matiere=mBois;
  add_object(info,hill);

  int tomates = info->config_terrain/100;
  int configE = (info->config_terrain%100)/10;
  int configI = info->config_terrain%10;
  object_t *obj1, *obj2;

  if(configE != 0 && configI !=0)
  {
    bool FakeCornE[3][3];
    bool FakeCornI[2];
    FakeCornE[0][0] = (configE==2 || configE==5 || configE==8);
    FakeCornE[1][0] = (configE==4 || configE==5 || configE==6);  
    FakeCornE[2][0] = (configE==7 || configE==8 || configE==9);    
    FakeCornE[0][1] = (configE==1 || configE==2 || configE==7);
    FakeCornE[1][1] = (configE==1 || configE==2 || configE==3);  
    FakeCornE[2][1] = (configI==2 || configI==4);
    FakeCornE[0][2] = (configE==3 || configE==4 || configE==9);
    FakeCornE[1][2] = (configI==1 || configI==3);
    FakeCornE[2][2] = false;      
    FakeCornI[0] = (configI==1 || configI==2);
    FakeCornI[1] = (configI==3 || configI==4);
    

    for(int i=0;i<3;++i)  
      for(int j=0;j<3;++j)    
        if(i != 2 || j != 2)
        {
          if(FakeCornE[i][j])
          {
            obj1 = new EltFakeCorn;
            obj2 = new EltFakeCorn;            
          }
          else
          {
            obj1 = new EltCorn;
            obj2 = new EltCorn;            
          }
            
          obj1->position.x = 0.15 + 0.45*i;
          obj2->position.x = pgWidth - 0.15 - 0.45*i;        
          obj1->position.y = 2.1 - 1.378 + 0.5*j + 0.25*i;                        
          obj2->position.y = 2.1 - 1.378 + 0.5*j + 0.25*i;                
          
          add_object(info,obj1);
          add_object(info,obj2);        
        }
        
    for(int i=0;i<2;++i) 
    {
      if(FakeCornI[i])
        obj1 = new EltFakeCorn;
      else
        obj1 = new EltCorn;
            
      obj1->position.x = pgWidth/2.;
      obj1->position.y = 2.1 - 0.628 + 0.5*i; 

      add_object(info,obj1);    
    }
  }
    
  if(tomates)
  {
    for(int i=0;i<3;++i)  
      for(int j=0;j<2;++j)    
      {
        obj1 = new EltTomato; 
        obj2 = new EltTomato; 
                
        obj1->position.x = 0.15 + 0.45*i;
        obj2->position.x = pgWidth - 0.15 - 0.45*i;
        obj1->position.y = 2.1 - 1.128 + 0.5*j + 0.25*i;       
        obj2->position.y = 2.1 - 1.128 + 0.5*j + 0.25*i;       
        
        add_object(info,obj1);
        add_object(info,obj2);        
      }
    obj1 = new EltTomato; 
    obj1->position.x = pgWidth/2.;
    obj1->position.y = 2.1 - 0.378;
    add_object(info,obj1);      

    obj1 = new EltTomato; 
    obj1->position.x = pgWidth/2.;
    obj1->position.y = 2.1 - 0.878;
    add_object(info,obj1);      
  }
}
//------------------------------------------------------------------------------
void draw_playground_2D(double scale)
{
  // sol
  FillRectSDL(0,0,int(scale*pgWidth),int(scale*pgHeight),clGround);  
  // départ 1
  FillRectSDL(0,0,int(scale*0.5),int(scale*0.5),clTeam1);
  FillRectSDL(int(scale*(pgWidth-0.5)),int(scale*(pgHeight-0.522)),int(scale*0.5),int(scale*0.522),clDeposeTeam1);  
  // départ 2
  FillRectSDL(int(scale*(pgWidth-0.5)),0,int(scale*0.5),int(scale*0.5),clTeam2);
  FillRectSDL(0.,int(scale*(pgHeight-0.522)),int(scale*0.5),int(scale*0.522),clDeposeTeam2);  
  // Hors Terrain
  FillRectSDL(int(scale*0.5),int(scale*2.122),int(scale*2.),int(scale*0.5),clBlack);
  // Mur Bas
  FillRectSDL(int(scale*0.5),int(scale*2.1),int(scale*2.),int(scale*0.022),clWall);    
  FillRectSDL(int(scale*0.5),int(scale*2.122),int(scale*0.022),int(scale*0.5),clWall);         
  FillRectSDL(int(scale*(pgWidth-0.522)),int(scale*2.122),int(scale*0.022),int(scale*0.5),clWall);                         
  // Colline
  FillRectSDL(int(scale*0.67),0,int(scale*1.66),int(scale*0.5),clHill);
  // Mur colline
  FillRectSDL(int(scale*0.74),int(scale*0.5),int(scale*1.52),int(scale*0.022),clWall); 
  LigneVerticaleSDL(int(scale*0.74), 0, int(scale*0.5), clBlack);               
  LigneVerticaleSDL(int(scale*(pgWidth-0.74)), 0, int(scale*0.5), clBlack);                 
  LigneVerticaleSDL(int(scale*1.24), 0, int(scale*0.5), clBlack);                                    
  LigneVerticaleSDL(int(scale*(pgWidth-1.24)), 0, int(scale*0.5), clBlack);                                      
}
//------------------------------------------------------------------------------
void draw_playground_3D()
{
  GLfloat w = pgWidth;
  GLfloat h = pgHeight;
  GLfloat e = 0.022;
  GLfloat z = 0.07;
  
  glNormal3f(0., 0., -1.);
  /* Table bleue */
  glColor3f(glGround);  
  glBegin(GL_QUADS);  
  glVertex3f(0.,0.,0.); glVertex3f(0.,2.12,0.);  glVertex3f(w,2.12,0.); glVertex3f(w,0.,0.);     
  glEnd();
  
  /* Marquages au sol */
  glPushMatrix(); 
  /* Départs rouge et vert */
  glTranslatef(0.,0.,-0.001); 
  glBegin(GL_QUADS);
  glColor3f(glTeam1);   
  glVertex3f(0.,0.,0.); glVertex3f(0.,0.5,0.);  glVertex3f(0.5,0.5,0.); glVertex3f(0.5,0.,0.);    
  glColor3f(glTeam2);   
  glVertex3f(w-0.5,0.,0.); glVertex3f(w-0.5,0.5,0.);  glVertex3f(w,0.5,0.); glVertex3f(w,0.,0.); 
  glColor3f(glDeposeTeam2);     
  glVertex3f(0.,2.1,0.); glVertex3f(0.,2.622,0.);  glVertex3f(0.5,2.622,0.); glVertex3f(0.5,2.1,0.); 
  glColor3f(glDeposeTeam1);     
  glVertex3f(w-0.5,2.1,0.); glVertex3f(w-0.5,2.622,0.);  glVertex3f(w,2.622,0.); glVertex3f(w,2.1,0.);   
  glColor3f(glHill);   
  glVertex3f(0.67,0.,0.); glVertex3f(0.67,0.5,0.);  glVertex3f(0.74,0.5,0.); glVertex3f(0.74,0.,0.);    
  glVertex3f(w-0.74,0.,0.); glVertex3f(w-0.74,0.5,0.);  glVertex3f(w-0.67,0.5,0.); glVertex3f(w-0.67,0.,0.);
  glEnd();
  glPopMatrix();
  
  // La colline
  glColor3f(glHill);
  glBegin(GL_QUADS);
  glNormal3f(0., 0., -1.);          
  glVertex3f(1.24,0., -0.21); glVertex3f(1.24,0.5,-0.21); glVertex3f(1.76,0.5,-0.21); glVertex3f(1.76,0.,-0.21);  
  glNormal3f(0.387232485, 0., -0.921982106);            
  glVertex3f(1.76,0.,-0.21);  glVertex3f(1.76,0.5,-0.21); glVertex3f(w-0.74,0.5,0.);  glVertex3f(w-0.74,0.,0.);   
  glNormal3f(-0.387232485, 0., -0.921982106);   
  glVertex3f(0.74,0., 0.);    glVertex3f(0.74,0.5, 0.);   glVertex3f(1.24,0.5,-0.21); glVertex3f(1.24,0., -0.21); 
  glEnd();  

  float y = 0.;
  glColor3f(glWall);  
  glNormal3f(0., 1., 0.);  
  glBegin(GL_POLYGON);   
  glVertex3f(0.74,   y, 0.);
  glVertex3f(w-0.74, y, 0.);   
  glVertex3f(w-0.74, y, -z);
  glVertex3f(w-1.24, y, -0.21-z); 
  glVertex3f(1.24,   y, -0.21-z);       
  glVertex3f(0.74,   y, -z);  
  glEnd();

  y = -e;
  glNormal3f(0., -1., 0.);  
  glBegin(GL_POLYGON);  
  glVertex3f(0.74,   y, 0.);
  glVertex3f(0.74,   y, -z);  
  glVertex3f(1.24,   y, -0.21-z);
  glVertex3f(w-1.24, y, -0.21-z);   
  glVertex3f(w-0.74, y, -z);  
  glVertex3f(w-0.74, y, 0.);        
  glEnd();  
  
  y = 0.5;
  glNormal3f(0., -1., 0.);  
  glBegin(GL_POLYGON);  
  glVertex3f(0.74,   y, 0.);
  glVertex3f(0.74,   y, -z);  
  glVertex3f(1.24,   y, -0.21-z);
  glVertex3f(w-1.24, y, -0.21-z);    
  glVertex3f(w-0.74, y, -z);        
  glVertex3f(w-0.74, y, 0.);    
  glEnd();

  y = 0.5+e;
  glNormal3f(0., 1., 0.);  
  glBegin(GL_POLYGON);  
  glVertex3f(0.74,   y, 0.);
  glVertex3f(w-0.74, y, 0.);  
  glVertex3f(w-0.74, y, -z);
  glVertex3f(w-1.24, y, -0.21-z);  
  glVertex3f(1.24,   y, -0.21-z);        
  glVertex3f(0.74,   y, -z);    
  glEnd();  

  y = -e;
  glBegin(GL_QUADS);
  glNormal3f(1., 0., 0.);  
  glVertex3f(w-0.74, y+e, -z); glVertex3f(w-0.74, y+e, 0.); glVertex3f(w-0.74, y, 0.);  glVertex3f(w-0.74, y, -z); 
  glNormal3f(0.387232485, 0., -0.921982106);      
  glVertex3f(w-0.74, y, -z); glVertex3f(w-1.24, y, -0.21-z); glVertex3f(w-1.24, y+e, -0.21-z); glVertex3f(w-0.74, y+e, -z); 
  glNormal3f(0. , 0., -1.);   
  glVertex3f(w-1.24, y, -0.21-z); glVertex3f(1.24, y, -0.21-z);  glVertex3f(1.24, y+e, -0.21-z);  glVertex3f(w-1.24, y+e, -0.21-z); 
  glNormal3f(-0.387232485, 0., -0.921982106);    
  glVertex3f(0.74, y, -z); glVertex3f(0.74, y+e, -z); glVertex3f(1.24, y+e, -0.21-z); glVertex3f(1.24, y, -0.21-z); 
  glNormal3f(-1., 0., 0.);  
  glVertex3f(0.74, y+e, -z); glVertex3f(0.74, y, -z); glVertex3f(0.74, y, 0.); glVertex3f(0.74, y+e, 0.); 
  glEnd();   
  
  y = 0.5;
  glBegin(GL_QUADS);
  glNormal3f(1., 0., 0.);  
  glVertex3f(w-0.74, y+e, -z); glVertex3f(w-0.74, y+e, 0.); glVertex3f(w-0.74, y, 0.); glVertex3f(w-0.74, y, -z); 
  glNormal3f(0.387232485, 0., -0.921982106);      
  glVertex3f(w-0.74, y, -z); glVertex3f(w-1.24, y, -0.21-z); glVertex3f(w-1.24, y+e, -0.21-z); glVertex3f(w-0.74, y+e, -z); 
  glNormal3f(0. , 0., -1.);   
  glVertex3f(w-1.24, y, -0.21-z); glVertex3f(1.24, y, -0.21-z); glVertex3f(1.24, y+e, -0.21-z); glVertex3f(w-1.24, y+e, -0.21-z); 
  glNormal3f(-0.387232485, 0., -0.921982106);    
  glVertex3f(0.74, y, -z); glVertex3f(0.74, y+e, -z); glVertex3f(1.24, y+e, -0.21-z); glVertex3f(1.24, y, -0.21-z); 
  glNormal3f(-1., 0., 0.);  
  glVertex3f(0.74, y+e, -z); glVertex3f(0.74, y, -z); glVertex3f(0.74, y, 0.); glVertex3f(0.74, y+e, 0.); 
  glEnd();   
  
  // Les murs
  glColor3f(glWall);
  glBegin(GL_QUADS); 
  glNormal3f(0., 0., -1.);
  glVertex3f(0.,0.,-z);   glVertex3f(w,0.,-z);      glVertex3f(w,-e,-z);        glVertex3f(0.,-e,-z);    
  glVertex3f(w,-e,-z);    glVertex3f(w,h+e,-z);     glVertex3f(w+e,h+e,-z);     glVertex3f(w+e,-e,-z);    
  glVertex3f(0.,-e,-z);   glVertex3f(-e,-e,-z);     glVertex3f(-e,h+e,-z);      glVertex3f(0.,h+e,-z);      
  glVertex3f(0.5,2.1,-z); glVertex3f(0.5,2.1+e,-z); glVertex3f(w-0.5,2.1+e,-z); glVertex3f(w-0.5,2.1,-z);           //
  glVertex3f(0.,h,-z);    glVertex3f(0.,h+e,-z);    glVertex3f(0.5,h+e,-z);     glVertex3f(0.5,h,-z);               //
  glVertex3f(w-0.5,h,-z); glVertex3f(w-0.5,h+e,-z);  glVertex3f(w,h+e,-z);      glVertex3f(w,h,-z);                 //
  glVertex3f(0.5,2.1+e,-z);     glVertex3f(0.5,h+e,-z);     glVertex3f(0.5+e,h+e,-z);  glVertex3f(0.5+e,2.1+e,-z);  //      
  glVertex3f(w-0.5-e,2.1+e,-z); glVertex3f(w-0.5-e,h+e,-z); glVertex3f(w-0.5,h+e,-z);  glVertex3f(w-0.5,2.1+e,-z);  //        
  glEnd(); 
  
  glBegin(GL_QUADS); 
  glNormal3f(0., 1., 0.);
  glVertex3f(0.,0.,0.);       glVertex3f(w,0.,0.);          glVertex3f(w,0.,-z);          glVertex3f(0.,0.,-z);       
  glVertex3f(0.5+e,2.1+e,0.); glVertex3f(w-0.5-e,2.1+e,0.); glVertex3f(w-0.5-e,2.1+e,-z); glVertex3f(0.5+e,2.1+e,-z);  //       
  glVertex3f(-e,h+e,0.);      glVertex3f(0.5+e,h+e,0.);     glVertex3f(0.5+e,h+e,-z);     glVertex3f(-e,h+e,-z);       // 
  glVertex3f(w-0.5-e,h+e,0.); glVertex3f(w+e,h+e,0.);       glVertex3f(w+e,h+e,-z);       glVertex3f(w-0.5-e,h+e,-z);  //     
  glEnd();   

  glBegin(GL_QUADS); 
  glNormal3f(0., -1., 0.);
  glVertex3f(-e,-e,0.);   glVertex3f(-e,-e,-z);    glVertex3f(w+e,-e,-z);    glVertex3f(w+e,-e,0.);  
  glVertex3f(0.5,2.1,0.); glVertex3f(0.5,2.1,-z);  glVertex3f(w-0.5,2.1,-z); glVertex3f(w-0.5,2.1,0.);   //  
  glVertex3f(0,h,0.);     glVertex3f(0.,h,-z);     glVertex3f(0.5,h,-z);     glVertex3f(0.5,h,0.);       //  
  glVertex3f(w-0.5,h,0.); glVertex3f(w-0.5,h,-z);  glVertex3f(w,h,-z);       glVertex3f(w,h,0.);         //      
  glEnd();      
   
  glBegin(GL_QUADS); 
  glNormal3f(1., 0., 0.); 
  glVertex3f(w+e,-e,-z); glVertex3f(w+e,h+e,-z); glVertex3f(w+e,h+e,0.); glVertex3f(w+e,-e,0.); 
  glVertex3f(0.,0.,0.);  glVertex3f(0.,0.,-z);   glVertex3f(0.,h,-z);    glVertex3f(0.,h,0.);       
  glVertex3f(w-0.5,2.1,0.);   glVertex3f(w-0.5,2.1,-z);   glVertex3f(w-0.5,h,-z);   glVertex3f(w-0.5,h,0.);       //
  glVertex3f(0.5+e,2.1+e,0.); glVertex3f(0.5+e,2.1+e,-z); glVertex3f(0.5+e,h+e,-z); glVertex3f(0.5+e,h+e,0.);     // 
  glEnd(); 

  glBegin(GL_QUADS); 
  glNormal3f(-1., 0., 0.);   
  glVertex3f(w,0.,0.);    glVertex3f(w,h,0.);    glVertex3f(w,h,-z);      glVertex3f(w,0.,-z);  
  glVertex3f(-e,-e,0.);   glVertex3f(-e,h+e,0.); glVertex3f(-e,h+e,-z);   glVertex3f(-e,-e,-z);  
  glVertex3f(0.5,2.1,-z);       glVertex3f(0.5,2.1,0.);        glVertex3f(0.5,h,0.);       glVertex3f(0.5,h,-z);        // 
  glVertex3f(w-0.5-e,2.1+e,0.); glVertex3f(w-0.5-e,2.1+e,-z);  glVertex3f(w-0.5-e,h+e,-z); glVertex3f(w-0.5-e,h+e,0.);  //
  glEnd();    
  
}
//------------------------------------------------------------------------------
