#include "draw.h"

void draw_plotSubTicks(cairo_t* cr) {
    // increment in defined space for x/y
    double tick = SPACE/(1.0 * NUMTICKS);
    vector point;
    vector perlinVec; 

    for(int i = 0; i < NUMTICKS; i++) {
        for(int j = 0; j < NUMTICKS; j++) {
                point.x = i/((1.0 * NUMTICKS)/SPACE) - SPACE/2.0; // get even-spaced x and y coordinates
                point.y = j/((1.0 * NUMTICKS)/SPACE) - SPACE/2.0;

                #if GREYSCALE
                cairo_set_source_rgba(cr, noise, noise, noise, 1);
                cairo_move_to(cr, point.x, point.y);
                cairo_rectangle(cr, point.x, point.y, tick, tick);
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
                        i/(NUMVECS_X / SPACE) - SPACE/2,
                        j/(NUMVECS_Y / SPACE) - SPACE/2);
            cairo_rel_line_to(cr, 
                            randomField[i][j].x/TICKMULT,
                            randomField[i][j].y/TICKMULT);
            cairo_set_source_rgba(cr, 0, 1, 0, 0.7); // green semitrasnparent
            cairo_stroke(cr);
            
            // draw dots at origins of lines
            cairo_move_to(cr,
                        i/(NUMVECS_X / SPACE) - SPACE/2,
                        j/(NUMVECS_Y / SPACE) - SPACE/2);
            cairo_set_source_rgba(cr, 1, 1, 1, 1); // white
            cairo_rel_line_to(cr, -0.01, -0.01);
            cairo_stroke(cr);
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