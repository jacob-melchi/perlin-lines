//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved         
//-    For use by owners of Test-Driven Development for Embedded C,   
//-    and attendees of Renaissance Software Consulting, Co. training 
//-    classes.                                                       
//-                                                                   
//-    Available at http://pragprog.com/titles/jgade/                 
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3               
//-                                                                   
//-    Authorized users may use this source code in your own          
//-    projects, however the source code may not be used to           
//-    create training material, courses, books, articles, and        
//-    the like. We make no guarantees that this source code is       
//-    fit for any purpose.                                           
//-                                                                   
//-    www.renaissancesoftware.net james@renaissancesoftware.net      
//- ------------------------------------------------------------------


#include "CppUTest/TestHarness.h"

extern "C"
{
#include "vector.h"
#include "settings.h"
#include "perlin.h"
}

#define DOUBLE_TOLERANCE .00001

TEST_GROUP(Subtick) {

    void setup() {
        for (int i = 0; i < NUMTICKS; i++) {
            for (int j = 0; j < NUMTICKS; j++) {
                subticks[i][j] = (vector){i, j};
            }
        }
    }

    void teardown() {
    }

    double sum_x_components(int index) {
        return subticks[index  ][index  ].x +\
               subticks[index+1][index  ].x +\
               subticks[index  ][index+1].x +\
               subticks[index+1][index+1].x;
    }

    double sum_y_components(int index) {
        return subticks[index  ][index  ].y +\
               subticks[index+1][index  ].y +\
               subticks[index  ][index+1].y +\
               subticks[index+1][index+1].y;
    }

};

TEST(Subtick, SetSubtick) {
    vector test = {64, 64};

    perlin_setSubtick(4, 4, &test);

    LONGS_EQUAL(subticks[4][4].x, 64);
    LONGS_EQUAL(subticks[4][4].y, 64);
};

TEST(Subtick, CannotSetSubtickOutOfBounds) {
    vector test = {64, 64};

    perlin_setSubtick(NUMTICKS + 1, NUMTICKS + 1, &test);

    perlin_setSubtick(-1, -1, &test);
};

TEST(Subtick, GetSubtickFromIndices) {
    vector test = {0, 0};

    perlin_getSubtick(3, 3, &test);

    LONGS_EQUAL(test.x, 3);
    LONGS_EQUAL(test.y, 3);
};

TEST(Subtick, CannotGetOutOfRangeSubtick) {
    vector test = {0, 0};

    perlin_getSubtick(NUMTICKS + 1, NUMTICKS + 1, &test);

    LONGS_EQUAL(test.x, 0);
    LONGS_EQUAL(test.y, 0);

    perlin_getSubtick(-1, -1, &test);

    LONGS_EQUAL(test.x, 0);
    LONGS_EQUAL(test.y, 0);
};

TEST(Subtick, InterpolateSubticks) {
    // pass point, get ... ?
    vector test   = {0, 0};            // origin
    vector output = {0, 0};
    int    index  = (int)(NUMTICKS/2.0); // index at the origin(?)

    output = perlin_interpSubticks(test);

    DOUBLES_EQUAL(sum_x_components(index)/4.0, output.x, DOUBLE_TOLERANCE);
    DOUBLES_EQUAL(sum_y_components(index)/4.0, output.y, DOUBLE_TOLERANCE);
};

TEST(Subtick, PointsAlongBoundaryGetValidCoordinates) {
    vector test   = {-(SPACE/2), -(SPACE/2)}; // top left corner
    vector output = {0, 0};
    int index;

    index = 0;
    output = perlin_interpSubticks(test);

    DOUBLES_EQUAL(sum_x_components(index)/4.0, output.x, DOUBLE_TOLERANCE);
    DOUBLES_EQUAL(sum_y_components(index)/4.0, output.y, DOUBLE_TOLERANCE);


    index = 749;
    test = {(SPACE/2), (SPACE/2)};
    output = perlin_interpSubticks(test);

    DOUBLES_EQUAL(sum_x_components(index)/4.0, output.x, DOUBLE_TOLERANCE);
    DOUBLES_EQUAL(sum_y_components(index)/4.0, output.y, DOUBLE_TOLERANCE);
};

TEST(Subtick, NothingReturnedIfInputOOB) {
    
};