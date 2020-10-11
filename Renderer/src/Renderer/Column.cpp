#include "Renderer.hpp"

Renderer::Stage::Column::Column(
    Renderer *ren, float columnStart, float columnWidth, float columnLineWidth) :
    _ren(ren),
    _columnStart(columnStart),
    _columnWidth(columnWidth),
    _columnLineWidth(columnLineWidth),
    _columnRect(
        {_columnWidth * _ren->_scalingFactor, static_cast<float>(_ren->_height)}),
    _columnLine({_columnLineWidth, static_cast<float>(_ren->_height)})
{
    _columnRect.setPosition(_columnStart * _ren->_scalingFactor, 0.0f);
    _columnRect.setFillColor(sf::Color::Black);
    _columnLine.setPosition(
        (_columnStart + _columnWidth) * _ren->_scalingFactor - 1.0f, 0.0f);
    _columnLine.setFillColor(sf::Color::White);
}

void Renderer::Stage::Column::draw()
{
    _ren->_target->draw(_columnRect);
    _ren->_target->draw(_columnLine);
}