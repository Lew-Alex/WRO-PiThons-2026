#pragma once
#include "config.hpp"
#include "libraries/pose.hpp"
#include "libraries/math.hpp"
#include "libraries/vector.hpp"
#include <random>
#include <Arduino.h>

void localizationCycle();

class Map{
public:
    double getBrightness(Vector pos);
};

#define N_PARTICLES 1
#define PARTICLES_RESAMPLED 0 // # of particles resampeld
#define RESAMPLING_RATE 1

class Localization {
private:
    // For particles
    Pose particles[N_PARTICLES] = {};
    double weights[N_PARTICLES] = {};

    std::default_random_engine gen;
    std::normal_distribution<double> linear_random{1.0, 0.0}; // Mean is 1, std Dev is 0.01
    std::normal_distribution<double> angular_random{1.0, 0.0}; // Mean is 1, std Dev is 0.01


    // For Odom
    static constexpr double DIST_PARA = 0.0;
    Pose delta_pos;

    // For color sensor
    Vector color_l_pos{50, 50};
    Vector color_r_pos{-50, 50};

    double sigma = 10.0; // Used for error in color sensors
    

private:
    Pose pos;

    void update_particles(); // Uses odom to move particles

    void set_particle_weights(); // Assigns weights based on light sensors

    void resample_particles(); // Resamples Particles

public:

    // Setup
    void setup(unsigned long interval_us = 5000); // default 5ms

    void localizationCycle();


    void reset(Pose pos);
    Pose getPose();
};

// Single global object
extern Localization positioning;