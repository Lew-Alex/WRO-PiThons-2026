#include "odom.hpp"
#include <cmath>
#include <TeensyThreads.h>

Odometry odom;
Threads::Mutex poseMutex;


void odom_run(){
    while (true){
        odom.update();

        delay(5);
    }
}

void Odometry::start(unsigned long interval_us) {
    imu.init();
    imu.calibrateGyro();
    parallelWheel.reset();
    perpWheel.reset();
    
    threads.addThread(odom_run);
}

void Odometry::update() {
    parallelWheel.update();
    perpWheel.update();
    imu.update();
    

    // Read current encoder distances in mm
    double par_mm  = -parallelWheel.getDeltaDistanceMM() / 25.4;
    double perp_mm = -perpWheel.getDeltaDistanceMM() / 25.4;


    // Read current heading in degrees
    double heading_rad = imu.getDeltaHeadingRad();

    double local_x, local_y;
    if (fabs(heading_rad) < 1e-6) {
        local_x = perp_mm;
        local_y = par_mm;
    } else {
        double beta = heading_rad / 2.0;
        double sinBeta = sin(beta);

        double radiusR = par_mm / heading_rad;
        local_y = radiusR * 2.0 * sinBeta;

        double radiusB = perp_mm / heading_rad;
        local_x = radiusB * 2.0 * sinBeta;
    }

    double alpha = pos.a + heading_rad / 2.0;
    double cos_alpha = cos(alpha);
    double sin_alpha = sin(alpha);


    poseMutex.lock();

    pos.x += local_x * cos_alpha + local_y * sin_alpha;
    pos.y += local_x * -sin_alpha + local_y * cos_alpha;
    pos.a += heading_rad;

    poseMutex.unlock();
}

Pose Odometry::getPose() {
    poseMutex.lock();
    Pose copy = pos;
    poseMutex.unlock();
    return copy;
}

