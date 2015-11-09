//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_LAYER_HPP
#define PEBBLE_CPP_LAYER_HPP

#include "pebble-sdk.hpp"

namespace pebble
{

    class Rect
    {
    public:
        Rect(sdk::GRect bounds)
            : bounds_(bounds)
        {
        }

        Rect(int16_t x, int16_t y, int16_t width, int16_t height)
        :
            bounds_(sdk::GRect{
                .origin = (sdk::GPoint) { .x = x, .y = y },
                .size = (sdk::GSize) { .w = width, .h = height }
            })
        {
        }

        int16_t x() const { return bounds_.origin.x; }
        int16_t y() const { return bounds_.origin.y; }
        int16_t width() const { return bounds_.size.w; }
        int16_t height() const { return bounds_.size.h; }

        const sdk::GRect& sdk_reference() const { return bounds_; }
        sdk::GRect& sdk_reference() { return bounds_; }

    protected:
        sdk::GRect bounds_;
    };

    class Font
    {
    public:
        Font(const char* name)
            : font_(sdk::fonts_get_system_font(name))
        {
        }

        const sdk::GFont sdk_reference() const { return font_; }

    private:
        sdk::GFont font_;
    };

    enum class Alignment : GTextAlignment
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
            text_layer_set_background_color(layer_, color);
        }

        void set_text_colour(GColor color)
        {
            text_layer_set_text_color(layer_, color);
        }

        void set_text(const char* text)
        {
            text_layer_set_text(layer_, text);
        }

        void set_font(Font font)
        {
            text_layer_set_font(layer_, font.sdk_reference());
        }

        void set_alignment(Alignment alignment)
        {
            text_layer_set_text_alignment(layer_, static_cast<GTextAlignment>(alignment));
        }

        ~TextLayer()
        {
            sdk::text_layer_destroy(layer_);
        }

        sdk::TextLayer* sdk_handle() { return layer_; }

    private:
        sdk::TextLayer* layer_;
    };

    class Layer
    {
    public:
        Layer(sdk::Layer* layer)
        :
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
