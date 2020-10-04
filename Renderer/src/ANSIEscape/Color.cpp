#include "Color.hpp"

std::ostream &Color::Error(std::ostream &stream)
{
    return stream << "\033[31m";
}
std::ostream &Color::Reset(std::ostream &stream)
{
    return stream << "\033[m";
}