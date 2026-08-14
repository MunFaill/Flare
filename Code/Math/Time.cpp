#include "Math/Time.h"

#include <chrono>

typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;

static TimePoint LastTime;

void Time::InitTime() {
    LastTime = std::chrono::steady_clock::now();
}

void Time::CalculateTime() {
    TimePoint CurrentTime = std::chrono::steady_clock::now();

    std::chrono::duration<float> elapsed = CurrentTime - LastTime;

    DeltaTime = elapsed.count();

    LastTime = CurrentTime;
}