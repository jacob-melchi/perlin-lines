#ifndef DRAW_H
#define DRAW_H

#include "cairo.h"
#include "settings.h"
#include "vector.h"
#include "perlin.h"

void draw_plotSubTicks(cairo_t* cr);
void draw_plotPerlinVectors(cairo_t* cr);
void draw_fillBackground(cairo_t* cr, cairo_surface_t* surface);
void draw_plotPaths(cairo_t* cr, vector paths[NUMLINES][NUMSTEPS + 1]);

#endif /* DRAW_H */