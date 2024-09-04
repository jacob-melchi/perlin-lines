#include "draw.h"

void drawSubTicks(cairo_t* cr) {
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
                
                getSubtick(i, j, &perlinVec);

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