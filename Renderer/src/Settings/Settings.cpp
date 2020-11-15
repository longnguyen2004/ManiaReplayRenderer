#include "Settings.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

Settings::Settings(std::string section) : _section(section) {}

Settings::Settings(const std::string &pathToFile, std::string section) :
    _section(section)
{
    loadFromFile(pathToFile);
}

Settings::Settings(std::istream &stream, std::string section) : _section(section)
{
    loadFromInputStream(stream);
}

void Settings::loadFromFile(const std::string &pathToFile)
{
    if (fs::exists(pathToFile))
    {
        std::ifstream f(pathToFile);
        loadFromInputStream(f);
    }
    else
    {
        std::cerr << "[Settings] Cannot find " << pathToFile << "!\n";
    }
}

void Settings::loadFromInputStream(std::istream &stream)
{
    static std::regex match_regex(R"((\w+) ?: ?(.+))");
    _valueMap.clear();
    std::string s;
    do
    {
        std::getline(stream, s);
    } while (stream && (s != _section));
    while (stream && (stream.peek() != '['))
    {
        std::getline(stream, s);
        // don't handle empty lines and comments
        if (auto pos = s.find("//"); pos != std::string::npos)
            s.resize(pos);
        if (std::smatch result;
            !s.empty() && std::regex_match(s, result, match_regex))
        {
            _valueMap[result[1]] = result[2];
        }
    }
}

std::string Settings::operator[](const std::string &s) const
{
    if (auto it = _valueMap.find(s); it != _valueMap.end())
        return it->second;
    else
        return "";
}