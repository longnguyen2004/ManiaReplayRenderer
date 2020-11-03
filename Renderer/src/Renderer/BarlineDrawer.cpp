#include "Renderer.hpp"
#include "SpriteManip.hpp"
#include <cmath>

Renderer::Stage::BarlineDrawer::BarlineDrawer(
    Renderer *ren, double hitPos, unsigned int stageStart, unsigned int stageEnd) :
    _ren(ren),
    _itUninherited(_ren->_map->getUninheritedTimingPoints().cbegin()),
    _itEnd(_ren->_map->getUninheritedTimingPoints().cend()),
    _hitPos(hitPos),
    _stageStart(stageStart),
    _stageEnd(stageEnd)
{
    _currentOffset = _itUninherited->getOffset();
    _currentBPM = _itUninherited->getBPM().value();
    _currentMeter = _itUninherited->getMeter();
    ++_itUninherited;
}

void Renderer::Stage::BarlineDrawer::update()
{
    unsigned int _stageWidth = _stageEnd - _stageStart;
    for (auto &i : _barlines)
    {
        double y_new = i.getPosition().y + 0.035 * _ren->_stage->_currentVel *
                                               _ren->_scrollSpeed *
                                               _ren->_scalingFactor;
        i.setPosition(i.getPosition().x, y_new);
    }
    while (!_barlines.empty() && _barlines.front().getPosition().y > _ren->_height)
        _barlines.erase(_barlines.begin());
    while (_ren->_map->getEndOffset() > _currentOffset &&
           (_barlines.empty() || _barlines.back().getPosition().y > 0))
    {
        double y =
            _hitPos - _ren->_positionCalc->getPosition(std::round(_currentOffset));
        auto &pos = _barlines.emplace_back(
            sf::Vector2f{_stageWidth * _ren->_scalingFactor, 2});
        pos.setOrigin(0, 2);
        pos.setPosition(
            _stageStart * _ren->_scalingFactor, y * _ren->_scalingFactor);
        _currentOffset += 60000 / _currentBPM * _currentMeter;
        if (_itUninherited != _ren->_map->getUninheritedTimingPoints().cend() &&
            _currentOffset > _itUninherited->getOffset())
        {
            _currentOffset = _itUninherited->getOffset();
            _currentBPM = _itUninherited->getBPM().value();
            _currentMeter = _itUninherited->getMeter();
            ++_itUninherited;
        }
    }
}

void Renderer::Stage::BarlineDrawer::draw()
{
    for (auto &i : _barlines)
        _ren->_target->draw(i);
}