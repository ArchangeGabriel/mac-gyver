#ifndef OBJECT_H
#define OBJECT_H

#include <ode/ode.h>
#include "shape.h"

class object_t
{
  private:
  bool dynamic;
	dBodyID body;
	dGeomID geom;
	dMass m;
	
	shape_t *shape;
	
	void set_dynamic(const dWorldID &worldID, const QDomElement &dyn);
	
	public:
	object_t(const dWorldID &worldID, const dSpaceID &spaceID, const QDomElement &obj);
	~object_t();
	
	inline void render() {shape->render();}
};

#endif
