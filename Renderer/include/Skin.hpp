#ifndef RENDERERLIB_SKIN_HPP
#define RENDERERLIB_SKIN_HPP

#include "Export.hpp"
#include "Settings.hpp"
#include <filesystem>
#include <list>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

class RENDERERLIB_EXPORT Skin
{
public:
    Skin();
    Skin(const std::string &pathToSkinFolder);
    ~Skin() = default;

    void load(const std::string &pathToSkinFolder);
    fs::path getSkinFolder() const;
    fs::path getPathToElement(const std::string &name,
        const std::string &suffix = "",
        unsigned int keys = 0) const;

    const Settings &getGeneralSettings() const;
    const Settings &getColorSettings() const;
    const Settings &getFontSettings() const;

    const Settings &getManiaSettings(unsigned int keys) const;

private:
    Settings _general, _colors, _fonts;
    std::unordered_map<unsigned int, Settings> _maniaSettings;
    fs::path _skinFolder;
    void loadSkinSettings(const std::string &pathToSkinIni);
};

#endif