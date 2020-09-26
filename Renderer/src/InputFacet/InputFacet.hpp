#ifndef RENDERERLIB_INPUTFACET_HPP
#define RENDERERLIB_INPUTFACET_HPP

#include <locale>
#include <vector>

struct ColonAsWhitespace : public std::ctype<char>
{
    ColonAsWhitespace() : std::ctype<char>(get_table()){};
    static mask const *get_table()
    {
        static std::vector<mask> v(
            classic_table(), classic_table() + table_size);
        v[':'] = std::ctype_base::space;
        return &v[0];
    }
};

#endif