#ifndef PERLIN_H
#define PERLIN_H

#include "vector.h"
#include "settings.h"

#if STATICFIELD
// not used right now. was storing a massive non-random field before.
vector randomField[(int)(WID*RESOLUTION)][(int)(HEI*RESOLUTION)] = {};

#else
extern vector randomField[(int)(WID*RESOLUTION) + 1][(int)(HEI*RESOLUTION) + 1];
#endif


double getPerlin(vector point, int norm);


#endif /* PERLIN_H */