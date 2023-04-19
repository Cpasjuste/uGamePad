//
// Created by cpasjuste on 18/04/23.
//

#include <ctime>
#include <sys/time.h>
#include "clock.h"

//extern "C" int clock_gettime(clockid_t cid, struct timespec *tp);

uGamePad::Clock::Clock() {
    m_startTime = Clock::getCurrentTime();
}

uGamePad::Time uGamePad::Clock::getCurrentTime() const {
    /*
    timespec time = {};
    clock_gettime(CLOCK_REALTIME, &time);
    return microseconds((long) time.tv_sec * 1000000 + time.tv_nsec / 1000);
    */
    struct timeval time{};
    gettimeofday(&time, nullptr);
    return microseconds((long) 1000000 * time.tv_sec + time.tv_usec);
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
