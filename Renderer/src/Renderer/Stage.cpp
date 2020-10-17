#include "../InputFacet/InputFacet.hpp"
#include "Renderer.hpp"
#include "SpriteManip.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

Renderer::Stage::Stage(Renderer *ren) : _ren(ren)
{
    _keys = std::stoi(_ren->_map->getDifficultySettings()["CircleSize"]);
    _hitPos = std::stoi(_ren->_skin->getManiaSettings(_keys)["HitPosition"]);
    createColumns();
    loadStageLeftRightHint();
}

void Renderer::Stage::drawNextFrame()
{
    drawColumns();
    drawStageLeftRightHint();
}

void Renderer::Stage::createColumns()
{
    std::cout << "[Renderer::Stage] Creating columns\n";
    _columns.clear();
    auto maniaSettings = _ren->_skin->getManiaSettings(_keys);
    _stageStart = std::stoi(maniaSettings["ColumnStart"]);
    _stageEnd = _stageStart;
    auto columnWidthStr = maniaSettings["ColumnWidth"];
    auto columnLineWidthStr = maniaSettings["ColumnLineWidth"];
    std::istringstream is1(columnWidthStr);
    std::istringstream is2(columnLineWidthStr);
    is1.imbue(std::locale(std::locale::classic(), new CommaAsSeparator()));
    is2.imbue(std::locale(std::locale::classic(), new CommaAsSeparator()));
    int temp;
    is2 >> temp; // ignore the first number for now
    for (unsigned int i = 0; i < _keys; ++i)
    {
        unsigned int width, lineWidth;
        is1 >> width;
        is2 >> lineWidth;
        _columns.emplace_back(_ren, _stageEnd, width, lineWidth);
        _stageEnd += width;
    }
}

void Renderer::Stage::drawColumns()
{
    for (auto &i : _columns)
        i.draw();
}

void Renderer::Stage::loadStageLeftRightHint()
{
    std::cout << "[Renderer::Stage] Loading stage left\n";
    auto stageLeft_path =
        _ren->_skin->getPathToElement("mania-stage-left").generic_string();
    _stageLeft.loadFromFile(stageLeft_path);
    _stageLeft_sprite.setTexture(_stageLeft);
    setOrigin(_stageLeft_sprite, HorizPos::RIGHT);
    scaleHeight(_stageLeft_sprite, _ren->_height, false);
    _stageLeft_sprite.setPosition(_stageStart * _ren->_scalingFactor, 0);

    std::cout << "[Renderer::Stage] Loading stage right\n";
    auto stageRight_path =
        _ren->_skin->getPathToElement("mania-stage-right").generic_string();
    _stageRight.loadFromFile(stageRight_path);
    _stageRight_sprite.setTexture(_stageRight);
    setOrigin(_stageRight_sprite, HorizPos::LEFT);
    scaleHeight(_stageRight_sprite, _ren->_height, false);
    _stageRight_sprite.setPosition(_stageEnd * _ren->_scalingFactor, 0);

    std::cout << "[Renderer::Stage] Loading stage hint\n";
    auto stageHint_path =
        _ren->_skin->getPathToElement("mania-stage-hint").generic_string();
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