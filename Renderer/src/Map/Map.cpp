#include "Map.hpp"
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>

#include "../ANSIEscape/Color.hpp"

namespace fs = std::filesystem;

Map::Map() :
    _general("[General]"),
    _metadata("[Metadata]"),
    _difficulty("[Difficulty]"),
    _uninheritedPoints(&TimingPointCompOffset),
    _inheritedPoints(&TimingPointCompOffset),
    _objects(&ObjectCompOffset),
    _endOffset(0)
{
}

Map::Map(const std::string &pathToOsuFile) :
    _general("[General]"),
    _metadata("[Metadata]"),
    _difficulty("[Difficulty]"),
    _uninheritedPoints(&TimingPointCompOffset),
    _inheritedPoints(&TimingPointCompOffset),
    _objects(&ObjectCompOffset),
    _endOffset(0)
{
    loadFromOsuFile(pathToOsuFile);
}

void Map::loadFromOsuFile(const std::string &pathToOsuFile)
{
    if (fs::path mapPath = fs::u8path(pathToOsuFile); fs::exists(mapPath))
    {
        _parentDir = mapPath.parent_path().generic_string();
        std::cout << "[Map] Opening file " << pathToOsuFile << '\n';
        _filestream.open(mapPath);
        loadSettings();
        loadTimingPoints();
        loadObjects();
        _leadIn = std::min(0LL, _objects.cbegin()->getStartTime() - 2000);
    }
    else
    {
        std::cerr << Color::Error << "[Map] Cannot find file " << pathToOsuFile
                  << Color::Reset << '\n';
    }
}

void Map::loadSettings()
{
    std::cout << "[Map::Settings] Loading settings\n";
    std::cout << "[Map::Settings] Loading general settings\n";
    _general.loadFromInputStream(_filestream);
    std::cout << "[Map::Settings] Loading metadata\n";
    _metadata.loadFromInputStream(_filestream);
    std::cout << "[Map::Settings] Loading difficulty settings\n";
    _difficulty.loadFromInputStream(_filestream);

    std::string s;
    do
    {
        std::getline(_filestream, s);
    } while (s != "//Background and Video events"); // pretty ugly but meh

    _filestream.ignore(4); // 0,0,
    _filestream >> std::quoted(_BGname);
}

void Map::loadTimingPoints()
{
    _uninheritedPoints.clear();
    _inheritedPoints.clear();
    std::cout << "[Map::TimingPoint] Loading timing points\n";
    std::string s;
    do
    {
        std::getline(_filestream, s);
    } while (_filestream && s != "[TimingPoints]");
    std::int64_t maxDuration = 0;
    TimingPoint prevTimingPoint;
    bool firstEncountered = false;
    while (_filestream && _filestream.peek() != '[')
    {
        std::getline(_filestream, s);
        if (!s.empty())
        {
            TimingPoint last(s);
            if (last.isUninherited())
            {
                _uninheritedPoints.insert(last);
                if (!firstEncountered)
                {
                    firstEncountered = true;
                    _baseBPM = last.getBPM().value();
                }
                else if (std::int64_t duration =
                             last.getOffset() - prevTimingPoint.getOffset();
                         duration > maxDuration)
                {
                    maxDuration = duration;
                    _baseBPM = prevTimingPoint.getBPM().value();
                }
                prevTimingPoint = last;
            }
            else
            {
                _inheritedPoints.insert(last);
            }
        }
    }
}

void Map::loadObjects()
{
    _objects.clear();
    unsigned int columnCount = std::stoi(_difficulty["CircleSize"]);
    std::cout << "[Map::Object] Loading objects\n";
    std::string s;
    do
    {
        std::getline(_filestream, s);
    } while (_filestream && s != "[HitObjects]");
    while (_filestream && _filestream.peek() != '[')
    {
        std::getline(_filestream, s);
        ObjectSet::const_iterator pos;
        if (!s.empty())
            pos = _objects.emplace(s, columnCount);
        if (pos->isLN())
            _endOffset = pos->getEndTime();
        else
            _endOffset = pos->getStartTime();
    }
}

const std::string &Map::getMapDirectory() const { return _parentDir; }

const Settings &Map::getGeneralSetings() const { return _general; }

const Settings &Map::getMetadata() const { return _metadata; }

const Settings &Map::getDifficultySettings() const { return _difficulty; }

const std::string &Map::getBGFilename() const { return _BGname; }

const Map::TimingPointSet &Map::getUninheritedTimingPoints() const
{
    return _uninheritedPoints;
}

const Map::TimingPointSet &Map::getInheritedTimingPoints() const
{
    return _inheritedPoints;
}

const Map::ObjectSet &Map::getObjects() const { return _objects; }

double Map::getBaseBPM() const { return _baseBPM; }

std::int64_t Map::getLeadIn() const { return _leadIn; }

std::int64_t Map::getEndOffset() const { return _endOffset; }