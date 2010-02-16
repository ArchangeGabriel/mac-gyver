#ifndef SIMULATION_H
#define SIMULATION_H

#include "xml.h"
#include "world.h"
#include "windows.h"


class simulation_t
{
  private:
  world_parser_t xml;
  world_t world;
  windows_t windows;
  
  public:
  simulation_t(int *argc, char **argv, const char *file);
  void start();
};

#endif
