#ifndef RENDERERLIB_ANSIESCAPE_COLOR_HPP
#define RENDERERLIB_ANSIESCAPE_COLOR_HPP

#include <ostream>

namespace Color
{
std::ostream &Error(std::ostream &stream);
std::ostream &Reset(std::ostream &stream);
} // namespace Color

#endif