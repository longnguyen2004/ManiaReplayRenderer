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
    createColumns();
    loadStageLeftRight();
}

void Renderer::Stage::drawNextFrame()
{
    drawColumns();
    drawStageLeftRight();
}

void Renderer::Stage::createColumns()
{
    std::cout << "[Mania::Stage] Creating columns\n";
    _columns.clear();
    int keys = std::stoi(_ren->_map->getDifficultySettings()["CircleSize"]);
    auto maniaSettings = _ren->_skin->getManiaSettings(keys);
    _stageStart = std::stoi(maniaSettings["ColumnStart"]);
    _stageEnd = _stageStart;
    auto columnWidthStr = maniaSettings["ColumnWidth"];
    std::replace(columnWidthStr.begin(), columnWidthStr.end(), ',', ' ');
    std::istringstream is(columnWidthStr);
    for (int i = 0; i < keys; ++i)
    {
        int width;
        is >> width;
        sf::RectangleShape column(
            {width * _ren->_scalingFactor, static_cast<float>(_ren->_height)});
        column.setPosition(_stageEnd * _ren->_scalingFactor, 0);
        column.setFillColor(sf::Color::Black);
        _columns.push_back(std::move(column));
        _stageEnd += width;
    }
}

void Renderer::Stage::drawColumns()
{
    for (auto &i : _columns)
        _ren->_target->draw(i);
}

void Renderer::Stage::loadStageLeftRight()
{
    auto stageLeft_path =
        _ren->_skin->getPathToElement("mania-stage-left").generic_string();
    auto stageRight_path =
        _ren->_skin->getPathToElement("mania-stage-right").generic_string();

    _stageLeft.loadFromFile(stageLeft_path);
    _stageLeft_sprite.setTexture(_stageLeft);
    setOrigin(_stageLeft_sprite, HorizPos::RIGHT);
    scaleHeight(_stageLeft_sprite, _ren->_height, false);
    _stageLeft_sprite.setPosition(_stageStart * _ren->_scalingFactor, 0);

    _stageRight.loadFromFile(stageRight_path);
    _stageRight_sprite.setTexture(_stageRight);
    setOrigin(_stageRight_sprite, HorizPos::LEFT);
    scaleHeight(_stageRight_sprite, _ren->_height, false);
    _stageRight_sprite.setPosition(_stageEnd * _ren->_scalingFactor, 0);
}

void Renderer::Stage::drawStageLeftRight()
{
    _ren->_target->draw(_stageLeft_sprite);
    _ren->_target->draw(_stageRight_sprite);
}