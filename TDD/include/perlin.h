#ifndef PERLIN_H
#define PERLIN_H

#include "vector.h"
#include "settings.h"
#include "stdbool.h"

extern vector randomField[(int)(WID*RESOLUTION) + 1][(int)(HEI*RESOLUTION) + 1];

#ifdef UNITTEST
extern vector subticks[NUMTICKS][NUMTICKS];
#endif

extern const double perlin_vector_x_increment;
extern const double perlin_vector_y_increment;
extern const double subtick_increment;

double perlin_getPerlin(vector point, int norm);
bool   perlin_setSubtick(int x, int y, vector* input);
bool   perlin_getSubtick(int x, int y, vector* output);
vector perlin_interpSubticks(vector input);

#endif /* PERLIN_H */