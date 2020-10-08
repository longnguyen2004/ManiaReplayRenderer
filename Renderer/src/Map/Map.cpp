#include "Map.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>

#include "../ANSIEscape/Color.hpp"
#include "../InputFacet/InputFacet.hpp"

namespace fs = std::filesystem;

Map::Map() {}

Map::Map(const std::string &pathToOsuFile) :
    _general("[General]"), _metadata("[Metadata]"), _difficulty("[Difficulty]")
{
    loadFromOsuFile(pathToOsuFile);
}

Map::Map(std::istream &stream) :
    _general("[General]"), _metadata("[Metadata]"), _difficulty("[Difficulty]")
{
    loadFromInputStream(stream);
}

void Map::loadFromOsuFile(const std::string &pathToOsuFile)
{
    fs::path mapPath = fs::u8path(pathToOsuFile);
    if (fs::exists(mapPath))
    {
        _parentDir = mapPath.parent_path().generic_string();
        std::cout << "[Map] Opening file " << pathToOsuFile << '\n';
        std::ifstream stream(pathToOsuFile);
        loadFromInputStream(stream);
    }
    else
    {
        std::cerr << Color::Error << "[Map] Cannot find file " << pathToOsuFile
                  << Color::Reset << '\n';
    }
}

void Map::loadFromInputStream(std::istream &stream)
{
    std::cout << "[Map] Loading from stream\n";
    std::cout << "[Map::Settings] Loading general settings\n";
    _general.loadFromInputStream(stream);
    std::cout << "[Map::Settings] Loading metadata\n";
    _metadata.loadFromInputStream(stream);
    std::cout << "[Map::Settings] Loading difficulty settings\n";
    _difficulty.loadFromInputStream(stream);

    std::string s;
    do
    {
        std::getline(stream, s);
    } while (s != "//Background and Video events"); // pretty ugly but meh

    stream.ignore(4); // 0,0,
    stream >> std::quoted(_BGname);
}

const std::string &Map::getMapDirectory() const { return _parentDir; }

const Settings &Map::getGeneralSetings() const { return _general; }

const Settings &Map::getMetadata() const { return _metadata; }

const Settings &Map::getDifficultySettings() const { return _difficulty; }

const std::string &Map::getBGFilename() const { return _BGname; }