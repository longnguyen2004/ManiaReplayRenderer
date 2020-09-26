#ifndef RENDERERLIB_ANSIESCAPE_COLOR_HPP
#define RENDERERLIB_ANSIESCAPE_COLOR_HPP

#include <ostream>

namespace Color
{
std::ostream &Error(std::ostream &stream) { return stream << "\033[31m"; }
std::ostream &Reset(std::ostream &stream) { return stream << "\033[m"; }
} // namespace Color

#endif