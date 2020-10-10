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
        _maniaSettings.emplace_back(skinIni, "[Mania]");
        std::cout << "[Skin::Mania] Loaded settings for "
                  << _maniaSettings.back()["Keys"] << "k\n";
    }
}

const Settings &Skin::getGeneralSettings() const { return _general; }
const Settings &Skin::getColorSettings() const { return _colors; }
const Settings &Skin::getFontSettings() const { return _fonts; }
const Settings &Skin::getManiaSettings(int keys) const
{
    return *std::find_if(_maniaSettings.begin(), _maniaSettings.end(),
        [keys](const Settings &a) { return keys == std::stoi(a["Keys"]); });
}