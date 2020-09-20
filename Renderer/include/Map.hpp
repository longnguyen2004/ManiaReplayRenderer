#ifndef RENDERERLIB_MAP_HPP
#define RENDERERLIB_MAP_HPP

#include <memory>
#include <string>

#include "Settings.hpp"

/* Class structure:
 *
 * Constructors and destructors:
 * Map()                   : default constructor
 * Map(const std::string&) : load from file
 * Map(std::istream&)      : load from stream
 * ~Map()                  : destructor
 * 
 * Load functions:
 * loadFromOsuFile(const std::string&) : load from file, called by ctor
 * loadFromInputStream(std::istream&)  : load from stream, called by ctor
 * 
 * Getter:
 * Settings getGeneralSettings()
 * Settings getMetadata()
 * Settings getDifficultySettings()
 */

class RENDERERLIB_EXPORT Map
{
public:
    Map();
    explicit Map(const std::string &pathToOsuFile);
    explicit Map(std::istream &stream);
    ~Map() = default;

    void loadFromOsuFile(const std::string &pathToOsuFile);
    void loadFromInputStream(std::istream &stream);
    
    const std::string& getMapDirectory() const;
    const std::string& getBGFilename() const;

    const Settings& getGeneralSetings() const;
    const Settings& getMetadata() const;
    const Settings& getDifficultySettings() const;

private:
    Settings _general, _metadata, _difficulty;
    std::string _parentDir, _BGname;
};

#undef make_ctor_dtor_loadfunc
#endif