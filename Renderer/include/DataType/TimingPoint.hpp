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
    std::int64_t getOffset() const;
    std::optional<double> getBPM() const;
    std::optional<double> getSV() const;
    unsigned int getMeter() const;
    SampleSet getSampleSet() const;
    unsigned int getSampleIndex() const;
    unsigned int getVolume() const;
    bool isUninherited() const;
    bool isKiai() const;
    bool isOmitBarline() const;

    friend bool TimingPointCompOffset(
        const TimingPoint &lhs, const TimingPoint &rhs);

private:
    enum class BitmaskEnum
    {
        KIAI = 1,
        OMIT_BARLINE = 1 << 2
    };
    std::int64_t _offset;
    double _beatLength;
    unsigned int _meter;
    SampleSet _sampleSet;
    unsigned int _sampleIndex;
    unsigned int _volume;
    bool _uninherited;
    std::uint16_t _effects;
};

#endif