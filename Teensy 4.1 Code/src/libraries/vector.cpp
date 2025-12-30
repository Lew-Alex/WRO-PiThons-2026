#include "vector.hpp"

// -------- Constructors -------- //

Vector::Vector(double param_1, double param_2, E_Vector_Types type) {
    if (type == E_Vector_Types::CARTESIAN) {
        setCartesian(param_1, param_2);
    } else {
        setPolar(param_1, param_2);
    }
}

Vector::Vector(const Pose& position) {
    setCartesian(position.x, position.y);
}

// -------- Setters -------- //

void Vector::setCartesian(double new_x, double new_y) {
    x = new_x;
    y = new_y;
    magnitude = std::sqrt(x * x + y * y);
    angle = std::atan2(y, x);
}

void Vector::setPolar(double new_mag, double new_angle) {
    magnitude = new_mag;
    angle = new_angle;
    x = magnitude * std::cos(angle);
    y = magnitude * std::sin(angle);
}

// -------- Rotation -------- //

void Vector::rotate(double rotation_angle) {
    angle += rotation_angle;
    x = magnitude * std::cos(angle);
    y = magnitude * std::sin(angle);
}

// -------- Getters -------- //

double Vector::getX() const { return x; }
double Vector::getY() const { return y; }
double Vector::getMagnitude() const { return magnitude; }
double Vector::getAngle() const { return angle; }

// -------- Operators -------- //

Vector Vector::operator+(const Vector& rhs) const {
    return Vector(x + rhs.x, y + rhs.y);
}

Vector Vector::operator-(const Vector& rhs) const {
    return Vector(x - rhs.x, y - rhs.y);
}

Vector Vector::operator*(const double& value) const {
    return Vector(x * value, y * value);
}