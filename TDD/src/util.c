#include "util.h"

char* util_generateFileName(void) {
    char* resString = malloc(sizeof(char) * 30);
    char* dirString = malloc(sizeof(char) * 30);
    char* radString = malloc(sizeof(char) * 30);
    char* numString = malloc(sizeof(char) * 30);

    sprintf(resString, "res %0.4f", RESOLUTION);
    sprintf(numString, "%d lines ",  NUMLINES);

    if(DOGRAVITY)  { sprintf(radString, "radius %0.2f ", GRAVRADIUS); }
    else           { sprintf(radString, " "); }

    if(DIRECTIONS) { sprintf(dirString, "LR "); }
    else           { sprintf(dirString, "");    }
    
    char* A = malloc(sizeof(char) * 150);

    A = strcat(dirString, numString);
    A = strcat(A, resString);
    A = strcat(A, radString);
    A = strcat(A, ".png");

    free(resString);
    free(dirString);
    free(radString);
    free(numString);

    return A;
}