
// Commenter les 3 lignes suivantes pour passer en compilation pour de vrai


#ifndef SIMULATION
#define SIMULATION
#endif


#define USE_SDL_FRANCHEMENT_OUAIS_TAS_VU

// Ne pas commenter
#ifdef SIMULATION
#define SIMULARG2(x,y)     x,y
#define SIMULARG3(x,y,z)   x,y,z
#define SIMULARG4(w,x,y,z) w,x,y,z
#else
#define SIMULARG2(x,y)     y
#define SIMULARG3(x,y,z)   y,z
#define SIMULARG4(w,x,y,z) x,y,z
#endif



