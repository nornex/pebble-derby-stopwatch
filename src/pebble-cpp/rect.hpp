//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_RECT_HPP
#define PEBBLE_CPP_RECT_HPP

#include "pebble-sdk.hpp"

namespace pebble
{
    class Rect
    {
    public:
        Rect(::GRect bounds)
            : bounds_(bounds)
        {
        }

        Rect(int16_t x, int16_t y, int16_t width, int16_t height)
            :
            bounds_(::GRect{
                .origin = (::GPoint) { .x = x, .y = y },
                .size = (::GSize) { .w = width, .h = height }
            })
        {
        }

        int16_t x() const { return bounds_.origin.x; }
        int16_t y() const { return bounds_.origin.y; }
        int16_t width() const { return bounds_.size.w; }
        int16_t height() const { return bounds_.size.h; }

        const ::GRect& sdk_value() const { return bounds_; }
        ::GRect& sdk_value() { return bounds_; }

    protected:
        ::GRect bounds_;
    };
}

#endif //PEBBLE_CPP_RECT_HPP
