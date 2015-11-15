//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_LAYER_HPP
#define PEBBLE_CPP_LAYER_HPP

#include "pebble-sdk.hpp"
#include "rect.hpp"
#include "util/fixed_string.hpp"

namespace pebble
{
    class Font
    {
    public:
        Font(const char* name)
            : font_(::fonts_get_system_font(name))
        {
        }

        ::GFont sdk_reference() { return font_; }

    private:
        ::GFont font_;
    };

    enum class Alignment
    {
        Left =      ::GTextAlignmentLeft,
        Right =     ::GTextAlignmentRight,
        Center =    ::GTextAlignmentCenter
    };

    class TextLayer : public Rect
    {
    public:
        TextLayer(int16_t x, int16_t y, int16_t width, int16_t height)
        :
            Rect(x, y, width, height),
            layer_(::text_layer_create(bounds_))
        {
        }

        void set_background_colour(GColor color)
        {
            ::text_layer_set_background_color(layer_, color);
        }

        void set_text_colour(GColor color)
        {
            ::text_layer_set_text_color(layer_, color);
        }

        template <class TString>
        void set_text(const TString& text)
        {
            ::text_layer_set_text(layer_, util::convert::CString(text));
        }

        void set_font(Font font)
        {
            ::text_layer_set_font(layer_, font.sdk_reference());
        }

        void set_alignment(Alignment alignment)
        {
            ::text_layer_set_text_alignment(layer_, static_cast<GTextAlignment>(alignment));
        }

        ~TextLayer()
        {
            ::text_layer_destroy(layer_);
        }

        ::TextLayer* sdk_handle() { return layer_; }

    private:
        ::TextLayer* layer_;
    };

    class Layer : public Rect
    {
    public:
        Layer(::Layer* layer)
        :
            Rect(::layer_get_bounds(layer)),
            layer_(layer)
        {
        }

        void AddChild(TextLayer& text_layer)
        {
            layer_add_child(layer_, text_layer_get_layer(text_layer.sdk_handle()));
        }

    private:
        ::Layer* layer_;
    };

}

#endif //PEBBLE_CPP_LAYER_HPP
