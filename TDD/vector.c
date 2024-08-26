/*
vector type + operations
*/
#include "vector.h"
 
// returns magnitude of input vector
double magnitude (vector vec) {
    return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

// non-destructively returns normalized version of input vector
vector normalize(vector vec) {
    double mag = magnitude(vec);

    if (mag == 0) {
        return vec;
    }

    vector out;
    out.x = (vec.x / mag);
    out.y = (vec.y / mag);

    return out;
}

// returns dot product of two input vectors
double dotProduct(vector v1, vector v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}