#include "Renderer.hpp"
#include "SpriteManip.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Renderer::Renderer(sf::RenderTarget *target, Map *map, Skin *skin, Clock *clock) :
    _target(target),
    _clock(clock),
    _frameCount(0),
    _width(_target->getSize().x),
    _height(_target->getSize().y),
    _scalingFactor(_height / 480.0f),
    _map(map),
    _skin(skin)
{
    std::cout << "[Renderer] Renderer settings:\n";
    std::cout << "[Renderer] Resolution: " << _width << 'x' << _height << '\n';
    initBG();
    _stage = std::make_unique<Stage>(this);
}

void Renderer::initBG()
{
    std::cout << "[Renderer] Loading map background\n";
    fs::path BGpath(_map->getMapDirectory());
    BGpath /= _map->getBGFilename();
    _mapBG.loadFromFile(BGpath.generic_string());
    _mapBG_sprite.setTexture(_mapBG);
    float width_ratio = static_cast<float>(_width) / _mapBG.getSize().x;
    float height_ratio = static_cast<float>(_height) / _mapBG.getSize().y;
    float scale_ratio = std::max(width_ratio, height_ratio);
    _mapBG_sprite.setScale(scale_ratio, scale_ratio);
    setOrigin(_mapBG_sprite, HorizPos::CENTER, VertPos::CENTER);
    _mapBG_sprite.setPosition(_width / 2, _height / 2);
}

bool Renderer::drawNextFrame()
{
    if (_clock->nextFrame())
    {
        _target->clear();
        _target->draw(_mapBG_sprite);
        _stage->drawNextFrame();
        ++_frameCount;
        return true;
    }
    else
    {
        return false;
    }
}

std::size_t Renderer::getFrameCount() const { return _frameCount; }
