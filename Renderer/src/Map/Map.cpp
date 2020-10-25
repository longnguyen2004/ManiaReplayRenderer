#include "Map.hpp"
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
    _inheritedPoints(&TimingPointCompOffset)
{
}

Map::Map(const std::string &pathToOsuFile) :
    _general("[General]"),
    _metadata("[Metadata]"),
    _difficulty("[Difficulty]"),
    _uninheritedPoints(&TimingPointCompOffset),
    _inheritedPoints(&TimingPointCompOffset)
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

double Map::getBaseBPM() const { return _baseBPM; }

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
    std::uint64_t maxDuration = 0;
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
                else if (std::uint64_t duration =
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