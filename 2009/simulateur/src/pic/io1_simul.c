#include <pthread.h>

#include "io1.h"

//------------------------------------------------------------------------------
int get_input1(PIC_ARG *SimulArg,int in_type,int in_index)
{
  return robot_t::pic_io(SimulArg->robot,in_type,in_index);
}
//------------------------------------------------------------------------------
void set_output1(PIC_ARG *SimulArg,int out_type,int out_index,int out_value)
{
  robot_t::pic_io(SimulArg->robot,out_type,out_index,out_value);
}
//------------------------------------------------------------------------------
