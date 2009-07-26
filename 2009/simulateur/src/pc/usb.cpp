#include "../common/simul.h"

#ifdef SIMULATION
#include "usb_simul.cpp"
#else
#include "usb_real.cpp"
#endif
