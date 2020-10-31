#ifndef RENDERERLIB_RENDERER_HPP
#define RENDERERLIB_RENDERER_HPP

#include "Export.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <list>
#include <memory>
#include <vector>

#include "Clock.hpp"
#include "Map.hpp"
#include "PositionCalculator.hpp"
#include "Skin.hpp"

class RENDERERLIB_EXPORT Renderer
{
public:
    Renderer(sf::RenderTarget *target,
        Clock *clock,
        Map *map,
        Skin *skin,
        double scrollSpeed);
    bool updateState();
    std::size_t getFrameCount() const;
    ~Renderer() = default;

private:
    sf::RenderTarget *_target;
    unsigned int _width, _height;
    float _scalingFactor;
    std::size_t _frameCount;
    double _scrollSpeed;

    Clock *_clock;
    Map *_map;
    Skin *_skin;

    sf::Texture _mapBG;
    sf::Sprite _mapBG_sprite;

    class Stage;

    std::unique_ptr<Stage> _stage;
    std::unique_ptr<PositionCalculator> _positionCalc;
    void initBG();
};

class Renderer::Stage
{
public:
    Stage(Renderer *ren);
    void draw();

private:
    class Column;

    Renderer *_ren;
    std::vector<Column> _columns;
    unsigned int _stageStart, _stageEnd, _hitPos;
    unsigned int _keys;
    sf::Texture _stageLeft, _stageRight, _stageHint;
    sf::Sprite _stageLeft_sprite, _stageRight_sprite, _stageHint_sprite;
    void createColumns();
    void drawColumns();
    void loadStageLeftRightHint();
    void drawStageLeftRightHint();
};

class Renderer::Stage::Column
{
public:
    Column(Renderer *ren,
        float columnStart,
        float columnWidth,
        float columnLineWidth = 0.0f);
    void draw();

private:
    Renderer *_ren;
    float _columnStart, _columnWidth, _columnLineWidth;
    sf::RectangleShape _columnRect, _columnLine;
};
#endif