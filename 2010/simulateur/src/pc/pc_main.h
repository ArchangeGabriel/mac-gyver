#ifndef PCMAINH
#define PCMAINH

#include "../common/simul.h"

#ifdef SIMULATION
int pc_main(int, char**);
#else
int main(int argc, char** argv);
#endif

#endif
