#include "timer.hpp"

// Constructor
Timer::Timer(const char* name, bool start, TimingUnit unit)
    : name(name), unit(unit), timeAccum(0), lastPlayTime(0), lastResetTime(0), paused(true)
{
    reset(start);
}

// Reset timer
void Timer::reset(bool start) {
    timeAccum = 0;
    lastResetTime = now();

    if (start) {
        paused = true;
        play();
    } else {
        paused = true;
    }
}

// Get current time
uint64_t Timer::getTime() {
    if (paused) return timeAccum;
    return timeAccum + (now() - lastPlayTime);
}

// Get last reset timestamp
uint64_t Timer::getLastResetTime() {
    return lastResetTime;
}

// Play / Resume
void Timer::play() {
    if (paused) {
        lastPlayTime = now();
        paused = false;
    } else {
        Serial.printf("Timer \"%s\" already playing\n", name);
    }
}

// Pause
void Timer::pause() {
    if (!paused) {
        timeAccum += (now() - lastPlayTime);
        paused = true;
    } else {
        Serial.printf("Timer \"%s\" already paused\n", name);
    }
}

// Check if running
bool Timer::isPlaying() {
    return !paused;
}

// Print
void Timer::print(const char* msg) {
    Serial.printf("%s time: %llu | %s\n", name, getTime(), msg);
}

// Internal time function
uint64_t Timer::now() {
    if (unit == TimingUnit::MICROS) {
        return micros();
    } else {
        return millis();
    }
}