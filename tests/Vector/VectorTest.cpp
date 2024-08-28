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
}

#define DOUBLE_TOLERANCE .00001

TEST_GROUP(Vector) {
    vector v1, v2;

    void setup() {
    }

    void teardown() {
    }
};

TEST(Vector, VectorTypeExists) {
    v1 = {1, 1};
    v2 = {1, 0};

    LONGS_EQUAL(v1.x, 1);
    LONGS_EQUAL(v1.y, 1);
};

TEST(Vector, MagnitudeExists) {
    v1 = {1, 1};

    DOUBLES_EQUAL(magnitude(v1), sqrt(2), DOUBLE_TOLERANCE);
};

TEST(Vector, MagnitudeIsOne) {
    v1 = {sqrt(2)/2, sqrt(2)/2};

    DOUBLES_EQUAL(magnitude(v1), 1, DOUBLE_TOLERANCE);
};

TEST(Vector, DotProduct) {
    v1 = {1, 1};
    v2 = {1, 0};

    LONGS_EQUAL(dotProduct(v1, v2), 1);
};

TEST(Vector, DotProductMinAngleReturnsZero) {
    // perpendicular lines
    v1 = {1, 0};
    v2 = {0, 1};

    LONGS_EQUAL(dotProduct(v1, v2), 0);
};

TEST(Vector, NormalizeReduces) {
    v1 = {2.0, 1.0};
    v2 = normalize(v1);

    DOUBLES_EQUAL(v2.x, (2/sqrt(5)), DOUBLE_TOLERANCE);
    DOUBLES_EQUAL(v2.y, (1/sqrt(5)), DOUBLE_TOLERANCE);
};

TEST(Vector, NormalizeExpands) {
    v1 = {0.5, 0.2};
    v2 = normalize(v1);

    DOUBLES_EQUAL(v2.x, 0.5/(sqrt(29.0/100)), DOUBLE_TOLERANCE);
    DOUBLES_EQUAL(v2.y, 0.2/(sqrt(29.0/100)), DOUBLE_TOLERANCE);
};

TEST(Vector, NormalizeDoesNotDivByZero) {
    v1 = {0, 0};
    v2 = normalize(v1);

    DOUBLES_EQUAL(v2.x, 0, DOUBLE_TOLERANCE);
    DOUBLES_EQUAL(v2.y, 0, DOUBLE_TOLERANCE);
};

TEST(Vector, GetAngleExists) {
    v1 = {1, 0};
    v2 = {0, 1};

    DOUBLES_EQUAL(getAngle(v1, v2), (M_PI/2.0), DOUBLE_TOLERANCE);
}

TEST(Vector, GetAngleMaxValue) {
    v1 = {1,  0};
    v2 = {-1, 0};

    DOUBLES_EQUAL(getAngle(v1, v2), M_PI, DOUBLE_TOLERANCE);
}

TEST(Vector, GetAngleMinValue) {
    v1 = {1, 0};
    v2 = {1, 0};

    DOUBLES_EQUAL(getAngle(v1, v2), 0, DOUBLE_TOLERANCE);
}