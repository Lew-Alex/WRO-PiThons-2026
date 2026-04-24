#include <Arduino.h>
#include "config.hpp"
#include "run.hpp"
#include "libraries/util.hpp"

// Step Stuff
void Step::update(){
    std::visit([](auto& s) {
        s.update();
    }, state);
}

bool Step::isFinished() {
    return std::visit([](auto& s) {
        return s.isFinished();
    }, state);
}




// **************************** Autonomous Run ****************************
Step routine[] = {
    // Step{Wait{1000}},
    // Step{MoveLine{Pose(0, 10), Pose(20, 20), 20}},
};

// **************************** End Run ****************************

int routineLength = sizeof(routine) / sizeof(routine[0]);
int STATE_STEP = 0;


void autoCycle() {
    if (STATE_STEP < routineLength){
        Step& current = routine[STATE_STEP];

        current.update();

        if (current.isFinished()){
            STATE_STEP++;
        }
    }
}