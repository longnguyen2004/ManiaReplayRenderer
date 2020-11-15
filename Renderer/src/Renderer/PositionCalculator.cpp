#include "PositionCalculator.hpp"
#include <iostream>

using VelocityStateMap = PositionCalculator::VelocityStateMap;

PositionCalculator::PositionCalculator(
    Map *map, std::int64_t startOffset, float scrollSpeed) :
    _baseBPM(map->getBaseBPM()), _scrollSpeed(scrollSpeed)
{
    preCalculate(map->getUninheritedTimingPoints(), map->getInheritedTimingPoints(),
        startOffset);
    auto firstVel = _stateMap.begin()->second._velocity;
    _stateMap[startOffset] = {0.0, firstVel};
}

float PositionCalculator::getPosition(std::int64_t offset) const
{
    auto [startOffset, state] = *std::prev(_stateMap.upper_bound(offset));
    return state._position +
           0.035 * (offset - startOffset) * state._velocity * _scrollSpeed;
}

void PositionCalculator::updateInternalState(float distancePassed)
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
    float prevPosition = 0.0;
    auto it_uninherited = uninherited.cbegin();
    auto it_inherited = inherited.cbegin();
    while (!inherited.empty() &&
           it_inherited->getOffset() < it_uninherited->getOffset())
        ++it_inherited;
    float vel = it_uninherited->getBPM().value() / _baseBPM;
    do
    {
        std::int64_t offset = it_uninherited->getOffset();
        float baseVel = it_uninherited->getBPM().value() / _baseBPM;
        float position =
            prevPosition + 0.035 * (offset - prevOffset) * vel * _scrollSpeed;
        _stateMap[offset] = {position, baseVel};
        prevOffset = offset;
        prevPosition = position;
        ++it_uninherited;
        vel = baseVel;
        while (it_inherited != inherited.cend() &&
               (it_uninherited == uninherited.cend() ||
                   it_inherited->getOffset() < it_uninherited->getOffset()))
        {
            offset = it_inherited->getOffset();
            position =
                prevPosition + 0.035 * (offset - prevOffset) * vel * _scrollSpeed;
            vel = baseVel * it_inherited->getSV().value();
            _stateMap[offset] = {position, vel};
            prevOffset = offset;
            prevPosition = position;
            ++it_inherited;
        }
    } while (it_uninherited != uninherited.cend());
}