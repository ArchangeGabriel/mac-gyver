#ifndef IOH
#define IOH

#include "pic/simul.h"
int get_input  (PIC_ARG *SimulArg,int in_type,int in_index=0);
void set_output(PIC_ARG *SimulArg,int out_type,int out_index,int out_value);

#endif
