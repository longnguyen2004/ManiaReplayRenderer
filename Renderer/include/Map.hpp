#ifndef RENDERERLIB_MAP_HPP
#define RENDERERLIB_MAP_HPP

#include <fstream>
#include <memory>
#include <set>
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
 *
 * Getter:
 * const std::string &getMapDirectory()
 * const std::string &getBGFilename()
 * Settings getGeneralSettings()
 * Settings getMetadata()
 * Settings getDifficultySettings()
 * const TimingPointSet &getTimingPoints()
 * double getBaseBPM()
 */

class RENDERERLIB_EXPORT Map
{
public:
    using TimingPointSet = std::set<TimingPoint, decltype(&TimingPointCompOffset)>;

    Map();
    explicit Map(const std::string &pathToOsuFile);
    ~Map() = default;

    void loadFromOsuFile(const std::string &pathToOsuFile);

    const std::string &getMapDirectory() const;
    const std::string &getBGFilename() const;

    const Settings &getGeneralSetings() const;
    const Settings &getMetadata() const;
    const Settings &getDifficultySettings() const;

    const TimingPointSet &getUninheritedTimingPoints() const;
    const TimingPointSet &getInheritedTimingPoints() const;
    double getBaseBPM() const;

private:
    std::ifstream _filestream;
    Settings _general, _metadata, _difficulty;
    std::string _parentDir, _BGname;
    TimingPointSet _uninheritedPoints;
    TimingPointSet _inheritedPoints;
    double _baseBPM;

    void loadSettings();
    void loadTimingPoints();
};

#endif