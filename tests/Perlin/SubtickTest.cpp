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
            subticks[i][i] = (vector){i, i};
        }
    }

    void teardown() {
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

TEST(Subtick, GetSubtick) {
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