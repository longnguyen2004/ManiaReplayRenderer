#include "Settings.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

#include "../InputFacet/InputFacet.hpp"

Settings::Settings(std::string section) : _section(section) {}

Settings::Settings(const std::string &pathToFile, std::string section) :
    _section(section)
{
    loadFromFile(pathToFile);
}

Settings::Settings(std::istream &stream, std::string section) :
    _section(section)
{
    loadFromInputStream(stream);
}

void Settings::loadFromFile(const std::string &pathToFile)
{
    if (fs::exists(pathToFile))
    {
        std::ifstream f(pathToOsuFile);
        loadFromInputStream(f);
    }
    else
    {
        std::cerr << "[Settings] Cannot find " << pathToFile << "!\n";
    }
}

void Settings::loadFromInputStream(std::istream &stream)
{
    std::cout << "[Settings] Loading settings section " << _section << '\n';
    std::string s;
    do
    {
        std::getline(stream, s);
    } while (stream && (s != _section));
    while (stream && (stream.peek() != '['))
    {
        std::getline(stream, s);
        if (!s.empty()) // don't handle empty lines
        {
            std::istringstream is(s);
            is.imbue(std::locale(is.getloc(), new ColonAsWhitespace()));
            std::string key, value;
            is >> key >> std::ws;
            std::getline(is, value);
            _valueMap[key] = value;
        }
    }
}

const std::string &Settings::operator[](const std::string &s) const
{
    return _valueMap.at(s);
}