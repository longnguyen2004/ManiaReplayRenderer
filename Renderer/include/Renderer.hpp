#ifndef RENDERERLIB_RENDERER_HPP
#define RENDERERLIB_RENDERER_HPP

#include "Export.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <list>
#include <memory>
#include <utility>
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
    void update();
    void draw();

private:
    class ColumnDrawer;
    class BarlineDrawer;

    using VelocityIt = PositionCalculator::VelocityStateMap::const_iterator;

    Renderer *_ren;
    unsigned int _stageStart, _stageEnd, _hitPos;
    unsigned int _keys;
    sf::Texture _stageLeft, _stageRight, _stageHint;
    sf::Sprite _stageLeft_sprite, _stageRight_sprite, _stageHint_sprite;
    double _currentVel;
    VelocityIt _nextVel, _endVel;

    std::unique_ptr<ColumnDrawer> _columnDrawer;
    std::unique_ptr<BarlineDrawer> _barlineDrawer;

    void loadStageLeftRightHint();
    void drawStageLeftRightHint();
};

class Renderer::Stage::ColumnDrawer
{
public:
    ColumnDrawer(Renderer *ren, unsigned int keys);
    void draw();
    std::pair<unsigned int, unsigned int> getStageBound() const;

private:
    Renderer *_ren;
    std::vector<sf::RectangleShape> _columns;
    std::list<sf::RectangleShape> _columnLines;
    unsigned int _stageStart, _stageEnd;
};

class Renderer::Stage::BarlineDrawer
{
public:
    BarlineDrawer(Renderer *ren,
        double hitPos,
        unsigned int stageStart,
        unsigned int stageEnd);
    void update();
    void draw();

private:
    Renderer *_ren;
    double _hitPos;
    unsigned int _stageStart, _stageEnd;
    double _currentOffset;
    double _currentBPM;
    unsigned int _currentMeter;
    std::list<sf::RectangleShape> _barlines;
    std::list<sf::RectangleShape>::iterator _barlineBegin;
    Map::TimingPointSet::const_iterator _itUninherited, _itEnd;
};
#endif