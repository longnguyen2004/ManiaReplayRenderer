#ifndef RENDERERLIB_SKIN_HPP
#define RENDERERLIB_SKIN_HPP

#include "Export.hpp"
#include "Settings.hpp"
#include <string>

class RENDERERLIB_EXPORT Skin
{
public:
    Skin();
    Skin(const std::string &pathToSkinFolder);
    ~Skin() = default;

    void load(const std::string &pathToSkinFolder);
    const std::string &getSkinFolder() const;

private:
    Settings _general, _colors, _fonts;
    std::string _skinFolder;
    void loadSkinSettings(const std::string &pathToSkinIni);
};

#endif