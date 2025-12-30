#pragma once
#include "pose.hpp"
#include <cmath>

// Type of vector initialization
enum class E_Vector_Types {
    CARTESIAN,
    POLAR
};


class Vector {

    double x;
    double y;
    double magnitude;
    double angle;

public:
    // Constructors
    Vector(double param_1, double param_2, E_Vector_Types type = E_Vector_Types::CARTESIAN);
    Vector(const Pose& position);

    // Setters: Cartesian and Polar
    void setCartesian(double x, double y);
    void setPolar(double magnitude, double direction);

    // Rotate the vector by an angle (radians)
    void rotate(double rotation_angle);

    // Getters
    double getX() const;
    double getY() const;
    double getMagnitude() const;
    double getAngle() const;

    // Arithmetic operators
    Vector operator+(const Vector& rhs) const;
    Vector operator-(const Vector& rhs) const;
    Vector operator*(const double& value) const;
};