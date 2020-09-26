#ifndef RENDERERLIB_SETTINGS_HPP
#define RENDERERLIB_SETTINGS_HPP

#include "Export.hpp"
#include <string>
#include <unordered_map>

/* Class structure:
 * Ctor and Dtor:
 * Settings(std::string section)                     : default constructor
 * Settings(std::string section, const std::string&) : load from file
 * Settings(std::string section, std::istream&)      : load from stream
 * ~Settings()                                       : destructor
 *
 * Load functions:
 * loadFromOsuFile(const std::string&) : load from file, called by ctor
 * loadFromInputStream(std::istream&)  : load from stream, called by ctor
 *
 * WARNING: loadFromInputStream does not rewind to the start of the stream!
 * This is to facilitate the use of this class in the Map class, which will
 * call loadFromInputStream 3 times to get 3 different sections, and we don't
 * want to rewind every time
 *
 * Getter:
 * const std::string& operator[](const std::string&) : get a value of a setting
 */

class RENDERERLIB_EXPORT Settings
{
public:
    Settings(std::string section = "");
    explicit Settings(const std::string &pathToFile, std::string section);
    explicit Settings(std::istream &stream, std::string section);
    ~Settings() = default;

    void loadFromFile(const std::string &pathToFile);
    void loadFromInputStream(std::istream &stream);

    const std::string &operator[](const std::string &s) const;

private:
    /* Slight rant:
     * I know that osu! settings does have integers, string, delimited integers
     * and strings, but parsing the settings alone doesn't allow you to know
     * which setting is which. For the sake of simplicity and to avoid
     * hardcoding types, I end up making the value type std::string, and rely on
     * the user to convert it to the corresponding type
     */
    std::unordered_map<std::string, std::string> _valueMap;
    std::string _section;
};

#endif