#include "../InputFacet/InputFacet.hpp"
#include "Renderer.hpp"
#include "SpriteManip.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

Renderer::Stage::Stage(Renderer *ren) :
    _ren(ren),
    _nextVel(_ren->_positionCalc->getStateMap().cbegin()),
    _endVel(_ren->_positionCalc->getStateMap().cend())
{
    _keys = std::stoi(_ren->_map->getDifficultySettings()["CircleSize"]);
    _hitPos = std::stoi(_ren->_skin->getManiaSettings(_keys)["HitPosition"]);
    _currentVel = _nextVel->second._velocity;
    ++_nextVel;
    _columnDrawer = std::make_unique<ColumnDrawer>(_ren, _keys);
    _stageStart = _columnDrawer->getStageBound().first;
    _stageEnd = _columnDrawer->getStageBound().second;
    _barlineDrawer =
        std::make_unique<BarlineDrawer>(_ren, _hitPos, _stageStart, _stageEnd);
    loadStageLeftRightHint();
}

void Renderer::Stage::update()
{
    if (_nextVel != _endVel && _ren->_clock->getCurrentTime() == _nextVel->first)
    {
        _currentVel = _nextVel->second._velocity;
        ++_nextVel;
    }
    _ren->_positionCalc->updateInternalState(
        0.035 * _currentVel * _ren->_scrollSpeed);
    _barlineDrawer->update();
}

void Renderer::Stage::draw()
{
    _columnDrawer->draw();
    _barlineDrawer->draw();
    drawStageLeftRightHint();
}

void Renderer::Stage::loadStageLeftRightHint()
{
    std::cout << "[Renderer::Stage] Loading stage left\n";
    auto stageLeft_path =
        _ren->_skin->getPathToElement("mania-stage-left", "", _keys)
            .generic_string();
    _stageLeft.loadFromFile(stageLeft_path);
    _stageLeft_sprite.setTexture(_stageLeft);
    setOrigin(_stageLeft_sprite, HorizPos::RIGHT);
    scaleHeight(_stageLeft_sprite, _ren->_height, false);
    _stageLeft_sprite.setPosition(_stageStart * _ren->_scalingFactor, 0);

    std::cout << "[Renderer::Stage] Loading stage right\n";
    auto stageRight_path =
        _ren->_skin->getPathToElement("mania-stage-right", "", _keys)
            .generic_string();
    _stageRight.loadFromFile(stageRight_path);
    _stageRight_sprite.setTexture(_stageRight);
    setOrigin(_stageRight_sprite, HorizPos::LEFT);
    scaleHeight(_stageRight_sprite, _ren->_height, false);
    _stageRight_sprite.setPosition(_stageEnd * _ren->_scalingFactor, 0);

    std::cout << "[Renderer::Stage] Loading stage hint\n";
    auto stageHint_path =
        _ren->_skin->getPathToElement("mania-stage-hint", "", _keys)
            .generic_string();
    _stageHint.loadFromFile(stageHint_path);
    _stageHint_sprite.setTexture(_stageHint);
    setOrigin(_stageHint_sprite, HorizPos::LEFT, VertPos::CENTER);
    scaleWidth(
        _stageHint_sprite, (_stageEnd - _stageStart) * _ren->_scalingFactor, false);
    _stageHint_sprite.setPosition(
        _stageStart * _ren->_scalingFactor, _hitPos * _ren->_scalingFactor);
}

void Renderer::Stage::drawStageLeftRightHint()
{
    _ren->_target->draw(_stageLeft_sprite);
    _ren->_target->draw(_stageRight_sprite);
    _ren->_target->draw(_stageHint_sprite);
}