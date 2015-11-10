//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_LAYER_HPP
#define PEBBLE_CPP_LAYER_HPP

#include "pebble-sdk.hpp"
#include "rect.hpp"

namespace pebble
{
    class Font
    {
    public:
        Font(const char* name)
            : font_(sdk::fonts_get_system_font(name))
        {
        }

        sdk::GFont sdk_reference() { return font_; }

    private:
        sdk::GFont font_;
    };

    enum class Alignment
    {
        Left = GTextAlignmentLeft,
        Right = GTextAlignmentRight,
        Center = GTextAlignmentCenter
    };

    class TextLayer : public Rect
    {
    public:
        TextLayer(int16_t x, int16_t y, int16_t width, int16_t height)
        :
            Rect(x, y, width, height),
            layer_(sdk::text_layer_create(bounds_))
        {
        }

        void set_background_colour(GColor color)
        {
            sdk::text_layer_set_background_color(layer_, color);
        }

        void set_text_colour(GColor color)
        {
            sdk::text_layer_set_text_color(layer_, color);
        }

        void set_text(const char* text)
        {
            sdk::text_layer_set_text(layer_, text);
        }

        void set_font(Font font)
        {
            sdk::text_layer_set_font(layer_, font.sdk_reference());
        }

        void set_alignment(Alignment alignment)
        {
            sdk::text_layer_set_text_alignment(layer_, static_cast<GTextAlignment>(alignment));
        }

        ~TextLayer()
        {
            sdk::text_layer_destroy(layer_);
        }

        sdk::TextLayer* sdk_handle() { return layer_; }

    private:
        sdk::TextLayer* layer_;
    };

    class Layer : public Rect
    {
    public:
        Layer(sdk::Layer* layer)
        :
            Rect(sdk::layer_get_bounds(layer)),
            layer_(layer)
        {
        }

        void AddChild(TextLayer& text_layer)
        {
            layer_add_child(layer_, text_layer_get_layer(text_layer.sdk_handle()));
        }

    private:
        sdk::Layer* layer_;
    };

}

#endif //PEBBLE_CPP_LAYER_HPP
