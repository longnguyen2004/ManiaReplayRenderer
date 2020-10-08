#include "Skin.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../ANSIEscape/Color.hpp"

namespace fs = std::filesystem;

Skin::Skin() : _general("[General]"), _colors("[Colours]"), _fonts("[Fonts]") {}

Skin::Skin(const std::string &pathToSkinFolder) :
    _general("[General]"), _colors("[Colours]"), _fonts("[Fonts]")
{
    load(pathToSkinFolder);
}

void Skin::load(const std::string &pathToSkinFolder)
{
    std::cout << "[Skin] Loading skin folder " << pathToSkinFolder << '\n';
    fs::path skinFolder(pathToSkinFolder);
    if (!fs::exists(skinFolder))
    {
        std::cerr << Color::Error << "[Skin] Skin folder doesn't exist!"
                  << Color::Reset;
        return;
    }
    if (auto skinIni = skinFolder / "skin.ini"; fs::exists(skinIni))
    {
        loadSkinSettings(skinIni.generic_string());
    }
    else
    {
        std::cerr << Color::Error << "[Skin] Cannot find skin.ini!\n"
                  << Color::Reset;
    }
}

void Skin::loadSkinSettings(const std::string &pathToSkinIni)
{
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