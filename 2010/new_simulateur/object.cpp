#include "object.h"


/*----------------------------------------------------------------------------*/
void object_t::set_dynamic(const dWorldID &worldID, const QDomElement &dyn)
{	
  body = dBodyCreate(worldID);
	dGeomSetBody(geom,body);	
	  
  QDomElement lin_vel;
  if(world_parser_t::get_element(dyn,"lin_velocity",lin_vel))
  {
    float x,y,z;
    world_parser_t::get_vector(lin_vel,x,y,z);
    dBodySetLinearVel(body,x,y,z);
  }
  else
    dBodySetLinearVel(body,0,0,0);
  
  QDomElement ang_vel;
  if(world_parser_t::get_element(dyn,"ang_velocity",ang_vel))
  {
    float x,y,z;
    world_parser_t::get_vector(ang_vel,x,y,z);
    dBodySetAngularVel(body,x,y,z);
  }
  else
    dBodySetAngularVel(body,0,0,0);
  
  float mass;
  world_parser_t::get_value(dyn.elementByName("mass"),mass); 
	dMassAdjust(&m,mass);		
	dBodySetMass(body,&m);
}
/*----------------------------------------------------------------------------*/
object_t::object_t(const dWorldID &worldID, const dSpaceID &spaceID, const QDomElement &obj)
{
  QString type = obj.attribute("type");
  
  if(type == "box")
  {
    shape_box_t *s = new shape_box_t(obj);
    shape = s;
  	geom = dCreateBox(spaceID,s->dimX,s->dimY,s->dimZ);
  	dMassSetBox(&m,1.0f,s->dimX,s->dimY,s->dimZ);
  }
  else if(type == "sphere")
  {
    shape_sphere_t *s = new shape_sphere_t(obj);
    shape = s;
  	geom = dCreateSphere(spaceID,s->radius);
  	dMassSetSphere(&m,1.0f,s->radius);    
  }
  else if(type == "cylinder")
  {
    shape_cylinder_t *s = new shape_cylinder_t(obj);
    shape = s;
  	geom = dCreateCylinder(spaceID,s->radius,s->length);
  	dMassSetCylinder(&m,1.0f,3,s->radius,s->length);
  }
  
  // Set object dynamic properties
  QDomElement dyn;
  dynamic = world_parser_t::get_element(obj,"dynamics",dyn);
  if(dynamic)
  {
    set_dynamic(worldID, dyn);
    shape->bind_to_geom(geom, true);
  }
  else
    shape->bind_to_geom(geom, false);
}
/*----------------------------------------------------------------------------*/
object_t::~object_t()
{
}
/*----------------------------------------------------------------------------*/
