#include "PositionCalculator.hpp"
#include <iostream>

PositionCalculator::PositionCalculator(Map *map) : _baseBPM(map->getBaseBPM())
{
    preCalculate(map->getUninheritedTimingPoints(), map->getInheritedTimingPoints());
}

void PositionCalculator::preCalculate(
    const Map::TimingPointSet &uninherited, const Map::TimingPointSet &inherited)
{
    std::cout << "[PositionCalculator] Precalculating positions\n";
    std::size_t prevOffset = 0;
    double prevPosition = 0.0;
    auto it_uninherited = uninherited.cbegin();
    auto it_inherited = inherited.cbegin();
    while (it_inherited->getOffset() < it_uninherited->getOffset())
        ++it_inherited;
    double vel = it_uninherited->getBPM().value() / _baseBPM;
    do
    {
        std::size_t offset = it_uninherited->getOffset();
        double globalSpeed = it_uninherited->getBPM().value() / _baseBPM;
        double position = prevPosition + 0.035 * (offset - prevOffset) * vel;
        _stateMap[offset] = {position, globalSpeed};
        prevOffset = offset;
        prevPosition = position;
        ++it_uninherited;
        while (it_inherited != inherited.cend() &&
               (it_uninherited == uninherited.cend() ||
                   it_inherited->getOffset() < it_uninherited->getOffset()))
        {
            offset = it_inherited->getOffset();
            vel = globalSpeed * it_inherited->getSV().value();
            position = prevPosition + 0.035 * (offset - prevOffset) * vel;
            _stateMap[offset] = {position, vel};
            prevOffset = offset;
            prevPosition = position;
            ++it_inherited;
        }
    } while (it_uninherited != uninherited.cend());
}