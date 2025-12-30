#include "math.hpp"
#include <cmath>

double radToDeg(double rad) {
    return rad * 180.0 / M_PI;
}

double degToRad(double deg) {
    return deg * M_PI / 180.0;
}

double nearAngle(double angle, double reference){
	return round((reference - angle)/(2*M_PI)) * (2*M_PI) + angle - reference;
}