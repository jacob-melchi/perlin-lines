#include "draw.h"

void draw_plotSubTicks(cairo_t* cr) {
    vector point;
    vector perlinVec;

    for(int i = 0; i < NUMTICKS; i++) {
        for(int j = 0; j < NUMTICKS; j++) {
                point.x = (i * subtick_increment) - HALFSPACE; // get even-spaced x and y coordinates
                point.y = (j * subtick_increment) - HALFSPACE;

                #if GREYSCALE
                cairo_set_source_rgba(cr, noise, noise, noise, 1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rectangle(cr, point.x, point.y, subtick_increment, subtick_increment);
                cairo_fill(cr);
                #endif
                
                perlin_getSubtick(i, j, &perlinVec);

                double angle = acos(perlinVec.x);
                if (perlinVec.y < 0) {
                    angle = (2.0 * M_PI) - acos(perlinVec.x);
                }

                // print perlin field?
                cairo_set_source_rgba(cr, 0, 1, 0, 1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rel_line_to(cr, .01, .01);
                cairo_stroke(cr);

                double hue = perlinVec.x;
                hue = (hue < 0) ? 0 : hue;
                cairo_set_source_rgba(cr, ((angle + ANGSHIFT)/(2.0 * M_PI)),
                                      0,
                                      (((2.0 * M_PI) - angle + ANGSHIFT)/(2.0 * M_PI)),
                                      1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rel_line_to(cr, perlinVec.x/(TICKMULT*2), perlinVec.y/(TICKMULT*2));
                cairo_stroke(cr);
        }
    }
}

void draw_plotPerlinVectors(cairo_t* cr) {
    // large grid
    for(int i = 0; i < NUMVECS_X; i++) { // draw random vector field
        for(int j = 0; j < NUMVECS_Y; j++) {
            // draw lines
            cairo_move_to(cr,
                          (i * perlin_vector_x_increment) - HALFSPACE,
                          (j * perlin_vector_y_increment) - HALFSPACE);

            cairo_rel_line_to(cr, 
                              randomField[i][j].x / TICKMULT,
                              randomField[i][j].y / TICKMULT);

            cairo_set_source_rgba(cr, 0, 1, 0, 0.7); // green semitrasnparent
            cairo_stroke(cr);
            
            // draw dots at origins of lines
            cairo_move_to(cr,
                          (i * perlin_vector_x_increment) - HALFSPACE,
                          (j * perlin_vector_y_increment) - HALFSPACE);

            cairo_set_source_rgba(cr, 1, 1, 1, 1); // white
            cairo_rel_line_to(cr, -0.01, -0.01);

            cairo_stroke(cr);
        }
    }
}

void draw_plotPaths(cairo_t* cr, vector paths[NUMLINES][NUMSTEPS + 1]) {
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
            if(vector_distance(paths[i][n], paths[i][n-1]) > 5) { // if there's some massive jump...
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
}

void draw_fillBackground(cairo_t* cr, cairo_surface_t* surface) {
    cairo_translate(cr, WID/2.0, HEI/2.0);
    cairo_scale(cr, WID/SPACE, -HEI/SPACE); // want to translate -1.0 <-> 1.0 to 0 <-> 500

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // fill background
    cairo_rectangle(cr, -SPACE/2, -SPACE/2, SPACE, SPACE);
    cairo_fill(cr);
}

void draw_indicateBounces(cairo_t* cr,
                          vector paths[NUMLINES][NUMSTEPS + 1],
                          int bounceTracker[NUMLINES][NUMSTEPS + 1]) {
    
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