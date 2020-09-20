#include "Map.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <filesystem>

#include "../InputFacet/InputFacet.hpp"

namespace fs = std::filesystem;

Map::Map()
{
}

Map::Map(const std::string& pathToOsuFile) :
    _general("[General]"),
    _metadata("[Metadata]"),
    _difficulty("[Difficulty]")
{
    loadFromOsuFile(pathToOsuFile);
}

Map::Map(std::istream& stream) :
    _general("[General]"),
    _metadata("[Metadata]"),
    _difficulty("[Difficulty]")
{
    loadFromInputStream(stream);
}

void Map::loadFromOsuFile(const std::string& pathToOsuFile)
{
    fs::path mapPath = fs::u8path(pathToOsuFile);
    _parentDir = mapPath.parent_path().generic_string();
    std::cout << "[Map] Opening file " << pathToOsuFile << '\n';
    std::ifstream stream(pathToOsuFile);
    loadFromInputStream(stream);
}

void Map::loadFromInputStream(std::istream& stream)
{
    std::cout << "[Map] Loading from stream\n";
    
    _general.loadFromInputStream(stream);
    _metadata.loadFromInputStream(stream);
    _difficulty.loadFromInputStream(stream);
    
    std::string s;
    do
    {
        std::getline(stream, s);
    } while (s != "//Background and Video events");     // pretty ugly but meh

    stream.ignore(4);                                   // 0,0,
    stream >> std::quoted(_BGname);
}

const std::string& Map::getMapDirectory() const
{
    return _parentDir;
}

const Settings& Map::getGeneralSetings() const
{
    return _general;
}

const Settings& Map::getMetadata() const
{
    return _metadata;
}

const Settings& Map::getDifficultySettings() const
{
    return _difficulty;
}

const std::string& Map::getBGFilename() const
{
    return _BGname;
}