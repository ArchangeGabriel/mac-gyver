#ifndef WEBCAMLISTH
#define WEBCAMLISTH

#include "../../common/simul.h"

#ifdef SIMULATION

#define WEBCAM1 "/dev/video1"
#define WEBCAM2 "/dev/video2"

#else

#define WEBCAM1 "0"
#define WEBCAM2 "1"

#endif

#endif
