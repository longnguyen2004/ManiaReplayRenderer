#ifndef RENDERERLIB_CLOCK_HPP
#define RENDERERLIB_CLOCK_HPP

#include "Export.hpp"
#include <cstdint>
#include <chrono>

class RENDERERLIB_EXPORT Clock
{
public:
    Clock(double frameRate = 60.0, std::int64_t offset = 0);
    ~Clock() = default;

    void tick(bool sleepOnTick = false);
    std::int64_t getCurrentTime() const;
    std::int64_t getElapsedTime() const;
    std::size_t getFrameCount() const;
    bool nextFrame() const;
    void reset();
    void setEpoch();

private:
    double _frameRate;
    std::int64_t _offset;
    std::int64_t _elapsedTime;
    double _nextFrameTime;
    bool _nextFrame;
    std::size_t _frameCount;
    std::chrono::time_point<std::chrono::steady_clock> _wakeTime;
};

#endif
