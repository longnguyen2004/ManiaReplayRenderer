#ifndef RENDERERLIB_POSITIONCALCULATOR_HPP
#define RENDERERLIB_POSITIONCALCULATOR_HPP

#include "Map.hpp"
#include <cstddef>
#include <map>

class PositionCalculator
{
public:
    PositionCalculator(Map *map);
    ~PositionCalculator() = default;
    double getPosition(std::size_t offset);

private:
    void preCalculate(const Map::TimingPointSet &uninherited,
        const Map::TimingPointSet &inherited);
    struct State
    {
        double _position; // in osu pixels
        double _velocity; // in osu px / ms, with scroll speed = 1
    };
    std::map<std::size_t, State> _stateMap;
    double _baseBPM;
};

#endif