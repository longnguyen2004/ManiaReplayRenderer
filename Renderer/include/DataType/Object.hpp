#ifndef RENDERERLIB_DATATYPE_OBJECT_HPP
#define RENDERERLIB_DATATYPE_OBJECT_HPP

#include <cstddef>
#include <cstdint>
#include <string>

class Object
{
public:
    Object(const std::string &data, unsigned int columnCount);
    ~Object() = default;
    unsigned int getColumn() const;
    std::int64_t getStartTime() const;
    bool isLN() const;
    std::byte getHitSoundMask() const;
    std::int64_t getEndTime() const;

    friend bool ObjectCompOffset(const Object &a, const Object &b);

private:
    unsigned int _column;
    std::int64_t _startTime;
    bool _isLN;
    std::byte _hitSound;
    std::int64_t _endTime;
};

#endif