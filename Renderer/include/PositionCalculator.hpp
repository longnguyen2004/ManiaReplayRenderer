#ifndef RENDERERLIB_POSITIONCALCULATOR_HPP
#define RENDERERLIB_POSITIONCALCULATOR_HPP

#include "Map.hpp"
#include <cstdint>
#include <map>

class PositionCalculator
{
public:
    struct State
    {
        double _position; // in osu pixels
        double _velocity; // in osu px / ms
    };
    using VelocityStateMap = std::map<std::int64_t, State>;
    PositionCalculator(Map *map, std::int64_t startOffset, double scrollSpeed);
    ~PositionCalculator() = default;
    double getPosition(std::int64_t offset) const;
    void updateInternalState(double distancePassed);
    const VelocityStateMap &getStateMap() const;

private:
    void preCalculate(const Map::TimingPointSet &uninherited,
        const Map::TimingPointSet &inherited,
        std::int64_t startOffset);
    VelocityStateMap _stateMap;
    double _baseBPM, _scrollSpeed;
};

#endif