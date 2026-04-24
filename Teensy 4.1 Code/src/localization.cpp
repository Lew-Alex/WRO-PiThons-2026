#include "localization.hpp"
#include <cmath>
#include "libraries/util.hpp"

Localization positioning;
Map floorMap;
Threads::Mutex poseMutex;



double Map::getBrightness(Vector pos){
    return 255;
}


void Localization::localizationCycle(){
    update_particles();
    Serial.printf("%d | Tracking: %.2f, %.2f, %.3f\n", millis(), pos.x, pos.y, radToDeg(pos.a));
}

void Localization::setup(unsigned long interval_us) {
    imu.init();
    imu.calibrateGyro();
    parallelWheel.reset();
    perpWheel.reset();

    // Create Particles
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i].x = 0.0;
        particles[i].y = 0.0;
        particles[i].a = 0.0;
        weights[i] = 1 / N_PARTICLES;
    }

}

void Localization::reset(Pose pos){
    // Reset all particles
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i] = pos;
        weights[i] = 1 / N_PARTICLES;
    }
}

void Localization::update_particles() {
    // Updates Wheels + IMU since last time
    parallelWheel.update();
    imu.update();
    

    // Read current encoder distances in mm
    double par_mm  = parallelWheel.getDeltaDistanceMM() / 25.4;


    // Read current heading in degrees
    double heading_rad = -imu.getDeltaHeadingRad();

    double local_y;
    if (fabs(heading_rad) < 1e-6) {
        local_y = par_mm;
    } else {
        double beta = heading_rad / 2.0;
        double sinBeta = sin(beta);

        double radiusR = par_mm / heading_rad;
        local_y = radiusR * 2.0 * sinBeta;

    }

    double alpha = pos.a + heading_rad / 2.0;
    double cos_alpha = cos(alpha);
    double sin_alpha = sin(alpha);

    delta_pos.x = local_y * sin_alpha;
    delta_pos.y = local_y * cos_alpha;
    delta_pos.a = heading_rad;

    poseMutex.lock();
    pos = pos + delta_pos;
    poseMutex.unlock();

    // Update all particles from odom with noise
    // for (int i = 0; i < N_PARTICLES; i++) {
    //     double noise_x = linear_random(gen);
    //     double noise_y = linear_random(gen);
    //     double noise_a = angular_random(gen);

    //     particles[i].x += delta_pos.x * noise_x;
    //     particles[i].y += delta_pos.y * noise_y;
    //     particles[i].a += delta_pos.a * noise_a;
    // }
}


void Localization::set_particle_weights(){
    for (int i = 0; i < N_PARTICLES; i++) {
        Pose particle_pose = particles[i];
        // Get position of sensor on the field
        Vector left_sensor = color_l_pos + particle_pose;
        Vector right_sensor = color_r_pos + particle_pose;

        double expected_light_l = floorMap.getBrightness(left_sensor);
        double expected_light_r = floorMap.getBrightness(right_sensor);
        
        // Get actual light
        double actual_light_l = 0.0;
        double actual_light_r = 0.0;

        // Calculate weight
        double error_l = actual_light_l - expected_light_l;
        double error_r = actual_light_r - expected_light_r;

        auto gaussian = [this](double error) {
            return exp(- (error * error) / (2.0 * sigma * sigma));
        };

        weights[i] = gaussian(error_l) * gaussian(error_r);
    }
}

void Localization::resample_particles(){
    // Normalize weights and determine pos
    double weight_sum = 0.0;
    for (int i = 0; i < N_PARTICLES; i++) {
        weight_sum += weights[i];
    }
    poseMutex.lock();
    pos = Pose(0, 0, 0);
    for (int i = 0; i < N_PARTICLES; i++) {
        weights[i] /= weight_sum;
        pos = pos + particles[i] * weights[i]; // Add to position
    }
    poseMutex.unlock();

    // Resample particles
    std::vector<int> indices(N_PARTICLES);
    for (int i = 0; i < N_PARTICLES; i++) indices[i] = i;

    std::sort(indices.begin(), indices.end(),
              [this](int a, int b) { return weights[a] < weights[b]; });

    // Replace bottom 20% particles with new ones around the average position
    for (int i = 0; i < PARTICLES_RESAMPLED; i++) {
        int idx = indices[i];
        
        double noise_x = linear_random(gen);
        double noise_y = linear_random(gen);
        double noise_theta = angular_random(gen);

        particles[idx] = Pose(pos.x + noise_x, pos.y + noise_y, pos.a + noise_theta);
        weights[idx] = 1.0 / N_PARTICLES;
    }
}

Pose Localization::getPose() {
    poseMutex.lock();
    Pose copy = pos;
    poseMutex.unlock();
    return copy;
}

