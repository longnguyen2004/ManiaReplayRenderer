#include "PositionCalculator.hpp"
#include <iostream>

using VelocityStateMap = PositionCalculator::VelocityStateMap;

PositionCalculator::PositionCalculator(Map *map, std::int64_t startOffset) :
    _baseBPM(map->getBaseBPM())
{
    preCalculate(map->getUninheritedTimingPoints(), map->getInheritedTimingPoints(),
        startOffset);
    auto firstVel = _stateMap.begin()->second._velocity;
    _stateMap[startOffset] = {0.0, firstVel};
}

double PositionCalculator::getPosition(std::int64_t offset) const
{
    auto [startOffset, state] = *std::prev(_stateMap.upper_bound(offset));
    return state._position + 0.035 * (offset - startOffset) * state._velocity;
}

void PositionCalculator::updateInternalState(double distancePassed)
{
    for (auto &[offset, state] : _stateMap)
        state._position -= distancePassed;
}

const VelocityStateMap &PositionCalculator::getStateMap() const { return _stateMap; }

void PositionCalculator::preCalculate(const Map::TimingPointSet &uninherited,
    const Map::TimingPointSet &inherited,
    std::int64_t startOffset)
{
    std::cout << "[PositionCalculator] Precalculating positions\n";
    std::int64_t prevOffset = startOffset;
    double prevPosition = 0.0;
    auto it_uninherited = uninherited.cbegin();
    auto it_inherited = inherited.cbegin();
    while (!inherited.empty() &&
           it_inherited->getOffset() < it_uninherited->getOffset())
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