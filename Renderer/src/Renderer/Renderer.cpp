#include "Renderer.hpp"
#include "SpriteManip.hpp"
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
    _stage = std::make_unique<Stage>(this);
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
    _mapBG_sprite.setTexture(_mapBG);
    setOrigin(_mapBG_sprite, HorizPos::CENTER, VertPos::CENTER);
    scaleHeight(_mapBG_sprite, _height);
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