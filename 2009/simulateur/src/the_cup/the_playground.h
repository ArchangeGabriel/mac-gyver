#ifndef THE_PLAYGROUND_H
#define THE_PLAYGROUND_H

#define pgWidth   3
#define pgHeight  2.622

#include "../simul/object.h"

void add_obstacles(simul_info_t *info);

void draw_playground_2D(double scale);
void draw_playground_3D();

#endif
