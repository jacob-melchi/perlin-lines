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

// gets the angle between two input vectors
// [0, pi]
double getAngle(vector v1, vector v2) {
    return acos(dotProduct(v1, v2) / (magnitude(v1) * magnitude(v2)));
}

// returns the distance between two vectors (assuming they both start at [0,0])
double dist(vector v1, vector v2) {
    return magnitude((vector){v1.x - v2.x, v1.y - v2.y});
}


// TODO: verify this. 
// //    probably not even the best way to do things given the new idea
// //    for implementing gravity (just updating [a copy of] the main field)
/*
// 'reflects' one vector off another
// v1 hits v2
vector reflect(vector v1, vector v2) {
    // bounce one vector off another - second is stationary 

    //          V1
    // \       /\
    //  \      /
    //   \    /
    //    \  /
    //     \/
    // ----------------> V2

    // sign difference? 2<V2 - <V1

    double angleV1 = getAngle(v1, (vector){1,0}); // [0, pi]
    if(v1.y < 0) angleV1 = (M_PI * 2.0) - angleV1;

    double angleV2 = getAngle(v2, (vector){1,0}); // angle of v2 from horizontal
    if(v2.y < 0) angleV2 = (M_PI * 2.0) - angleV2; // want [0, 2pi]

    double angleOut = (angleV2 * 2.0) - angleV1;
    return (vector){cos(angleOut), sin(angleOut)};
}
*/