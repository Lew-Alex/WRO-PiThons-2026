#pragma once
#include "config.hpp"
#include "../libraries/pose.hpp"
#include "../libraries/math.hpp"
#include <Arduino.h>

class Odometry {
private:
    Pose pos;

    static constexpr double DIST_PARA = 0.0;
    static constexpr double DIST_PERP = 0.0;

public:

    void start(unsigned long interval_us = 5000); // default 5ms

    void update();

    Pose getPose();
};

// Single global object
extern Odometry odom;