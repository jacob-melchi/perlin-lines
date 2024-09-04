#ifndef VECTOR_H
#define VECTOR_H

#include "math.h"

// 2D vector type
typedef struct vector { 
    double x;
    double y;
} vector;

double vector_dotProduct(vector v1, vector v2);
double vector_magnitude(vector vec);
vector vector_normalize(vector vec);
double vector_getAngle(vector v1, vector v2);
double vector_distance(vector v1, vector v2);

#endif /* VECTOR_H */