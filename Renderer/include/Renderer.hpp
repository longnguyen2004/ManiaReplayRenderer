#ifndef RENDERERLIB_RENDERER_HPP
#define RENDERERLIB_RENDERER_HPP

#include "Export.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <list>
#include <memory>

#include "Clock.hpp"
#include "Map.hpp"
#include "Skin.hpp"

class RENDERERLIB_EXPORT Renderer
{
public:
    Renderer(sf::RenderTarget *target, Map *map, Skin *skin, Clock *clock);
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

    Skin *_skin;
    class Stage;

    std::unique_ptr<Stage> _stage;
    void initBG();
};

class Renderer::Stage
{
public:
    Stage(Renderer *ren);
    void drawNextFrame();

private:
    Renderer *_ren;
    std::list<sf::RectangleShape> _columns;
    unsigned int _stageStart, _stageEnd;
    sf::Texture _stageLeft, _stageRight;
    sf::Sprite _stageLeft_sprite, _stageRight_sprite;
    void createColumns();
    void drawColumns();
    void loadStageLeftRight();
    void drawStageLeftRight();
};

#endif