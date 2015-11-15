//
// Created by matt on 15/11/15.
//

#ifndef PEBBLE_CPP_TYPES_HPP
#define PEBBLE_CPP_TYPES_HPP

#include "../pebble-sdk.hpp"

namespace util
{
    template <size_t TSize>
    struct IntOfSize
    {
    };

    template <> struct IntOfSize<1> { typedef int8_t Type; };
    template <> struct IntOfSize<2> { typedef int16_t Type; };
    template <> struct IntOfSize<4> { typedef int32_t Type; };
    template <> struct IntOfSize<8> { typedef int64_t Type; };

    template <class T>
    struct IntOfSameSize
    {
        typedef typename  IntOfSize<sizeof(T)>::Type Type;
    };
}

#endif //PEBBLE_CPP_TYPES_HPP
