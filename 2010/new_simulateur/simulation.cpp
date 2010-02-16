#include "simulation.h"

/*----------------------------------------------------------------------------*/
simulation_t::simulation_t(int *argc, char **argv, const char* file) : windows(argc, argv)
{
  GLfloat pos[3] = {4.,3.,1.};
  GLfloat at[3] = {1.5,1.,0.};  
  windows.add_window(800, 600, "Scene", pos, at);  

  xml.parse(file);
  world.init(xml.doc);
  windows.init(xml.doc);
  
  windows.set_idle(world_t::simulate);
  windows.set_display(world_t::render);  
}
/*----------------------------------------------------------------------------*/
void simulation_t::start()
{
  world.start();
  windows.start();
}
/*----------------------------------------------------------------------------*/
