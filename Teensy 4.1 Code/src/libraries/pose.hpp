#pragma once

class Vector; // forward declare

struct Pose {
    double x;
    double y;
    double a;

    // Constructors
    Pose();
    Pose(double x, double y, double a = 0.0);
    Pose(Vector v, double a = 0.0);

    // Arithmetic operators
    Pose operator+(const Pose& rhs) const;
    Pose operator-(const Pose& rhs) const;
    Pose operator*(double scalar) const;
    Pose operator/(double scalar) const;
};

struct MotionPose {
    // Position
    double x;
    double y;
    double a;

    // Velocities
    double vx;
    double vy;
    double va;

    // Constructors
    MotionPose();
    MotionPose(double x, double y, double a,
               double vx = 0.0, double vy = 0.0, double va = 0.0);
    MotionPose(Vector xy, double a,
                Vector vxy, double va);

    MotionPose operator+(const MotionPose& rhs) const;
    MotionPose operator-(const MotionPose& rhs) const;
    MotionPose operator*(double scalar) const;
    MotionPose operator/(double scalar) const;
};