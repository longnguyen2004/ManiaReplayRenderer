#include "Renderer.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Renderer::Renderer(sf::RenderTarget *target, Map *map, Clock *clock) :
    _target(target), _clock(clock), _frameCount(0),
    _width(_target->getSize().x), _height(_target->getSize().y),
    _scalingFactor(_height / 480.0f), _map(map)
{
    std::cout << "[Renderer] Renderer settings:\n";
    std::cout << "[Renderer] Resolution: " << _width << 'x' << _height << '\n';
    initBG();
}

void Renderer::initBG()
{
    std::cout << "[Renderer] Loading map background\n";
    fs::path BGpath(_map->getMapDirectory());
    BGpath /= _map->getBGFilename();
    _mapBG.loadFromFile(BGpath.generic_string());
    auto [x, y] = _mapBG.getSize();
    _mapBG_sprite.setTexture(_mapBG);
    _mapBG_sprite.setOrigin(x / 2, y / 2);
    float BGscale = _height / static_cast<float>(y);
    _mapBG_sprite.setScale(BGscale, BGscale);
    _mapBG_sprite.setPosition(_width / 2, _height / 2);
}

bool Renderer::drawNextFrame()
{
    if (_clock->nextFrame())
    {
        _target->clear();
        _target->draw(_mapBG_sprite);
        ++_frameCount;
        return true;
    }
    else
    {
        return false;
    }
}

std::size_t Renderer::getFrameCount() const { return _frameCount; }