#include "Object.hpp"
#include "../InputFacet/InputFacet.hpp"
#include <cmath>
#include <sstream>

Object::Object(const std::string &data, unsigned int columnCount)
{
    std::istringstream is(data);
    is.imbue(std::locale(std::locale::classic(), new CommaAsSeparator));
    unsigned int type;
    float x, y;
    is >> x >> y >> _startTime >> type; // we don't use y
    is >> reinterpret_cast<unsigned int &>(_hitSound);
    _column = std::floor(x * columnCount / 512);
    _isLN = type & 1 << 7;
    if (_isLN)
        is >> _endTime;
    else
        _endTime = 0;
}

unsigned int Object::getColumn() const { return _column; }
std::int64_t Object::getStartTime() const { return _startTime; }
bool Object::isLN() const { return _isLN; }
std::byte Object::getHitSoundMask() const { return _hitSound; }
std::int64_t Object::getEndTime() const { return _endTime; }

bool ObjectCompOffset(const Object &a, const Object &b)
{
    return a._startTime < b._startTime;
}