//
// Created by matt on 27/11/15.
//

#ifndef PEBBLE_CPP_FONT_HPP
#define PEBBLE_CPP_FONT_HPP

#include "pebble-sdk.hpp"

namespace pebble
{
    class Font
    {
    public:
        Font(const char* name)
            : font_(::fonts_get_system_font(name))
        {
        }

        ::GFont sdk_reference()
        { return font_; }

    private:
        ::GFont font_;
    };
}

#endif //PEBBLE_CPP_FONT_HPP
