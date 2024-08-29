#include "perlin.h"

// returns a point between a and b
// factor w, in [0,1], will determine where the returned point will be
// on a straight line between a and b. w == 1 just returns a, w == 0 returns b.
double lerp(double a, double b, double w) {
    // linear interpolation
    return (w * a) + ((1.0 - w) * b);
}

// returns a 'faded' value as specified by other implementations of perlin noise
double fade(double x) {
    return ((6 * pow(x,5)) - (15 * pow(x,4)) + (10 * pow(x,3)));
    //return x;
}

// generates the perlin noise vector of a point, given coordinates + other parameters
// no cairo interfacing in here
double getPerlin(vector point, int norm) {
    // find which grid space point is in
    // add one for other corners
    /*
        v1 | v2
        -------
        v3 | v4
    */
    // get an index for the array of randomly-generated vectors
    int randIdx_x = (int)((point.x + SPACE/2) * (WID * RESOLUTION/SPACE));
    int randIdx_y = (int)((point.y + SPACE/2) * (HEI * RESOLUTION/SPACE));

    // check against array limits
    if(randIdx_x > (WID * RESOLUTION) - 1) { randIdx_x = (WID * RESOLUTION) - 1; }
    if(randIdx_x < 0) { randIdx_x = 0; }
    if(randIdx_y > (HEI * RESOLUTION) - 1) { randIdx_y = (HEI * RESOLUTION) - 1; }
    if(randIdx_y < 0) { randIdx_y = 0; }

    // retrieve vectors from field
    vector v1 = randomField[randIdx_x    ][randIdx_y + 1]; // vectors at the four corners (not the corners' coordinates!)
    vector v2 = randomField[randIdx_x + 1][randIdx_y + 1];
    vector v3 = randomField[randIdx_x    ][randIdx_y];
    vector v4 = randomField[randIdx_x + 1][randIdx_y];

    // get COORDINATES of corners in actual space
    vector c1, c2, c3, c4;
    c1.x = ((randIdx_x      )/((WID * RESOLUTION)/SPACE) - SPACE/2);
    c1.y = ((randIdx_y + 1.0)/((HEI * RESOLUTION)/SPACE) - SPACE/2);

    c2.x = ((randIdx_x + 1.0)/((WID * RESOLUTION)/SPACE) - SPACE/2);
    c2.y = ((randIdx_y + 1.0)/((HEI * RESOLUTION)/SPACE) - SPACE/2);

    c3.x = ((randIdx_x      )/((WID * RESOLUTION)/SPACE) - SPACE/2);
    c3.y = ((randIdx_y      )/((HEI * RESOLUTION)/SPACE) - SPACE/2);

    c4.x = ((randIdx_x + 1.0)/((WID * RESOLUTION)/SPACE) - SPACE/2);
    c4.y = ((randIdx_y      )/((HEI * RESOLUTION)/SPACE) - SPACE/2);
    
    // get offset vectors
    // multiply by scale to get vectors that look like they're from a 1x1 square
    vector u1, u2, u3, u4;
    double dir = -1.0;
    u1.x = (dir * (point.x - c1.x)) * ((WID * RESOLUTION)/SPACE);
    u1.y = (dir * (point.y - c1.y)) * ((HEI * RESOLUTION)/SPACE);

    u2.x = (dir * (point.x - c2.x)) * ((WID * RESOLUTION)/SPACE);
    u2.y = (dir * (point.y - c2.y)) * ((HEI * RESOLUTION)/SPACE);

    u3.x = (dir * (point.x - c3.x)) * ((WID * RESOLUTION)/SPACE);
    u3.y = (dir * (point.y - c3.y)) * ((HEI * RESOLUTION)/SPACE);

    u4.x = (dir * (point.x - c4.x)) * ((WID * RESOLUTION)/SPACE);
    u4.y = (dir * (point.y - c4.y)) * ((HEI * RESOLUTION)/SPACE);

    // normalize if argument is true
    if(norm == 1) {
        u1 = normalize(u1);
        u2 = normalize(u2);
        u3 = normalize(u3);
        u4 = normalize(u4);
    }

    // get dot products
    double d1, d2, d3, d4;
    double scl = 1;
    d1 = dotProduct(u1, v1) * scl;
    d2 = dotProduct(u2, v2) * scl;
    d3 = dotProduct(u3, v3) * scl;
    d4 = dotProduct(u4, v4) * scl;

    // linear interpolation of dot products
    // double xWeight1 = (c2.x - point.x)/(c2.x - c1.x);
    // double xWeight2 = (c4.x - point.x)/(c4.x - c3.x);
    // double yWeight  = (c1.y - point.y)/(c1.y - c3.y);
    double x1  = lerp(d1, d2, fade((c2.x - point.x)/(c2.x - c1.x)));
    double x2  = lerp(d3, d4, fade((c4.x - point.x)/(c4.x - c3.x)));

    double value = lerp(x1, x2, fade((c3.y - point.y)/(c3.y - c1.y)));

    // printf("xWeight1: %f  xWeight12: %f  yWeight: %f\n", xWeight1, xWeight2, yWeight);

    return value;
}