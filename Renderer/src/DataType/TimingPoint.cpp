#include "TimingPoint.hpp"
#include "../InputFacet/InputFacet.hpp"

#include <sstream>
#include <type_traits>

TimingPoint::TimingPoint() {}

TimingPoint::TimingPoint(const std::string &data)
{
    std::istringstream is(data);
    is.imbue(std::locale(std::locale::classic(), new CommaAsSeparator()));
    is >> _offset >> _beatLength >> _meter;
    // ugly conversion from SampleSet to int, but this is the cleanest
    is >> reinterpret_cast<std::underlying_type_t<SampleSet> &>(_sampleSet);
    is >> _sampleIndex;
    is >> _volume >> _uninherited >> _effects;
}

std::uint64_t TimingPoint::getOffset() const { return _offset; }

std::optional<double> TimingPoint::getBPM() const
{
    return _uninherited ? 60000.0 / _beatLength : std::optional<double>{};
}

std::optional<double> TimingPoint::getSV() const
{
    return !_uninherited ? -100 / _beatLength : std::optional<double>{};
}

SampleSet TimingPoint::getSampleSet() const { return _sampleSet; }
unsigned int TimingPoint::getSampleIndex() const { return _sampleIndex; }
bool TimingPoint::isUninherited() const { return _uninherited; }
bool TimingPoint::isKiai() const
{
    return _effects & static_cast<std::uint16_t>(BitmaskEnum::KIAI);
}
bool TimingPoint::isOmitBarline() const
{
    return _effects & static_cast<std::uint16_t>(BitmaskEnum::OMIT_BARLINE);
}

bool TimingPointCompOffset(const TimingPoint &lhs, const TimingPoint &rhs)
{
    // Always put red lines before green lines
    return lhs._offset < rhs._offset ||
           lhs._offset == rhs._offset && lhs._uninherited && !rhs._uninherited;
}