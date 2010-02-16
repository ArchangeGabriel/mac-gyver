#include <stdio.h>

#include "main.h"
#include "simulation.h"

int main(int argc, char** argv)
{
  simulation_t simul(&argc, argv, "world.xml");
  
  simul.start();
  
  return 0;
}
