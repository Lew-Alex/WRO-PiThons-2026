#pragma once
#include <cmath>

double radToDeg(double rad);
double degToRad(double deg);
double nearAngle(double angle, double reference);

template <typename T>
int sgn(T value) {
    return (T(0) < value) - (value < T(0));
}

inline double _deg(double deg) { return deg * M_PI / 180.0; }
inline double _rad(double rad) { return rad; }
inline double _inch(double in) { return in * 25.4; }
inline double _mm(double mm)   { return mm; }