#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <qdatetime.h>
#include "xml.h"
#include "object.h"

using namespace std;

class world_t
{
  friend void nearCallback(void *data, dGeomID o0, dGeomID o1);
  private:
  // physics
  static dWorldID worldID;
  static dSpaceID spaceID;
  static dJointGroupID jointGpID; 

  static vector<shape_t*> graphical_objects;  
  static vector<object_t*> physical_objects;
  void add_object(const dWorldID &worldID, const dSpaceID &spaceID, const QDomElement &obj);
    
  static QTime time;
  static bool simul_started;
  static float nbSecondsByStep;
  
  static void handleCollisionBetween(void *, dGeomID o0, dGeomID o1);  
  
  public:
  ~world_t();
  void init(const QDomDocument &world);
  
  static bool render_done;
  static void render();
  static void simulate();
  
  void start();
};

#endif


