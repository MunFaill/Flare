#include "Math/Time.h"

#include <chrono>

typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;

static TimePoint Last;

TimePoint Now() {
    return std::chrono::steady_clock::now();
}

void Time::Init() {
    Last = Now();
}

void Time::Update() {
    TimePoint Current = Now();

    std::chrono::duration<float> Elapsed = Current - Last;

    Delta = Elapsed.count();

    Last = Current;
}