#include "world.h"

#define MAX_CONTACTS  10

QTime world_t::time;
dWorldID world_t::worldID = 0;
dSpaceID world_t::spaceID = 0;
dJointGroupID world_t::jointGpID = 0; 

bool world_t::render_done = false;
bool world_t::simul_started = false;
float world_t::nbSecondsByStep = 0.001;

vector<shape_t*> world_t::graphical_objects;  
vector<object_t*> world_t::physical_objects;

/*----------------------------------------------------------------------------*/
void world_t::init(const QDomDocument &world)
{
  worldID = dWorldCreate();
  spaceID = dHashSpaceCreate(0);

  dWorldSetGravity (worldID,0,0,-9.81);
  jointGpID = dJointGroupCreate(0);
  
  graphical_objects.clear();
  physical_objects.clear();  
  QDomNodeList nodes = world.documentElement().elementsByTagName("object");
  for(unsigned int i=0; i<nodes.length(); i++)
    add_object(worldID, spaceID, nodes.item(i).toElement());
}
/*----------------------------------------------------------------------------*/
world_t::~world_t()
{
  for(unsigned int i=0; i<graphical_objects.size(); i++)
    delete graphical_objects[i];
  for(unsigned int i=0; i<physical_objects.size(); i++)
    delete physical_objects[i];

	dSpaceDestroy(spaceID);
	dWorldDestroy(worldID);
	dCloseODE();    
}
/*----------------------------------------------------------------------------*/
void world_t::add_object(const dWorldID &worldID, const dSpaceID &spaceID, const QDomElement &obj)
{
  if(obj.hasAttribute("physical") && obj.attribute("physical").toInt() == 0)
  {
    graphical_objects.resize(graphical_objects.size()+1);
    
    QString type = obj.attribute("type");
    if(type == "box")
      graphical_objects[graphical_objects.size()-1] = new shape_box_t(obj);
    else if(type == "sphere")
      graphical_objects[graphical_objects.size()-1] = new shape_sphere_t(obj);
    else if(type == "cylinder")
      graphical_objects[graphical_objects.size()-1] = new shape_cylinder_t(obj);
  }
  else
  {
    physical_objects.resize(physical_objects.size()+1);
    physical_objects[physical_objects.size()-1] = new object_t(worldID, spaceID, obj);
  }

}
/*----------------------------------------------------------------------------*/
void world_t::render()
{
  if(render_done) return;
  //render_done = true;
  for(unsigned int i=0; i<graphical_objects.size(); i++)
    graphical_objects[i]->render();
  for(unsigned int i=0; i<physical_objects.size(); i++)
    physical_objects[i]->render();
}
/*----------------------------------------------------------------------------*/
void world_t::start()
{
  time.start();
  simul_started = true;  
}
/*----------------------------------------------------------------------------*/
void world_t::simulate()
{
  if(!simul_started || !time.elapsed()) return;

	float nbSecsElapsed = time.restart()/1000.;
	int nbStepsToPerform = static_cast<int>(nbSecsElapsed/nbSecondsByStep);
	
	for (int i=0;i<nbStepsToPerform;++i)
	{
		// Detect collision
		dSpaceCollide(spaceID,NULL,handleCollisionBetween);
		// Step world
		dWorldQuickStep(worldID, nbSecondsByStep);
		// Remove all temporary collision joints now that the world has been stepped
		dJointGroupEmpty(jointGpID);	
	}
  
  render_done = false;
  glutPostRedisplay();  
}
/*----------------------------------------------------------------------------*/
void world_t::handleCollisionBetween(void *, dGeomID o0, dGeomID o1)
{
	dContact contact[MAX_CONTACTS];

	for (int i = 0; i < MAX_CONTACTS; i++)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.8;
		contact[i].surface.bounce_vel = 0.1;
		contact[i].surface.soft_cfm = 0.01;
	}

	dBodyID b1 = dGeomGetBody(o0);
  dBodyID b2 = dGeomGetBody(o1);

  int numc = dCollide(o0, o1, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
	if (numc)
	{
		for (int i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(worldID, jointGpID, contact + i);
			dJointAttach(c, b1, b2);
		}
  }
}
/*----------------------------------------------------------------------------*/
