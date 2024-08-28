#ifndef VECTOR_H
#define VECTOR_H

#include "math.h"

// 2D vector type
typedef struct vector { 
    double x;
    double y;
} vector;

double dotProduct(vector v1, vector v2);
double magnitude(vector vec);
vector normalize(vector vec);
double getAngle(vector v1, vector v2);
double dist(vector v1, vector v2);

#endif /* VECTOR_H */