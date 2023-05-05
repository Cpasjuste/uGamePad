//
// Created by cpasjuste on 18/04/23.
//

#ifdef NATIVE
#include <ctime>
#include <sys/time.h>
#else
#include <api/ArduinoAPI.h>
#endif
#include "clock.h"

uGamePad::Clock::Clock() {
    m_startTime = Clock::getCurrentTime();
}

uGamePad::Time uGamePad::Clock::getCurrentTime() const {
#ifdef NATIVE
    timespec time = {};
    clock_gettime(CLOCK_MONOTONIC, &time);
    return microseconds((long) time.tv_sec * 1000000 + time.tv_nsec / 1000);
#else
    return microseconds((long) micros());
#endif
}

uGamePad::Time uGamePad::Clock::getElapsedTime() const {
    return getCurrentTime() - m_startTime;
}

uGamePad::Time uGamePad::Clock::restart() {
    Time now = getCurrentTime();
    Time elapsed = now - m_startTime;
    m_startTime = now;
    return elapsed;
}
