#ifndef RENDERERLIB_INPUTFACET_HPP
#define RENDERERLIB_INPUTFACET_HPP

#include <locale>
#include <vector>

class CommaAsSeparator : public std::ctype<char>
{
public:
    CommaAsSeparator(std::size_t refs = 0);
    static const mask *make_table();
};

#endif