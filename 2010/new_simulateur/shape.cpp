#include <stdlib.h>

#include "shape.h"
#include "xml.h"
#include "primitive.h"

/*----------------------------------------------------------------------------*/
void shape_t::set_material(const QDomElement &mat)
{
  world_parser_t::get_color(mat.elementByName("color"),color[0],color[1],color[2]);
  world_parser_t::get_color(mat.elementByName("specular"),spec[0],spec[1],spec[2]);  
  world_parser_t::get_value(mat.elementByName("alpha"),color[3]);
  world_parser_t::get_value(mat.elementByName("shininess"),shininess);  
}
/*----------------------------------------------------------------------------*/
void shape_t::set_pos_rot(const QDomElement &obj)
{
  float x,y,z;
  dMatrix3 mat;
  
  // Set object position
  QDomElement pos = obj.elementByName("position");
  world_parser_t::get_vector(pos,x,y,z);
  m[12] = x;
  m[13] = y;
  m[14] = z;
  m[15] = 1.;

  // Set object rotation  
  QDomElement rot = obj.elementByName("rotation");
  world_parser_t::get_vector(rot,x,y,z);
  x *= M_PI/180.;
  y *= M_PI/180.;
  z *= M_PI/180.;  
  dRFromEulerAngles(mat,x,y,z);  
  m[0] = mat[0]; m[1] = mat[4]; m[ 2] = mat[ 8]; m[ 3] = 0.;
  m[4] = mat[1]; m[5] = mat[5]; m[ 6] = mat[ 9]; m[ 7] = 0.;
  m[8] = mat[2]; m[9] = mat[6]; m[10] = mat[10]; m[11] = 0.;	    
}
/*----------------------------------------------------------------------------*/
void shape_t::bind_to_geom(dGeomID geomID, bool is_dynamic)
{
  geom = geomID;
  dMatrix3 mat;
  mat[0] = m[0]; mat[4] = m[1]; mat[ 8] = m[ 2];
  mat[1] = m[4]; mat[5] = m[5]; mat[ 9] = m[ 6];
  mat[2] = m[8]; mat[6] = m[9]; mat[10] = m[10];
  
  dGeomSetRotation(geom, mat);
  dGeomSetPosition(geom, m[12], m[13], m[14]);
  
  dynamic = is_dynamic;
}
/*----------------------------------------------------------------------------*/
void shape_t::render() const
{
  glPushMatrix();
  if(dynamic)
  { 
    GLfloat mat[16];      
    const dReal *p = dGeomGetPosition(geom);
    const dReal *r = dGeomGetRotation(geom);  
    mat[ 0] = r[0]; mat[ 1] = r[4]; mat[ 2] = r[ 8]; mat[ 3] = 0.;
    mat[ 4] = r[1]; mat[ 5] = r[5]; mat[ 6] = r[ 9]; mat[ 7] = 0.;
    mat[ 8] = r[2]; mat[ 9] = r[6]; mat[10] = r[10]; mat[11] = 0.;
    mat[12] = p[0]; mat[13] = p[1]; mat[14] = p[ 2]; mat[15] = 1.;
    glMultMatrixf(mat);
  }
  else
    glMultMatrixf(m);

  glColor4fv(color); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
 
  render_shape();
  glPopMatrix();
}
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
shape_box_t::shape_box_t(const QDomElement &obj)
{
  dimX = obj.attribute("dimx").toFloat();
  dimY = obj.attribute("dimy").toFloat();
  dimZ = obj.attribute("dimz").toFloat();    
  set_material(obj.elementByName("material"));
  set_pos_rot(obj);
}
/*----------------------------------------------------------------------------*/
inline void shape_box_t::render_shape() const
{
  glutSolidBox(dimX,dimY,dimZ);
}
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
shape_sphere_t::shape_sphere_t(const QDomElement &obj)
{
  radius = obj.attribute("radius").toFloat();
  set_material(obj.elementByName("material"));
  set_pos_rot(obj);
}
/*----------------------------------------------------------------------------*/
inline void shape_sphere_t::render_shape() const
{
  glutSolidSphere(radius,32,32);
}
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
shape_cylinder_t::shape_cylinder_t(const QDomElement &obj)
{
  radius = obj.attribute("radius").toFloat();
  length = obj.attribute("length").toFloat();
  set_material(obj.elementByName("material"));
  set_pos_rot(obj);
}
/*----------------------------------------------------------------------------*/
inline void shape_cylinder_t::render_shape() const
{
  glutSolidCylinder(radius, length, 32, 1);
}
/*----------------------------------------------------------------------------*/


