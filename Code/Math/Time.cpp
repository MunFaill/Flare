#include "Math/Time.h"

#include <chrono>

// Helpers

typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;

static TimePoint LastTime;

TimePoint Now() {
    return std::chrono::steady_clock::now();
}

// Code

void Time::InitTime() {
    LastTime = Now();
}

void Time::CalculateTime() {
    TimePoint CurrentTime = Now();

    std::chrono::duration<float> elapsed = CurrentTime - LastTime;

    DeltaTime = elapsed.count();

    LastTime = CurrentTime;
}