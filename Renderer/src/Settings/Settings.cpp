#include "Settings.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../InputFacet/InputFacet.hpp"

Settings::Settings(std::string section) : _section(section)
{
}

Settings::Settings(const std::string &pathToOsuFile, std::string section) :
    _section(section)
{
    loadFromOsuFile(pathToOsuFile);
}

Settings::Settings(std::istream &stream, std::string section) :
    _section(section)
{
    loadFromInputStream(stream);
}

void Settings::loadFromOsuFile(const std::string &pathToOsuFile)
{
    std::ifstream f(pathToOsuFile);
    loadFromInputStream(f);
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

const std::string& Settings::operator[](const std::string& s) const
{
    return _valueMap.at(s);
}