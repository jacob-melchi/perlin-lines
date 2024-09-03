#ifndef PERLIN_H
#define PERLIN_H

#include "vector.h"
#include "settings.h"

extern vector randomField[(int)(WID*RESOLUTION) + 1][(int)(HEI*RESOLUTION) + 1];

double getPerlin(vector point, int norm);


#endif /* PERLIN_H */