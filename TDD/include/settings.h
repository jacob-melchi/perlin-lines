#ifndef SETTINGS_H
#define SETTINGS_H


#define WID 3000 // pixel resolution of output
#define HEI 3000

#define RESOLUTION .001 // use xx% of image dimension for number of grid squares
// increase = more vectors (nVectors = wid * resolution)
// default has been .005; .003 might be cooler and smoother?

// number of vectors used for perlin noise along each axis
#define NUMVECS_X (WID * RESOLUTION)
#define NUMVECS_Y (HEI * RESOLUTION)

/** total size of space **/
#define SPACE       50.0          // width of total transformed space
#define HALFSPACE   (SPACE / 2.0) // half-width of space

/** noise generation **/
#define NORM        0        // normalize offset vectors?
#define CARDINALS   1        // use only cardinal directions for random vectors?
#define SCALERANGE  1        // divide noise by sqrt(2)/2?
#define ANGSHIFT    0        // shift angle by some constant? (subtract)

/** noise visualization **/
#define DRAWTICKS   1        // draw perlin noise field?
#define NUMTICKS    750      // how many ticks to draw for non-greyscale visualization
#define TICKMULT    5.0      // scale factor for length of each tick (divide calculated perlin vector)
#define GREYSCALE   0        // draw greyscale squares based on noise value?

/** line definitions **/
#define NUMSTEPS    10000    // number of steps to take per line
#define NUMLINES    2        // number of lines to draw
#define LINESPACE   48       // range (units) along which to spread lines
#define PARALLEL    0        // step-by-step? (otherwise, line-by-line)
#define DIRECTIONS  0        // draw from both directions?

/** collisions **/
#define DOBOUNCES   0        // do bouncing?
#define KEEPOUT     0.5      // max distance lines can be from one another
#define BOUNCETIME  100      // number of steps for which a path will be affected by a collision
#define BOUNCESPEED 8        // rough step size of added bouncing velocity
#define DRAWBOUNCES 0        // indicate bounced sections in red?
#define NOJUMPS     1        // skip drawing big jumps in paths?

/** gravity **/
#define DOGRAVITY   0        // make lines repel each other? (yes yes, gravity isn't actually repulsive.)
#define MEMORY      NUMLINES // nuber of lines "backwards" to look for gravity comparisons
#define GRAVRADIUS  0.4      // radius in units of circle used for checking whether a line should be repelled
#define GRAVSTREN   45.0     // big G; some constant to multiply into gravity equation
#define GRAVSUM     1        // try summing gravity from all points in range?
#define GRAVSUMSCL  0        // scale summed gravity vector by number of vectors in sum? (post-normalization)
#define GRAVDECAY   0.95     // need acceleration. reduce pushing vector by this much every step.

/** misc **/
#define FILENAME    0        // generate a second file w/ dynamic filename?


#endif /* SETTINGS_H */