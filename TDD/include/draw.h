#ifndef DRAW_H
#define DRAW_H

#include "cairo.h"
#include "settings.h"
#include "vector.h"
#include "perlin.h"

void drawSubTicks(cairo_t* cr);
void drawPerlinVectors(cairo_t* cr);
void fillBackground(cairo_t* cr, cairo_surface_t* surface);

#endif /* DRAW_H */