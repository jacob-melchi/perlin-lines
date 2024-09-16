#include "perlin.h"

vector randomField[(int)NUMVECS_X + 1][(int)NUMVECS_Y + 1];
vector subticks[NUMTICKS][NUMTICKS];

const double perlin_vector_x_increment = (SPACE / NUMVECS_X);
const double perlin_vector_y_increment = (SPACE / NUMVECS_Y);
const double subtick_increment         = (SPACE / NUMTICKS);

// returns a point between a and b
// factor w, in [0,1], will determine where the returned point will be
// on a straight line between a and b. w == 1 just returns a, w == 0 returns b.
static double lerp(double a, double b, double w) {
    // linear interpolation
    return (w * a) + ((1.0 - w) * b);
}

// returns a 'faded' value as specified by other implementations of perlin noise
static double fade(double x) {
    return ((6 * pow(x,5)) - (15 * pow(x,4)) + (10 * pow(x,3)));
}

static inline bool subtickIndexOOB(int x, int y) {
    return !((x < NUMTICKS) && (x > 0) &&\
             (y < NUMTICKS) && (y > 0));
}


// generates the perlin noise vector of a point, given coordinates + other parameters
// no cairo interfacing in here
double perlin_getPerlin(vector point, int norm) {
    // find which grid space point is in
    // add one for other corners
    /*
        v1 | v2
        -------
        v3 | v4
    */
    // get an index for the array of randomly-generated vectors
    int randIdx_x = (int)((point.x + SPACE/2) * (NUMVECS_X/SPACE));
    int randIdx_y = (int)((point.y + SPACE/2) * (NUMVECS_Y/SPACE));

    // check against array limits
    if(randIdx_x > NUMVECS_X - 1) { randIdx_x = NUMVECS_X - 1; }
    if(randIdx_x < 0)             { randIdx_x = 0; }
    if(randIdx_y > NUMVECS_Y - 1) { randIdx_y = NUMVECS_Y - 1; }
    if(randIdx_y < 0)             { randIdx_y = 0; }

    // retrieve vectors from field
    vector v1 = randomField[randIdx_x    ][randIdx_y + 1]; // vectors at the four corners (not the corners' coordinates!)
    vector v2 = randomField[randIdx_x + 1][randIdx_y + 1];
    vector v3 = randomField[randIdx_x    ][randIdx_y];
    vector v4 = randomField[randIdx_x + 1][randIdx_y];

    // get COORDINATES of corners in actual space
    vector c1, c2, c3, c4;
    c1.x = ((randIdx_x      )/(NUMVECS_X/SPACE) - SPACE/2);
    c1.y = ((randIdx_y + 1.0)/(NUMVECS_Y/SPACE) - SPACE/2);

    c2.x = ((randIdx_x + 1.0)/(NUMVECS_X/SPACE) - SPACE/2);
    c2.y = ((randIdx_y + 1.0)/(NUMVECS_Y/SPACE) - SPACE/2);

    c3.x = ((randIdx_x      )/(NUMVECS_X/SPACE) - SPACE/2);
    c3.y = ((randIdx_y      )/(NUMVECS_Y/SPACE) - SPACE/2);

    c4.x = ((randIdx_x + 1.0)/(NUMVECS_X/SPACE) - SPACE/2);
    c4.y = ((randIdx_y      )/(NUMVECS_Y/SPACE) - SPACE/2);
    
    // get offset vectors
    // multiply by scale to get vectors that look like they're from a 1x1 square
    vector u1, u2, u3, u4;
    double dir = -1.0;
    u1.x = (dir * (point.x - c1.x)) * (NUMVECS_X/SPACE);
    u1.y = (dir * (point.y - c1.y)) * (NUMVECS_Y/SPACE);

    u2.x = (dir * (point.x - c2.x)) * (NUMVECS_X/SPACE);
    u2.y = (dir * (point.y - c2.y)) * (NUMVECS_Y/SPACE);

    u3.x = (dir * (point.x - c3.x)) * (NUMVECS_X/SPACE);
    u3.y = (dir * (point.y - c3.y)) * (NUMVECS_Y/SPACE);

    u4.x = (dir * (point.x - c4.x)) * (NUMVECS_X/SPACE);
    u4.y = (dir * (point.y - c4.y)) * (NUMVECS_Y/SPACE);

    // normalize if argument is true
    if(norm == 1) {
        u1 = vector_normalize(u1);
        u2 = vector_normalize(u2);
        u3 = vector_normalize(u3);
        u4 = vector_normalize(u4);
    }

    // get dot products
    double d1, d2, d3, d4;
    double scl = 1;
    d1 = vector_dotProduct(u1, v1) * scl;
    d2 = vector_dotProduct(u2, v2) * scl;
    d3 = vector_dotProduct(u3, v3) * scl;
    d4 = vector_dotProduct(u4, v4) * scl;

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


bool perlin_setSubtick(int x, int y, vector* input) {
    if (subtickIndexOOB(x, y)) {
        return false;
    }

    subticks[x][y].x = input->x;
    subticks[x][y].y = input->y;

    return true;
}

bool perlin_getSubtick(int x, int y, vector* output) {
    if (subtickIndexOOB(x, y)) {
        return false;
    }
    
    output->x = subticks[x][y].x;
    output->y = subticks[x][y].y;

    return true;
}

vector perlin_interpSubticks(vector input) {
    vector output = {0, 0};
    double out_x  = 0;
    double out_y  = 0;
    int    x_idx  = -1;
    int    y_idx  = -1;

    // convert coordinate to index
    x_idx = (int)((input.x + (SPACE/2)) * (NUMTICKS/SPACE));
    y_idx = (int)((input.y + (SPACE/2)) * (NUMTICKS/SPACE));

    if (x_idx >= NUMTICKS) {
        x_idx = (NUMTICKS - 1);
    }

    if (x_idx < 0) {
        x_idx = 0;
    }

    if (y_idx >= NUMTICKS) {
        y_idx = (NUMTICKS - 1);
    }

    if (y_idx < 0) {
        y_idx = 0;
    }

    // THEN get sum/avg
    out_x = subticks[x_idx    ][y_idx    ].x +\
            subticks[x_idx + 1][y_idx    ].x +\
            subticks[x_idx    ][y_idx + 1].x +\
            subticks[x_idx + 1][y_idx + 1].x;

    out_y = subticks[x_idx    ][y_idx    ].y +\
            subticks[x_idx + 1][y_idx    ].y +\
            subticks[x_idx    ][y_idx + 1].y +\
            subticks[x_idx + 1][y_idx + 1].y;
    
    output.x = out_x / 4.0;
    output.y = out_y / 4.0;

    return output;
}