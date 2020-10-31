#ifndef RENDERERLIB_POSITIONCALCULATOR_HPP
#define RENDERERLIB_POSITIONCALCULATOR_HPP

#include "Map.hpp"
#include <cstdint>
#include <map>

class PositionCalculator
{
public:
    PositionCalculator(Map *map);
    ~PositionCalculator() = default;
    double getPosition(std::int64_t offset) const;
    void updateInternalState(std::int64_t newOffset);

private:
    void preCalculate(const Map::TimingPointSet &uninherited,
        const Map::TimingPointSet &inherited);
    struct State
    {
        double _position; // in osu pixels, with scroll speed = 1
        double _velocity; // in osu px / ms, with scroll speed = 1
    };
    std::map<std::int64_t, State> _stateMap;
    std::map<std::int64_t, State>::const_iterator _it_stateMap;
    double _baseBPM;
    std::int64_t _currentOffset;
};

#endif