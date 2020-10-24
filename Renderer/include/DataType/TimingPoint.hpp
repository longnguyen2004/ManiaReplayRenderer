#ifndef RENDERERLIB_DATATYPE_TIMINGPOINT_HPP
#define RENDERERLIB_DATATYPE_TIMINGPOINT_HPP

#include "SampleSet.hpp"
#include <cstdint>
#include <optional>
#include <string>

class TimingPoint
{
public:
    TimingPoint();
    TimingPoint(const std::string &data); // read-only for now
    ~TimingPoint() = default;
    std::uint64_t getOffset() const;
    std::optional<double> getBPM() const;
    std::optional<double> getSV() const;
    SampleSet getSampleSet() const;
    unsigned int getSampleIndex() const;
    unsigned int getVolume() const;
    bool isUninherited() const;
    bool isKiai() const;
    bool isOmitBarline() const;

private:
    enum class BitmaskEnum
    {
        KIAI = 1,
        OMIT_BARLINE = 1 << 2
    };
    std::uint64_t _offset;
    double _beatLength;
    unsigned int _meter;
    SampleSet _sampleSet;
    unsigned int _sampleIndex;
    unsigned int _volume;
    bool _uninherited;
    std::uint16_t _effects;
};

#endif