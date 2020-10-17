#include "InputFacet.hpp"

using mask = std::ctype<char>::mask;

CommaAsSeparator::CommaAsSeparator(std::size_t refs) :
    std::ctype<char>(make_table(), false, refs)
{
}

const mask *CommaAsSeparator::make_table()
{
    static std::vector<mask> v(classic_table(), classic_table() + table_size);
    v[','] |= space;
    return v.data();
}