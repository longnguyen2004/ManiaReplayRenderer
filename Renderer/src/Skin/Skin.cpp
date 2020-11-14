#include "Skin.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>

#include "../ANSIEscape/Color.hpp"

using namespace std::literals;

Skin::Skin() : _general("[General]"), _colors("[Colours]"), _fonts("[Fonts]") {}

Skin::Skin(const std::string &pathToSkinFolder) :
    _general("[General]"), _colors("[Colours]"), _fonts("[Fonts]")
{
    load(pathToSkinFolder);
}

void Skin::load(const std::string &pathToSkinFolder)
{
    std::cout << "[Skin] Loading skin folder " << pathToSkinFolder << '\n';
    _skinFolder = pathToSkinFolder;
    if (!fs::exists(_skinFolder))
    {
        std::cerr << Color::Error << "[Skin] Skin folder doesn't exist!"
                  << Color::Reset;
        return;
    }
    if (auto skinIni = _skinFolder / "skin.ini"; fs::exists(skinIni))
    {
        loadSkinSettings(skinIni.generic_string());
    }
    else
    {
        std::cerr << Color::Error << "[Skin] Cannot find skin.ini!\n"
                  << Color::Reset;
    }
}

fs::path Skin::getSkinFolder() const { return _skinFolder; }

fs::path Skin::getPathToElement(const std::string &name) const
{
    static std::list<std::string> exts{".png"s, ".jpg"s};
    static std::list<std::string> res{"@2x", ""};
    for (auto &i : res)
        for (auto &ext : exts)
        {
            std::string file_name(name + i + ext);
            fs::path elem_path(_skinFolder);
            elem_path /= file_name;
            if (fs::exists(elem_path))
                return elem_path;
        }
    return {};
}

void Skin::loadSkinSettings(const std::string &pathToSkinIni)
{
    _maniaSettings.clear();
    std::cout << "[Skin] Loading skin.ini\n";
    std::ifstream skinIni(pathToSkinIni);
    std::cout << "[Skin] Loading general settings\n";
    _general.loadFromInputStream(skinIni);
    std::cout << "[Skin] Loading color settings\n";
    _colors.loadFromInputStream(skinIni);
    std::cout << "[Skin] Loading font settings\n";
    _fonts.loadFromInputStream(skinIni);
    while (skinIni)
    {
        Settings currentSetting(skinIni, "[Mania]");
        unsigned int keys = std::stoi(currentSetting["Keys"]);
        std::cout << "[Skin::Mania] Loaded settings for " << keys << "k\n";
        _maniaSettings[keys] = std::move(currentSetting);
    }
}

const Settings &Skin::getGeneralSettings() const { return _general; }
const Settings &Skin::getColorSettings() const { return _colors; }
const Settings &Skin::getFontSettings() const { return _fonts; }
const Settings &Skin::getManiaSettings(unsigned int keys) const
{
    if (auto it = _maniaSettings.find(keys); it != _maniaSettings.end())
        return it->second;
    else
        return {};
}