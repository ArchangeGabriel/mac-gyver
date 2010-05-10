#include <unistd.h>
#include <stdio.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "picAPI.h"
#include "anticol.h"
#include "path_tracker.h"
#include "cinematik.h"
#include "strategie.h"

//------------------------------------------------------------------------------
void* ac_MainLoop(void*)
{
  while(true)
  {
    usleep(100000);
  }  
}
//------------------------------------------------------------------------------
void ac_OnRecvCaptors(int n, float *values)
{
  if(n != 4)
  {
    fprintf(stderr,"<anticol.cpp> Nombre de capteurs incorrect (%d).\n",n);
    fflush(stdout);
    return;
  }
}
//------------------------------------------------------------------------------

