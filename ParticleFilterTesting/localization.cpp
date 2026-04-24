#include "localization.hpp"
#include <cmath>
#include <thread>
#include <mutex>

Localization positioning;
Map floorMap;
std::mutex poseMutex;


double Map::getBrightness(Vector pos){
    const int MAP_WIDTH = 472;  
    const int MAP_HEIGHT = 228;
    const double FIELD_WIDTH_MM = 2362.0;
    const double FIELD_HEIGHT_MM = 1143.0;

    int x_idx = static_cast<int>(pos.getX() / FIELD_WIDTH_MM * MAP_WIDTH);
    int y_idx = static_cast<int>((FIELD_HEIGHT_MM - pos.getY()) / FIELD_HEIGHT_MM * MAP_HEIGHT);

    if(x_idx < 0) x_idx = 0;
    if(x_idx >= MAP_WIDTH) x_idx = MAP_WIDTH - 1;
    if(y_idx < 0) y_idx = 0;
    if(y_idx >= MAP_HEIGHT) y_idx = MAP_HEIGHT - 1;

    return static_cast<double>(map[y_idx][x_idx]);
}

void Localization::simulate_pos(int count){
    // Assume perfect arcs for now(so perfect odom)
    PAR_MM_SIMULATE = 10.0;
    if (count < 30) IMU_SIMULATE = 0.05;
    else if (count < 180) IMU_SIMULATE = 0.0;
    else IMU_SIMULATE = -0.05;

    

    double local_y;
    if (fabs(IMU_SIMULATE) < 1e-6) {
        local_y = PAR_MM_SIMULATE;
    } else {
        double beta = IMU_SIMULATE / 2.0;
        double sinBeta = sin(beta);

        double radiusR = PAR_MM_SIMULATE / IMU_SIMULATE;
        local_y = radiusR * 2.0 * sinBeta;
    }

    double alpha = real_pose.a + IMU_SIMULATE / 2.0;
    double cos_alpha = cos(alpha);
    double sin_alpha = sin(alpha);

    real_pose.x += local_y * sin_alpha;
    real_pose.y += local_y * cos_alpha;
    real_pose.a += IMU_SIMULATE;

    // Sensor errors
    PAR_MM_SIMULATE += 0.5;
    IMU_SIMULATE += 0.000174533;

    Vector left_sensor = color_l_pos;
    
    left_sensor.rotate(-real_pose.a);
    left_sensor = left_sensor + real_pose;

    Vector right_sensor = color_r_pos;
    right_sensor.rotate(-real_pose.a);
    right_sensor = right_sensor + real_pose;

    LEFT_SENSOR_SIMULATE = floorMap.getBrightness(left_sensor);
    RIGHT_SENSOR_SIMULAR = floorMap.getBrightness(right_sensor);

    // printf("LEFT SENSOR: %.1f, %.1f\n", left_sensor.getX(), left_sensor.getY());


    printf("REAL | X: %.3f Y: %.3f A:%.3f\n", real_pose.x, real_pose.y, radToDeg(real_pose.a));
}


void run_localization(){
    int count = 0;
    positioning.reset({50, 50, 0.0});
    while (count < 250){
        count += 1;

        positioning.simulate_pos(count);

        // First update particles with odom + randomness
        positioning.update_particles();
        // Set particle weights based on color sensors
        positioning.set_particle_weights();
        // Resample particles on count
        positioning.resample_particles();

    
        // delay(1); // The functions themselves take up time
    }

    printf("MAX ERROR | %.1f\n", positioning.max_error);
}

void Localization::start(unsigned long interval_us) {
    // imu.init();
    // imu.calibrateGyro();
    // parallelWheel.reset();
    // perpWheel.reset();

    // Create Particles
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i].x = 50.0;
        particles[i].y = 50.0;
        particles[i].a = 0.0;
        weights[i] = 1 / N_PARTICLES;
    }
    
    // 
    // threads.addThread(run_localization);
}

void Localization::reset(Pose pos){
    // Reset all particles
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i] = pos;
        weights[i] = 1 / N_PARTICLES;
    }
    real_pose = pos;
}

void Localization::update_particles() {
    // Updates Wheels + IMU since last time
    // parallelWheel.update();
    // imu.update();
    

    // Read current encoder distances in mm
    // double par_mm  = -parallelWheel.getDeltaDistanceMM() / 25.4;
    double par_mm = PAR_MM_SIMULATE;


    // Read current heading in degrees
    // double heading_rad = imu.getDeltaHeadingRad();
    double heading_rad = IMU_SIMULATE;

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

    // Update all particles from odom with noise
    printf("PARTICLES | ");
    for (int i = 0; i < N_PARTICLES; i++) {
        double noise_x = linear_random(gen);
        double noise_y = linear_random(gen);
        double noise_a = angular_random(gen);

        particles[i].x += delta_pos.x + noise_x;
        particles[i].y += delta_pos.y + noise_y;
        particles[i].a += delta_pos.a + noise_a;
        printf("X: %.1f Y: %.1f W: %.3f | ", particles[i].x, particles[i].y, weights[i]);
    }
    printf("\n");
}


void Localization::set_particle_weights(){
    for (int i = 0; i < N_PARTICLES; i++) {
        Pose particle_pose = particles[i];
        // Get position of sensor on the field
        Vector left_sensor = color_l_pos;
        left_sensor.rotate(-particle_pose.a);
        left_sensor = left_sensor + particle_pose;

        Vector right_sensor = color_r_pos;
        right_sensor.rotate(-particle_pose.a);
        right_sensor = right_sensor + particle_pose;


        double expected_light_l = floorMap.getBrightness(left_sensor);
        double expected_light_r = floorMap.getBrightness(right_sensor);
        
        // Get actual light
        double actual_light_l = LEFT_SENSOR_SIMULATE;
        double actual_light_r = RIGHT_SENSOR_SIMULAR;

        // Calculate weight
        double error_l = actual_light_l - expected_light_l;
        double error_r = actual_light_r - expected_light_r;

        auto gaussian = [this](double error) {
            return exp(- (error * error) / (2.0 * sigma * sigma));
        };

        // printf("Actual: %.1f, %.1f | Expected: %.1f, %.1f Light | %.1f, %.1f\n", actual_light_l, actual_light_r, expected_light_l, expected_light_r, left_sensor.getX(), left_sensor.getY());
        weights[i] = gaussian(error_l) * gaussian(error_r);
        // weights[i] = 1;
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
    printf("ESTIMATE | X: %.3f Y: %.3f A:%.3f\n", pos.x, pos.y, radToDeg(pos.a));
    poseMutex.unlock();


    // Resample particles
    std::vector<int> indices(N_PARTICLES);
    for (int i = 0; i < N_PARTICLES; i++) indices[i] = i;

    std::sort(indices.begin(), indices.end(),
              [this](int a, int b) { return weights[a] < weights[b]; });

    // Replace bottom 20% particles with new ones around the average position
    for (int i = 0; i < PARTICLES_RESAMPLED; i++) {
        int idx = indices[i];
        
        double noise_x = resample_linear_random(gen);
        double noise_y = resample_linear_random(gen);
        double noise_theta = resample_angular_random(gen);


        particles[idx] = Pose(pos.x + noise_x, pos.y + noise_y, pos.a + noise_theta);
        weights[idx] = 1.0 / N_PARTICLES;
    }


    if (Vector(pos - real_pose).getMagnitude() > max_error){
        max_error = Vector(pos - real_pose).getMagnitude();
    }
}

Pose Localization::getPose() {
    poseMutex.lock();
    Pose copy = pos;
    poseMutex.unlock();
    return copy;
}

