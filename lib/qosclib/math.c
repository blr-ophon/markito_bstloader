#include "math.h"

double n_pow(double x, double y){
    double rv = 1;
    for(int i = 0; i < y; i++){
        rv *= x;
    }
    return rv;
}

