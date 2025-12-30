#include "pose.hpp"
#include "vector.hpp"

// Constructors
Pose::Pose() : x(0), y(0), a(0) {}

Pose::Pose(double x, double y, double a)
    : x(x), y(y), a(a) {}

Pose::Pose(Vector v, double a)
    : x(v.getX()), y(v.getY()), a(a) {}


// Operators
Pose Pose::operator+(const Pose& rhs) const {
    return Pose(x + rhs.x, y + rhs.y, a + rhs.a);
}

Pose Pose::operator-(const Pose& rhs) const {
    return Pose(x - rhs.x, y - rhs.y, a - rhs.a);
}

Pose Pose::operator*(double scalar) const {
    return Pose(x * scalar, y * scalar, a * scalar);
}

Pose Pose::operator/(double scalar) const {
    return Pose(x / scalar, y / scalar, a / scalar);
}




// Constructors
MotionPose::MotionPose()
    : x(0), y(0), a(0),
      vx(0), vy(0), va(0) {}

MotionPose::MotionPose(double x, double y, double a,
                       double vx, double vy, double va)
    : x(x), y(y), a(a),
      vx(vx), vy(vy), va(va) {}

// Operators
MotionPose MotionPose::operator+(const MotionPose& rhs) const {
    return MotionPose(
        x + rhs.x,  y + rhs.y,  a + rhs.a,
        vx + rhs.vx, vy + rhs.vy, va + rhs.va
    );
}

MotionPose MotionPose::operator-(const MotionPose& rhs) const {
    return MotionPose(
        x - rhs.x,  y - rhs.y,  a - rhs.a,
        vx - rhs.vx, vy - rhs.vy, va - rhs.va
    );
}

MotionPose MotionPose::operator*(double scalar) const {
    return MotionPose(
        x * scalar,  y * scalar,  a * scalar,
        vx * scalar, vy * scalar, va * scalar
    );
}

MotionPose MotionPose::operator/(double scalar) const {
    return MotionPose(
        x / scalar,  y / scalar,  a / scalar,
        vx / scalar, vy / scalar, va / scalar
    );
}