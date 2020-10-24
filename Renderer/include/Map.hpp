#ifndef RENDERERLIB_MAP_HPP
#define RENDERERLIB_MAP_HPP

#include <list>
#include <memory>
#include <string>

#include "DataType/TimingPoint.hpp"
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
 * const std::string &getMapDirectory()
 * const std::string &getBGFilename()
 * Settings getGeneralSettings()
 * Settings getMetadata()
 * Settings getDifficultySettings()
 * const std::list<TimingPoint> &getTimingPoints()
 * double getBaseBPM()
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

    const std::string &getMapDirectory() const;
    const std::string &getBGFilename() const;

    const Settings &getGeneralSetings() const;
    const Settings &getMetadata() const;
    const Settings &getDifficultySettings() const;

    const std::list<TimingPoint> &getTimingPoints() const;
    double getBaseBPM() const;

private:
    Settings _general, _metadata, _difficulty;
    std::string _parentDir, _BGname;
    std::list<TimingPoint> _timingPoints;
    double _baseBPM;

    void loadTimingPoints(std::istream &stream);
};

#endif