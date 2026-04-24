#pragma once
#include <variant>
#include <cstdint>
#include "libraries/pid.hpp"
#include "libraries/pose.hpp"
#include "localization.hpp"

struct MotionParams{
    double max_speed = 100.0;
    double min_speed = 0.0;
    
    int dir = 1;
};

// Autonomous States
struct Wait{
private:
    uint32_t start_time;

public:
    uint32_t delay_ms;

    Wait(uint32_t ms): delay_ms(ms) {}

    void update();
    bool isFinished();
    bool setup = false;


};
struct TurnAngle{
    double angle;
    MotionParams params;


    TurnAngle(double angle, MotionParams params = {}): angle(angle), params(params), angular_pid(1.75, 0.0, 0.04), motion_timer("motion_timer") {}

    void update(); 
    bool isFinished();
    bool setup = false;
    bool finished = false;

private:
    PID angular_pid;
    Timer motion_timer;
};

struct MoveLine{
    Pose start;
    Pose end;
    double look_ahead;
    MotionParams params;

    MoveLine(Pose start, Pose end, double look_ahead, MotionParams params = {}): start(start), end(end), look_ahead(look_ahead), params(params), motion_pid(12.0, 0.0, 0.0), angular_pid(2.5, 0.0, 0.04), motion_timer("motion_timer") {}
    MoveLine(Pose end, double look_ahead, MotionParams params = {}): start(Pose(-1.0, -1.0)), end(end), look_ahead(look_ahead), params(params), motion_pid(12.0, 0.0, 0.0), angular_pid(2.5, 0.0, 0.04), motion_timer("motion_timer") {}

    void update(); 
    bool isFinished();
    bool setup = false;
    bool finished = false;

private:
    PID motion_pid;
    PID angular_pid;
    Timer motion_timer;

    double line_angle;
};


#define AUTO_STATES Wait, TurnAngle, MoveLine
#define AUTO_STATES_VARIANT std::variant<AUTO_STATES>




struct Step{
    AUTO_STATES_VARIANT state;

    void update();
    bool isFinished();
};


void autoCycle();