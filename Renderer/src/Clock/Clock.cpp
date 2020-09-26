#include "Clock.hpp"

#include <cmath>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

Clock::Clock(double frameRate, std::int64_t offset) :
    _frameRate(frameRate), _offset(offset)
{
    std::cout << "[Clock] Target FPS: ";
    if (_frameRate == 0)
        std::cout << "Unlimited";
    else
        std::cout << _frameRate;
    std::cout << '\n';
    reset();
}

void Clock::tick(bool sleepOnTick)
{
    ++_elapsedTime;
    _wakeTime += 1ms;
    if (_frameRate == 0.0)
    {
        _nextFrame = true;
        ++_frameCount;
    }
    else if (_elapsedTime == std::ceil(_nextFrameTime))
    {
        _nextFrame = true;
        ++_frameCount;
        _nextFrameTime += 1000 / _frameRate;
    }
    else
    {
        _nextFrame = false;
    }
    if (sleepOnTick)
        std::this_thread::sleep_until(_wakeTime);
}

void Clock::reset()
{
    _elapsedTime = 0;
    _nextFrame = true;
    _nextFrameTime = 1000 / _frameRate;
    _frameCount = 0;
}

void Clock::setEpoch() { _wakeTime = std::chrono::steady_clock::now(); }

std::int64_t Clock::getCurrentTime() const { return _elapsedTime + _offset; }

std::int64_t Clock::getElapsedTime() const { return _elapsedTime; }

bool Clock::nextFrame() const { return _nextFrame; }

std::size_t Clock::getFrameCount() const { return _frameCount; }