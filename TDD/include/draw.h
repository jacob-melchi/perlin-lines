#ifndef DRAW_H
#define DRAW_H

#include "cairo.h"
#include "settings.h"
#include "vector.h"
#include "perlin.h"

void draw_plotSubTicks(cairo_t* cr);
void draw_plotPerlinVectors(cairo_t* cr);
void draw_fillBackground(cairo_t* cr, cairo_surface_t* surface);

#endif /* DRAW_H */