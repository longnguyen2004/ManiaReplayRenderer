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
    _general.loadFromInputStream(skinIni);
    _colors.loadFromInputStream(skinIni);
    _fonts.loadFromInputStream(skinIni);
}