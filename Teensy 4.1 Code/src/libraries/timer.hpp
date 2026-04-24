#pragma once
#include <Arduino.h>

enum class TimingUnit {
    MICROS,
    MILLIS
};

class Timer {
public:
    Timer(const char* name, bool start = true, TimingUnit unit = TimingUnit::MILLIS);

    void reset(bool start = true);
    void play();
    void pause();

    uint64_t getTime();
    uint64_t getLastResetTime();

    bool isPlaying();

    void print(const char* msg = "");

private:
    const char* name;

    TimingUnit unit;

    uint64_t timeAccum;        // accumulated time
    uint64_t lastPlayTime;     // last time play() was called
    uint64_t lastResetTime;

    bool paused;

    uint64_t now();            // internal time getter
};