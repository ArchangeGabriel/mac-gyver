#ifndef IO1H
#define IO1H

#include "../common/simul.h"

#ifdef SIMULATION
#include "../pic/simul.h"
int get_input1  (PIC_ARG *SimulArg,int in_type,int in_index=0);
void set_output1(PIC_ARG *SimulArg,int out_type,int out_index,int out_value);
#else
int get_input1  (int in_type,int in_index=0);
void set_output1(int out_type,int out_index,int out_value);
#endif

#endif
