#include <unistd.h>
#include <stdio.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "picAPI.h"
#include "anticol.h"
#include "path_planning.h"
#include "cinematik.h"
#include "strategie.h"

//------------------------------------------------------------------------------
void anticolMainLoop()
{
  fprintf(stderr,"AC thread...                    ok\n");  fflush(stdout);
  while(true)
  {
    if(picDistCaptors() == 0)
      usleep(100000);
    else
      usleep(100000);
  }  
}
//------------------------------------------------------------------------------
void anticolOnRecvCaptors(int n, float *values)
{
  if(n != 4)
  {
    fprintf(stderr,"<anticol.cpp> Nombre de capteurs incorrect (%d).\n",n);
    fflush(stdout);
    return;
  }
}
//------------------------------------------------------------------------------

