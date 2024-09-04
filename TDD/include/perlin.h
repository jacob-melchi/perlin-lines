#ifndef PERLIN_H
#define PERLIN_H

#include "vector.h"
#include "settings.h"
#include "stdbool.h"

extern vector randomField[(int)(WID*RESOLUTION) + 1][(int)(HEI*RESOLUTION) + 1];

#ifdef UNITTEST
extern vector subticks[NUMTICKS][NUMTICKS];
#endif


double perlin_getPerlin(vector point, int norm);
bool   perlin_setSubtick(int x, int y, vector* input);
bool   perlin_getSubtick(int x, int y, vector* output);

#endif /* PERLIN_H */