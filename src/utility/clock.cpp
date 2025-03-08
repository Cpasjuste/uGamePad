//
// Created by cpasjuste on 18/04/23.
//

#ifdef LINUX
#include <ctime>
#include <sys/time.h>
#else
#include <pico/time.h>
#endif

#include "clock.h"

uGamePad::Clock::Clock() {
    m_startTime = getCurrentTime();
}

uGamePad::Time uGamePad::Clock::getCurrentTime() const {
#ifdef LINUX
    timespec time = {};
    clock_gettime(CLOCK_MONOTONIC, &time);
    return microseconds((long) time.tv_sec * 1000000 + time.tv_nsec / 1000);
#else
    return microseconds((long) to_us_since_boot(get_absolute_time()));
#endif
}

uGamePad::Time uGamePad::Clock::getElapsedTime() const {
    return getCurrentTime() - m_startTime;
}

uGamePad::Time uGamePad::Clock::restart() {
    const Time now = getCurrentTime();
    const Time elapsed = now - m_startTime;
    m_startTime = now;
    return elapsed;
}
