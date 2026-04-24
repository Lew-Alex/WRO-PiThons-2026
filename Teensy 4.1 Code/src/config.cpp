#include "config.hpp"


Drive left_wheel(4, 5, 20, 21);
Drive right_wheel(6, 7, 20, 21);

TrackingWheel parallelWheel(26, 27, 4096.0, 38.0);
TrackingWheel perpWheel(0, 1, 4096.0, 38.0);
IMU imu;