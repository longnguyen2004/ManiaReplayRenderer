#ifndef RENDERERLIB_CLOCK_HPP
#define RENDERERLIB_CLOCK_HPP

#include "Export.hpp"
#include <chrono>
#include <cstdint>

class RENDERERLIB_EXPORT Clock
{
public:
    Clock(double frameRate = 60.0, std::int64_t offset = 0);
    ~Clock() = default;

    void tick(bool sleepOnTick = false);
    std::int64_t getCurrentTime() const;
    std::int64_t getElapsedTime() const;
    bool nextFrame() const;
    void reset();
    void setEpoch();

private:
    double _frameRate;
    std::int64_t _offset;
    std::int64_t _elapsedTime;
    double _nextFrameTime;
    bool _nextFrame;
    std::chrono::steady_clock::time_point _wakeTime;
};

#endif
