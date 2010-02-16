#ifndef SHAPE_H
#define SHAPE_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <ode/ode.h>
#include "xml.h"

class shape_t
{
  private:
	GLfloat color[4];
	GLfloat spec[3];
	GLfloat shininess;
	GLuint  texture;
	
  bool dynamic;
  dGeomID geom;
  GLfloat m[16];  
	
	public:
  void set_material(const QDomElement &mat);	
  void set_pos_rot(const QDomElement &obj);
	void bind_to_geom(dGeomID bodyID, bool is_dynamic);
	
	void render() const;
	virtual void render_shape() const = 0;
};

class shape_box_t : public shape_t
{
  friend class object_t;
  private:
  GLfloat dimX,dimY,dimZ;
  
  public:
  shape_box_t(const QDomElement &obj);
  void render_shape() const;
};


class shape_sphere_t : public shape_t
{
  friend class object_t;
  private:
  GLfloat radius;
  
  public:
  shape_sphere_t(const QDomElement &obj);
  void render_shape() const;
};


class shape_cylinder_t : public shape_t
{
  friend class object_t;
  private:
  GLfloat radius;
  GLfloat length;
  
  public:
  shape_cylinder_t(const QDomElement &obj);
  void render_shape() const;
};

#endif
