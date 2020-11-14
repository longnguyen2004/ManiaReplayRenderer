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

fs::path Skin::getPathToElement(
    const std::string &name, const std::string &suffix, unsigned int keys) const
{
    static std::list<std::string> exts{".png", ".jpg"};
    static std::list<std::string> res{"@2x", ""};
    static std::unordered_map<std::string, std::string>
        elementNameTosettingName // Mappings from file names to skin.ini settings
        {
            // clang-format off
            // Keys and notes
            { "mania-key" , "KeyImage"  },
            { "mania-note", "NoteImage" },

            // Stage elements
            { "mania-stage-left"  , "StageLeft"   },
            { "mania-stage-right" , "StageRight"  },
            { "mania-stage-bottom", "StageBottom" },
            { "mania-stage-hint"  , "StageHint"   },
            { "mania-stage-light" , "StageLight"  },

            // Lighting
            { "lighting", "Lighting" },

            // Hit images
            { "mania-hit", "Hit" }
            // clang-format on
        };
    std::string baseName;
    if (!keys)
    {
        if (auto it = elementNameTosettingName.find(name + suffix);
            it != elementNameTosettingName.end())
        {
            baseName = getManiaSettings(keys)[it->second];
        }
    }
    if (baseName.empty())
    {
        baseName = name + suffix;
    }
    for (auto &i : res)
        for (auto &ext : exts)
        {
            std::string file_name(baseName + i + ext);
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