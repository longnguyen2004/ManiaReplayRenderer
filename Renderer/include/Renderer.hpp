#ifndef RENDERERLIB_RENDERER_HPP
#define RENDERERLIB_RENDERER_HPP

#include "Export.hpp"
#include <SFML/Graphics.hpp>

#include "Map.hpp"
#include "Clock.hpp"

class RENDERERLIB_EXPORT Renderer
{
private:
    sf::RenderTarget *_target;
    unsigned int _width, _height;
    float _scalingFactor;

    Clock *_clock;

    Map *_map;
    sf::Texture _mapBG;
    sf::Sprite _mapBG_sprite;

    void initBG();


public:
    Renderer(sf::RenderTarget *target, Map *map, Clock *clock);
    bool drawNextFrame();
    ~Renderer() = default;
};

#endif