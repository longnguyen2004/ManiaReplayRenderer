#include "../InputFacet/InputFacet.hpp"
#include "Renderer.hpp"
#include <iostream>
#include <sstream>

Renderer::Stage::ColumnDrawer::ColumnDrawer(Renderer *ren, unsigned int keys) :
    _ren(ren), _smallestCol(std::numeric_limits<float>::max())
{
    std::cout << "[Renderer::Stage::ColumnDrawer] Loading columns\n";
    const auto &maniaSettings = _ren->_skin->getManiaSettings(keys);
    auto columnWidthStr = maniaSettings["ColumnWidth"];
    auto columnLineWidthStr = maniaSettings["ColumnLineWidth"];
    _stageStart = std::stoi(maniaSettings["ColumnStart"]);
    _stageEnd = _stageStart;
    std::istringstream is1(columnWidthStr);
    std::istringstream is2(columnLineWidthStr);
    is1.imbue(std::locale(std::locale::classic(), new CommaAsSeparator()));
    is2.imbue(std::locale(std::locale::classic(), new CommaAsSeparator()));
    float columnWidth, columnLineWidth;
    for (unsigned int i = 0; i < keys; ++i)
    {
        is1 >> columnWidth;
        is2 >> columnLineWidth;
        auto &column = _columns.emplace_back(
            sf::Vector2f(columnWidth * _ren->_scalingFactor, _ren->_height));
        column.setPosition(_stageEnd * _ren->_scalingFactor, 0);
        column.setFillColor(sf::Color::Black);
        auto &columnLine = _columnLines.emplace_back(
            sf::Vector2f(columnLineWidth * _ren->_scalingFactor, _ren->_height));
        columnLine.setPosition(_stageEnd * _ren->_scalingFactor, 0);
        columnLine.setFillColor(sf::Color::White);
        _stageEnd += columnWidth;
        _smallestCol = std::min(_smallestCol, columnWidth);
    }
    is2 >> columnLineWidth;
    auto &column = _columnLines.emplace_back(
        sf::Vector2f(columnLineWidth * _ren->_scalingFactor, _ren->_height));
    column.setPosition(_stageEnd * _ren->_scalingFactor, 0);
    column.setFillColor(sf::Color::White);
}

void Renderer::Stage::ColumnDrawer::draw()
{
    for (auto &i : _columns)
    {
        _ren->_target->draw(i);
    }
    for (auto &i : _columnLines)
    {
        _ren->_target->draw(i);
    }
}

float Renderer::Stage::ColumnDrawer::getSmallestColumnWidth() const
{
    return _smallestCol;
}

std::pair<float, float> Renderer::Stage::ColumnDrawer::getColumnBound(
    unsigned int col) const
{
    auto start = _columns[col].getPosition().x / _ren->_scalingFactor;
    auto end = start + _columns[col].getSize().x / _ren->_scalingFactor;
    return {start, end};
}

std::pair<float, float> Renderer::Stage::ColumnDrawer::getStageBound() const
{
    return {_stageStart, _stageEnd};
}