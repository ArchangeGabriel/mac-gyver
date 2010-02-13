#ifndef IO2H
#define IO2H

#include "../../common/simul.h"

#ifdef SIMULATION
#include "simul.h"
int get_input2  (PIC_ARG *SimulArg,int in_type,int in_index=0);
void set_output2(PIC_ARG *SimulArg,int out_type,int out_index,int out_value);
#else
int get_input2  (int in_type,int in_index=0);
void set_output2(int out_type,int out_index,int out_value);
#endif

#endif
