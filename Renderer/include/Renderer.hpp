#ifndef RENDERERLIB_RENDERER_HPP
#define RENDERERLIB_RENDERER_HPP

#include <cstddef>

#include "Export.hpp"
#include <SFML/Graphics.hpp>

#include "Clock.hpp"
#include "Map.hpp"

class RENDERERLIB_EXPORT Renderer
{
public:
    Renderer(sf::RenderTarget *target, Map *map, Clock *clock);
    bool drawNextFrame();
    std::size_t getFrameCount() const;
    ~Renderer() = default;

private:
    sf::RenderTarget *_target;
    unsigned int _width, _height;
    float _scalingFactor;
    std::size_t _frameCount;

    Clock *_clock;

    Map *_map;
    sf::Texture _mapBG;
    sf::Sprite _mapBG_sprite;

    void initBG();
};

#endif