#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "perlin.h"
#include "settings.h"
#include "draw.h"

// TODO: want to either:
// //    1. keep a separate field with gravity updated by lines
// //    2. update/hardcode the original field with the lines' gravity

// TODO: OR,, process in parallel and just assume that a line will only
// //    ever bounce off of the ones it's 'between' at the beginning?
// // //    this would only work when starting all particles from the
// // //    same side, and maybe not always be true but.. worth trying

// not a lot of rightward movement because perlin noise doesn't reach its exremes very often.
// interpreting these extremes as angles, they would be 0 and 2pi.
// BOTH of these extreme angles are ones that point right.
// thus, there's going to be a ~1/4 circle (? that much? is this accurate?) that just isn't touched on very much.
// additionally, a line isn't going to reach a huge peak before it reaches the mellower vectors surrounding it.
// perlin noise is continuous.
// at the moment, ANGSHIFT is the only thing accounting for this, and it really doesn't change the situation.s
// rotating the entire image will change which direction the lines flow towards.
// // could there be some sort of.. stretching of the output range? log or otherwise?


// store the path of each line.
// going to try collision detection.
// switch order of for loops? go step-by-step and not line-by-line?
vector paths[NUMLINES][NUMSTEPS + 1]; // +1 to index for initial position
// keep track of additional redirecting vector if collision happens
int isBouncing[NUMLINES] = {0}; // state of line - has it reflected recently?
int bounceTracker[NUMLINES][NUMSTEPS + 1] = {0};
vector perlinTest[NUMLINES];
int direction = 1;
const double stepSize = .01; // size of step for drawing

// gravity-related variables
vector gravityCandidates[NUMSTEPS];
vector closestPoint;
vector repel = (vector){0, 0};
vector repelOverTime;

// timekeeping
clock_t start, end;
double cpuTimeUsed, totalTimeUsed;

// static cardinal directions to choose from if so desired
vector choices[8] = {
    (vector){1, 0},
    (vector){M_SQRT2/2.0, M_SQRT2/2.0},
    (vector){0, 1},
    (vector){-M_SQRT2/2.0, M_SQRT2/2.0},
    (vector){-1, 0},
    (vector){-M_SQRT2/2.0, -M_SQRT2/2.0},
    (vector){0, -1},
    (vector){M_SQRT2/2.0, -M_SQRT2/2.0}
};

static inline int paramValidation(void) {
    if(MEMORY > NUMLINES && DOGRAVITY) {
        printf("\n!!!!!!! number of lines to check against greater than number of actual lines. exiting. !!!!!!!\n");
        return 1;
    }

    if((DOGRAVITY) && (GRAVRADIUS >= (LINESPACE / (NUMLINES - 1.0)))) {
        printf("\n!!!!!!! gravity radius larger than gap between starting points. exiting. !!!!!!!\n");
        return 1;
    }

    return 0;
}

