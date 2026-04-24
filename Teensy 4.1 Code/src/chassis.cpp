#include "run.hpp"
#include "libraries/pid.hpp"
#include "localization.hpp"
#include "libraries/util.hpp"
#include "run.hpp"
#include "libraries/pose.hpp"


void Wait::update(){
    if (!setup){
        setup = true;
        start_time = millis();
    }

    Serial.printf("Waiting\n");
}
bool Wait::isFinished(){ return (millis() - start_time) > delay_ms;}


void TurnAngle::update(){
    if (!setup){
        setup = true;
        motion_timer.reset();
        angular_pid.reset();
    }

    if (!finished){
        double power = angular_pid.compute(radToDeg(positioning.getPose().a), angle);

        left_wheel.move(power);
        right_wheel.move(-power);
    }

    if (motion_timer.getTime() > 1000){
        left_wheel.move(0);
        right_wheel.move(0);
        finished = true;
    }
}
bool TurnAngle::isFinished(){return finished;}


void MoveLine::update(){
    if (!setup){
        setup = true;
        motion_timer.reset();
        angular_pid.reset();
        motion_pid.reset();

        if (start.x == -1.0 && start.y == -1.0){
            start = positioning.getPose();
        }

        Vector line_error = end - positioning.getPose();  // Displacement from robot's position to target
        line_angle = M_PI_2 - line_error.getAngle();
        
    }


    Pose cur_pos = positioning.getPose();

    int dir = params.dir;
    Vector line = end - start;
    // Vector
    Vector error = cur_pos - start;

    // Dot products
    double dotABAB = line.getX() * line.getX() + line.getY() * line.getY();
    double dotAPAB = error.getX() * line.getX() + error.getY() * line.getY();

    // Projection scalar
    float t = dotAPAB / dotABAB;

    Pose closest_point = Pose(start.x + t * line.getX(), start.y + t * line.getY());

    Pose target =  Pose((line * (1 / line.getMagnitude()) * look_ahead));

    target = target + closest_point;



    
    Vector line_error = target - cur_pos;

    double error_a = nearAngle((M_PI_2 - line_error.getAngle()), cur_pos.a + (dir == -1 ? M_PI : 0.0));
    
    line_error = end - cur_pos;
    line_error.rotate(line_angle);

    double linear_error = Vector(end - closest_point).getMagnitude();
    double power_y = motion_pid.compute(-dir*linear_error, 0.0) * sgn(line_error.getY());
    if(fabs(power_y) > params.max_speed) power_y = sgn(power_y) * params.max_speed;
    if (fabs(power_y) < params.min_speed) power_y = sgn(power_y) * params.min_speed;


    // Only corrects if necessary (if robot won't land within an acceptable distance from the target)
    double correction = angular_pid.compute(-error_a, 0.0) * dir;
    
    double power_l, power_r;
    switch(sgn(correction)){
    case 0:
        power_l = power_y, power_r = power_y;
        break;
    case 1:
        power_l = power_y, power_r = power_y * exp(-correction);
        break;
    case -1:
        power_l = power_y * exp(correction), power_r = power_y;
        break;
    }


    double max = std::max(fabs(power_l), fabs(power_r));
    if (max > 100){
        double scale = 100 / max;
        power_l *= scale;
        power_r *= scale;
    }

    Serial.printf("MoveLine: %.1f, %.1f | %.1f, %.1f\n", power_l, power_r, start.x, start.y);

    left_wheel.move(power_l);
    right_wheel.move(power_r);

    
    if (linear_error < 0.25){
        left_wheel.move(0);
        right_wheel.move(0);
        finished = true;
    }
}
bool MoveLine::isFinished(){return finished;}

