#include "cairo.h"
#include "vector.h"
#include "perlin.h"
#include "settings.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// TODO: want to either:
// //    1. keep a separate field with gravity updated by lines
// //    2. update/hardcode the original field with the lines' gravity

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


int manin(int argc, char* argv[]) {
    if(MEMORY > NUMLINES && DOGRAVITY) {
        printf("\n!!!!!!! number of lines to check against greater than number of actual lines. exiting. !!!!!!!\n");
        return 1;
    }
    if((DOGRAVITY) && (GRAVRADIUS >= (LINESPACE / (NUMLINES - 1.0)))) {
        printf("\n!!!!!!! gravity radius larger than gap between starting points. exiting. !!!!!!!\n");
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
    cairo_translate(cr, WID/2.0, HEI/2.0);
    cairo_scale(cr, WID/SPACE, -HEI/SPACE); // want to translate -1.0 <-> 1.0 to 0 <-> 500

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // fill background
    cairo_rectangle(cr, -SPACE/2, -SPACE/2, SPACE, SPACE);
    cairo_fill(cr);

    for(int i = 0; i < WID * RESOLUTION + 1; i++) { // generate random field of vectors given resolution
        for(int j = 0; j < HEI * RESOLUTION + 1; j++) {
            #if CARDINALS
            int seed = rand() % 8;

            randomField[i][j] = choices[seed];
            // printf("%d\n", a);

            #else
            randomField[i][j].x = (double)(rand()/(RAND_MAX/2.0) - 1.0); 
            randomField[i][j].y = (double)(rand()/(RAND_MAX/2.0) - 1.0);
            randomField[i][j] = normalize(randomField[i][j]); // want unit vectors
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
    // print perlin field
    #if DRAWTICKS
    for(int i = 0; i < NUMTICKS; i++) {
        for(int j = 0; j < NUMTICKS; j++) {
                // increment in defined space for x/y
                double tick = SPACE/(1.0 * NUMTICKS);

                point.x = i/((1.0 * NUMTICKS)/SPACE) - SPACE/2.0; // get even-spaced x and y coordinates
                point.y = j/((1.0 * NUMTICKS)/SPACE) - SPACE/2.0;
                
                double noise = getPerlin(point, NORM);
                if(SCALERANGE) noise /= range;

                #if GREYSCALE
                cairo_set_source_rgba(cr, noise, noise, noise, 1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rectangle(cr, point.x, point.y, tick, tick);
                cairo_fill(cr);

                #else
                double ang = ((noise + 1) * M_PI) - (ANGSHIFT);
                
                // if(ang > 2 * M_PI) ang = (2 * M_PI) - .01;
                // if(ang < 0) ang = 0.0;

                vector perlinVec;
                perlinVec.x = cos(ang);
                perlinVec.y = sin(ang);
                
                cairo_set_source_rgba(cr, 0, 1, 0, 1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rel_line_to(cr, .01, .01);
                cairo_stroke(cr);

                double hue = cos(ang);
                hue = (hue < 0) ? 0 : hue;
                cairo_set_source_rgba(cr, ((ang + ANGSHIFT)/(2.0 * M_PI)),
                                      0,
                                      (((2.0 * M_PI) - ang + ANGSHIFT)/(2.0 * M_PI)),
                                      1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rel_line_to(cr, perlinVec.x/(TICKMULT*2), perlinVec.y/(TICKMULT*2));
                cairo_stroke(cr);
                #endif
        }
    }

    // large grid
    for(int i = 0; i < WID * RESOLUTION; i++) { // draw random vector field
        for(int j = 0; j < HEI * RESOLUTION; j++) {
            // draw lines
            cairo_move_to(cr,
                        i/(WID * (RESOLUTION/SPACE)) - SPACE/2,
                        j/(HEI * (RESOLUTION/SPACE)) - SPACE/2);
            cairo_rel_line_to(cr, 
                            randomField[i][j].x/TICKMULT,
                            randomField[i][j].y/TICKMULT);
            cairo_set_source_rgba(cr, 0, 1, 0, 0.7); // green semitrasnparent
            cairo_stroke(cr);
            
            // draw dots at origins of lines
            cairo_move_to(cr,
                        i/(WID * (RESOLUTION/SPACE)) - SPACE/2,
                        j/(HEI * (RESOLUTION/SPACE)) - SPACE/2);
            cairo_set_source_rgba(cr, 1, 1, 1, 1); // white
            cairo_rel_line_to(cr, -0.01, -0.01);
            cairo_stroke(cr);
        }
    }
    #endif
    
    
    
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
            double noise = getPerlin(point, NORM); // get noise
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
            // make lines repel each other. each point on each line emits a "field" that
            // pushes other lines away (if they get within a certain radius)
            int candidateTracker = 0;
            int lookBack = ((i - MEMORY) >= 0) ? (i - MEMORY) : 0;

            for(int otherLine = lookBack; otherLine < i; otherLine++) { // for each previous line
                candidateTracker = 0; // need to count number of "hits"

                for(int j = 0; j < NUMSTEPS; j++) { // reset list of candidates
                    gravityCandidates[j] = (vector){SPACE + 10, SPACE + 10}; // outside defined bounds
                }

                // for some radius around the head of the current line
                for(int j = 0; j < NUMSTEPS; j++) { // go over entirety of each other line
                    if(dist(paths[i][n], paths[otherLine][j]) < GRAVRADIUS) { // check if another line is within said radius
                        // add to some list
                        gravityCandidates[candidateTracker] = paths[otherLine][j]; // store candidate
                        candidateTracker++; // increment counter
                    }
                }
                
                for(int j = 0; j < candidateTracker; j++) { // get closest point
                    if(j == 0) {
                        closestPoint = gravityCandidates[j]; // just store first candidate
                    }
                    else {
                        if(dist(gravityCandidates[j], paths[i][n]) < dist(closestPoint, paths[i][n])) {
                            closestPoint = gravityCandidates[j]; // overwrite, if even closer point, store that
                        }
                    }
                }

                // repel current line radially away from that point
                if(candidateTracker != 0) { // make sure there's a point in range!
                    // get vector pointing from line to head
                    repel = (vector){0, 0}; // zero out repel vector

                    #if GRAVSUM
                    for(int j = 0; j < candidateTracker; j++) { // sum gravity of ALL points in range
                        repel.x += (paths[i][n].x - gravityCandidates[j].x);
                        repel.y += (paths[i][n].y - gravityCandidates[j].y);
                    }

                    #else
                    repel.x = paths[i][n].x - closestPoint.x;
                    repel.y = paths[i][n].y - closestPoint.y;
                    #endif

                    // 1) normalize
                    repel = normalize(repel);

                    // 2) apply inverse square (and step size?)
                    double d = dist(closestPoint, paths[i][n]);
                    double r = GRAVRADIUS;
                    double alpha;
                    if((r - d) <= 0) { alpha = 0; }
                    else { alpha = (GRAVSTREN * ((r * r) - (2 * r * d) + (d * d))) / (r * r); }

                    #if GRAVSUMSCL
                    alpha *= candidateTracker; // scale gravity by number of points?
                    #endif

                    repel.x *= alpha;
                    repel.y *= alpha;

                    // 3) add to original
                    // paths[i][n].x += repel.x;
                    // paths[i][n].y += repel.y;
                    repelOverTime.x += repel.x;
                    repelOverTime.y += repel.y;
                }
            } // end loop over other lines (for comparison)
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
        for(int currentLine = 0; currentLine < NUMLINES; currentLine++) {
            // TODO: lines bounce ~through~ each other sometimes.
            // // add some sort of check for resultant directions?? something else?
            // // just compensate for acos in the first place maybe?
            if(isBouncing[currentLine] > 0) {
                //printf("bouncing %d\n", isBouncing[currentLine]);
                bounceTracker[currentLine][n] = 1;
                isBouncing[currentLine]--;

                float multiplier = (isBouncing[currentLine] / (1.0 * BOUNCETIME)) * stepSize * BOUNCESPEED;

                paths[currentLine][n].x += ((reflecting[currentLine].x) * multiplier);
                paths[currentLine][n].y += ((reflecting[currentLine].y) * multiplier);
            }
            else {
                for(int otherLine = 0; otherLine < NUMLINES; otherLine++) { // for each previous line
                    // 2. check if this addition causes any collisions
                    // if collision will happen:
                    // // 1. reflect direction of current path
                    // // 2. keep this reflection, decay it for some time
                    if(otherLine != currentLine) { // don't check path against itself
                        if(dist(paths[currentLine][n], paths[otherLine][n]) < KEEPOUT) { // 1. new points in test[]
                            // 3a. if addition causes two "heads" to collide, bounce them both

                            // if A and B collide, don't want to check AGAIN for B and A colliding
                            if(isBouncing[currentLine] == 0) {
                                printf("HIT HEAD %d %d\n", currentLine, otherLine);
                                isBouncing[currentLine] = BOUNCETIME; // bounce counter
                                isBouncing[otherLine]   = BOUNCETIME;

                                bounceTracker[currentLine][n] = 1; // bounce tracker for drawing
                                bounceTracker[otherLine][n]   = 1;

                                reflecting[currentLine] = reflect(perlinTest[currentLine], perlinTest[otherLine]);
                                reflecting[otherLine]   = reflect(perlinTest[otherLine], perlinTest[currentLine]);

                                paths[currentLine][n].x = point.x + (reflecting[currentLine].x * stepSize);
                                paths[currentLine][n].y = point.y + (reflecting[currentLine].y * stepSize);

                                paths[otherLine][n].x = point.x + (reflecting[otherLine].x * stepSize);
                                paths[otherLine][n].y = point.y + (reflecting[otherLine].y * stepSize);
                            }
                        }
                    }

                    for(int prevSteps = 0; prevSteps < n - 1; prevSteps++) { // up to (current step - 1)
                        if(otherLine != currentLine) { // don't check path against itself
                            if(dist(paths[currentLine][n], paths[otherLine][prevSteps]) < KEEPOUT) { // 2. all other points
                                // 3b. if collision is between a point and an old bit of a
                                //     path, bounce only the incident line
                                // paths[i][n].x = paths[i][n-1].x; - (perlinTest[i].x * stepSize * 3);

                                printf("HIT PREV %d\n", currentLine);
                                bounceTracker[currentLine][n] = 1;
                                
                                // indicate that line is bouncing
                                isBouncing[currentLine] = BOUNCETIME;

                                // get reflected version of current line's velocity vector
                                reflecting[currentLine] = reflect(perlinTest[currentLine], paths[otherLine][prevSteps]);

                                // 4. go back and actually add to each path accordingly
                                paths[currentLine][n].x = point.x + (reflecting[currentLine].x * stepSize);
                                paths[currentLine][n].y = point.y + (reflecting[currentLine].y * stepSize);
                                break;
                            }
                        }
                    }
                }
            }
        }
        #endif

        end = clock(); // store time in clock cycles
        cpuTimeUsed = cpuTimeUsed = ((double) (end - start)) / CLOCKS_PER_SEC; // convert to seconds 
        totalTimeUsed += cpuTimeUsed; // add to total time
        printf("%.2f seconds\n", cpuTimeUsed); // print progress
    } // end loop over i lines

    printf("lines calculated in a total of %.2f seconds.\n", totalTimeUsed);

    // draw!
    cairo_set_line_width(cr, 0.2);
    // just setting cairo to dots doesn't play nice with lots of closely-packed points:
    const double dashes[2] = {0.001, .7}; 
    cairo_set_dash(cr, dashes, 2, 0.0);

    for(int i = 0; i < NUMLINES; i++) { // for each line...
        cairo_set_source_rgba(cr, 0.1 + (i * 0.7/NUMLINES), 0.3, 0.4, 1); // get a color gradient
        cairo_move_to(cr, paths[i][0].x, paths[i][0].y); // go to beginning of path

        for(int n = 1; n < NUMSTEPS + 1; n++) { // for each step in current line...
            #if NOJUMPS
            // TODO: are we getting NaNs again????
            if(dist(paths[i][n], paths[i][n-1]) > 5) { // if there's some massive jump...
                // need to move, otherwise there will still be a weird long-ass line
                cairo_move_to(cr, paths[i][n].x, paths[i][n].y);
                continue; // next point
            }
            #endif

            cairo_line_to(cr, paths[i][n].x, paths[i][n].y); // get the current segment and draw it
            cairo_stroke(cr);

            cairo_move_to(cr, paths[i][n].x, paths[i][n].y); // jump to end of segment
            cairo_set_source_rgba(cr, 0.1 + (i * 0.7/NUMLINES), 0.3, 0.4, 1); // increment gradient
        }
    }

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

    /*
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_width(cr, .01);
    const double foo[2] = {0.001, .03};
    cairo_set_dash(cr, foo, 2, 0.0);
    cairo_set_source_rgba(cr, 0, 1.0, 0, 1.0);
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            cairo_set_source_rgba(cr, i/18.0, 1.0, j/18.0, 1.0);
            drawCurve(cr, 1000, .01, (-0.9 + (i * 0.2)), (-0.9 + (j * 0.2)), 0);
            drawCurve(cr, 1000, .05, 0, 0, 0);
        }
    }
    */

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
