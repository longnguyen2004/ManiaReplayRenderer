#include "PositionCalculator.hpp"
#include <iostream>

PositionCalculator::PositionCalculator(Map *map) :
    _baseBPM(map->getBaseBPM()), _currentOffset(0)
{
    preCalculate(map->getUninheritedTimingPoints(), map->getInheritedTimingPoints());
    _it_stateMap = std::next(_stateMap.cbegin());
}

double PositionCalculator::getPosition(std::int64_t offset) const
{
    auto [start_offset, state] = *std::prev(_stateMap.upper_bound(offset));
    return state._position + 0.035 * (offset - start_offset) * state._velocity;
}

void PositionCalculator::updateInternalState(std::int64_t newOffset)
{
    double delta = getPosition(newOffset) - getPosition(_currentOffset);
    for (auto &[offset, state] : _stateMap)
        state._position -= delta;
}

void PositionCalculator::preCalculate(
    const Map::TimingPointSet &uninherited, const Map::TimingPointSet &inherited)
{
    std::cout << "[PositionCalculator] Precalculating positions\n";
    std::int64_t prevOffset = 0;
    double prevPosition = 0.0;
    auto it_uninherited = uninherited.cbegin();
    auto it_inherited = inherited.cbegin();
    while (it_inherited->getOffset() < it_uninherited->getOffset())
        ++it_inherited;
    double vel = it_uninherited->getBPM().value() / _baseBPM;
    do
    {
        std::int64_t offset = it_uninherited->getOffset();
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