#ifndef UNITTEST
int main(int argc, char* argv[]) {
#else
int dummy_main(int argc, char* argv[]) {
#endif

    if (paramValidation()) {
        return 1;
    }
    
    srand(time(NULL)); // seed rand()

    // range - due to interpolation, noise won't really ever reach [-1, 1].
    // this seems to be the real / "effective" range.
    // // this is correct!
    const double range = sqrt(2.0)/2.0; // 0.707 :)

    cairo_surface_t* surface; // create surface and context
    cairo_t* cr;
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WID, HEI); // ARGB32 type (R/G/B + alpha)
    cr = cairo_create(surface);

    draw_fillBackground(cr, surface);

    for(int i = 0; i < WID * RESOLUTION + 1; i++) { // generate random field of vectors given resolution
        for(int j = 0; j < HEI * RESOLUTION + 1; j++) {
            #if CARDINALS
            int seed = rand() % 8;

            randomField[i][j] = choices[seed];
            // printf("%d\n", a);

            #else
            randomField[i][j].x = (double)(rand()/(RAND_MAX/2.0) - 1.0); 
            randomField[i][j].y = (double)(rand()/(RAND_MAX/2.0) - 1.0);
            randomField[i][j] = vector_normalize(randomField[i][j]); // want unit vectors
            #endif
            // *!* TODO: *!* maybe randomize the magnitudes a little bit? add noise?
        }
    }

    // set up cairo pen
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_width(cr, 0.02);
    
    double maxAng = 0; // store angle values for reporting later
    double minAng = 5;
    vector point;
    vector perlinVec;
    int bonks = 0; // collision detection counter

    // populate 'subtick' array
    for(int i = 0; i < NUMTICKS; i++) {
        for(int j = 0; j < NUMTICKS; j++) {
                // increment in defined space for x/y
                double tick = SPACE/(1.0 * NUMTICKS);

                point.x = i/((1.0 * NUMTICKS)/SPACE) - SPACE/2.0; // get even-spaced x and y coordinates
                point.y = j/((1.0 * NUMTICKS)/SPACE) - SPACE/2.0;
                
                double noise = perlin_getPerlin(point, NORM);
                if(SCALERANGE) noise /= range;

                double ang = ((noise + 1) * M_PI) - (ANGSHIFT);
                
                // if(ang > 2 * M_PI) ang = (2 * M_PI) - .01;
                // if(ang < 0) ang = 0.0;

                perlinVec.x = cos(ang);
                perlinVec.y = sin(ang);

                perlin_setSubtick(i, j, &perlinVec);
        }
    }

    if (DRAWTICKS) {
        draw_plotSubTicks(cr);
        draw_plotPerlinVectors(cr);
    }

    
    vector reflecting[NUMLINES] = {}; // store reflected vector for bouncing
    // want to keep reflecting vector, and shrink/add it to current line as long as this status is active

    printf("\n");

    // init paths with first step
    #if DIRECTIONS
    for(int i = 0; i < NUMLINES; i++) {
        paths[i][0].x = ((i % 2) == 0) ? 25 : -25;
        paths[i][0].y = (-1.0 * (LINESPACE/2.0)) + i * (LINESPACE / (NUMLINES - 1.0));
    }

    #else
    for(int i = 0; i < NUMLINES; i++) {
        paths[i][0].x = 25;
        paths[i][0].y = (-1.0 * (LINESPACE/2.0)) + i * (LINESPACE / (NUMLINES - 1.0));
    }
    #endif
    
    // will want to draw AFTER.
    // can just loop through each path and draw once lines are defined.
    #if PARALLEL
    for(int n = 1; n < NUMSTEPS + 1; n++) {
        for(int i = 0; i < NUMLINES; i++) {

    #else // SERIES
    for(int i = 0; i < NUMLINES; i++) {

        if(i % 2 == 0) { direction = 1;  } // alternate odd and even
        else           { direction = -1; }

        printf("line %02d of %02d: ", (i+1), NUMLINES);
        start = clock();
        for(int n = 1; n < NUMSTEPS + 1; n++) {  
    #endif
            // 1. TENTATIVELY add noise to all paths
            point = paths[i][n-1]; // start at previous point
            double noise = perlin_getPerlin(point, NORM); // get noise
            if(SCALERANGE) noise /= range; // "normalize" noise
            
            if(noise > maxAng) maxAng = noise; // check values & store max
            if(noise < minAng) minAng = noise;

            // get angle from noise
            double ang = ((noise + 1) * M_PI) - (ANGSHIFT);
            // if(ang > 2 * M_PI) ang = (2 * M_PI) - .01;
            // if(ang < 0) ang = 0.0;

            // turn angle into unit vector (sin^2 + cos^2 = 1)
            perlinTest[i].x = cos(ang);
            perlinTest[i].y = sin(ang);

            // store new paths in temp array (is this necessary?)
            #if DIRECTIONS
            if(direction == -1) { paths[i][n].x = point.x + (perlinTest[i].x * stepSize * -1); }
            else                { paths[i][n].x = point.x + (perlinTest[i].x * stepSize);      }

            #else
            paths[i][n].x = point.x + (perlinTest[i].x * stepSize);
            #endif

            paths[i][n].y = point.y + (perlinTest[i].y * stepSize);

            #if DOGRAVITY
            /* see vecField.c */
            #endif    

            // it seems that there needs to be some sort of acceleration/hysteresis on the gravity,
            // otherwise a line will go immediately back to the one it just fled from, and jagged edges appear.
            // so -- always do this:
            paths[i][n].x += repelOverTime.x;
            paths[i][n].y += repelOverTime.y;

            repelOverTime.x *= GRAVDECAY; // decay field strength over time
            repelOverTime.y *= GRAVDECAY;
        } // end loop over n steps


        #if DOBOUNCES
        /* see vecField.c */
        #endif

        end = clock(); // store time in clock cycles
        cpuTimeUsed = cpuTimeUsed = ((double) (end - start)) / CLOCKS_PER_SEC; // convert to seconds 
        totalTimeUsed += cpuTimeUsed; // add to total time
        printf("%.2f seconds\n", cpuTimeUsed); // print progress
    } // end loop over i lines

    printf("lines calculated in a total of %.2f seconds.\n", totalTimeUsed);

    

    if(DOBOUNCES && DRAWBOUNCES) {
        cairo_set_source_rgba(cr, 1, 0.3, 0.4, 1); // set to red
        cairo_set_line_width(cr, 0.08); // set thinner
        for(int i = 0; i < NUMLINES; i++) {
            cairo_move_to(cr, paths[i][0].x, paths[i][0].y); // move to head of path

            for(int n = 1; n < NUMSTEPS + 1; n++) { // for each step in path
                cairo_move_to(cr, paths[i][n].x, paths[i][n].y);
                
                    if(bounceTracker[i][n] == 1) {
                        //cairo_set_source_rgba(cr, 1, 0.3, 0.4, 1);
                        cairo_line_to(cr, paths[i][n-1].x, paths[i][n-1].y); // line to
                        cairo_stroke(cr);
                    }
            }
        }
    }

    printf("%f %f\n", minAng, maxAng); // min and max angles found with perlin noise?
    
    draw_plotPaths(cr, paths);

    cairo_destroy(cr); // clean up and save
    cairo_surface_write_to_png(surface, "out.png");

    #if FILENAME
    // resolution, directions, grav radius,
    char[30] resString, dirString, radString numString;

    sprintf(resString, "res %0.4f ", RESOLUTION);
    sprintf(numString, "%d lines",    NUMLINES);

    if(DOGRAVITY)  { sprintf(radString, "radius %0.2f ", GRAVRADIUS); }
    else           { sprintf(radString, " "); }

    if(DIRECTIONS) { dirString = "LR "; }
    else           { dirString = "";    }

    const char[] A = strcat(dirString, numString);
    const char[] B = strcat(A, resString);
    const char[] C = strcat(B, radString);
    
    cairo_surface_write_to_png(surface, C);
    #endif
     

    cairo_surface_destroy(surface);
    printf("\a"); // make a lil beep
    return 0;
}
