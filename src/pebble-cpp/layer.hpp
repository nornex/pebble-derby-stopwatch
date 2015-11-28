//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_LAYER_HPP
#define PEBBLE_CPP_LAYER_HPP

#include "pebble-sdk.hpp"
#include "rect.hpp"
#include "font.hpp"
#include "color.hpp"
#include "util/fixed_string.hpp"
#include "util/types.hpp"

namespace pebble
{
    enum class Alignment : util::IntOfSameSize< ::GTextAlignment >::Type
    {
        Left =      ::GTextAlignmentLeft,
        Right =     ::GTextAlignmentRight,
        Center =    ::GTextAlignmentCenter
    };

    enum class Alignment2d : util::IntOfSameSize< ::GAlign >::Type
    {
        /// Align by centering.
        Center      = ::GAlignCenter,

        /// Align by making the top edges overlap and left edges overlap.
        TopLeft     = ::GAlignTopLeft,

        /// Align by making the top edges overlap and right edges overlap.
        TopRight    = ::GAlignTopRight,

        /// Align by making the top edges overlap and centered horizontally.
        Top         = ::GAlignTop,

        /// Align by making the left edges overlap and centered vertically.
        Left        = ::GAlignLeft,

        /// Align by making the bottom edges overlap and centered horizontally.
        Bottom      = ::GAlignBottom,

        // Align by making the right edges overlap and centered vertically.
        Right       = ::GAlignRight,

        /// Align by making the bottom edges overlap and right edges overlap.
        BottomRight = ::GAlignBottomRight,

        /// Align by making the bottom edges overlap and left edges overlap.
        BottomLeft  = ::GAlignBottomLeft
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

        void SetBackgroundColor(Color color)
        {
            ::text_layer_set_background_color(layer_, color.sdk_value());
        }

        void SetTextColor(Color color)
        {
            ::text_layer_set_text_color(layer_, color.sdk_value());
        }

        template <class TString>
        void SetText(const TString& text)
        {
            ::text_layer_set_text(layer_, util::convert::CString(text));
        }

        void SetFont(Font font)
        {
            ::text_layer_set_font(layer_, font.sdk_reference());
        }

        void SetAlignment(Alignment alignment)
        {
            ::text_layer_set_text_alignment(layer_, static_cast< ::GTextAlignment >(alignment));
        }

        ~TextLayer()
        {
            ::text_layer_destroy(layer_);
        }

        ::TextLayer* sdk_handle() { return layer_; }

    private:
        ::TextLayer* layer_;
    };

    class BitmapLayer : public Rect
    {
    public:
        BitmapLayer(int16_t x, int16_t y, int16_t width, int16_t height)
        :
            Rect(x, y, width, height),
            layer_(::bitmap_layer_create(bounds_))
        {
        }

        void SetBitmap(const ::GBitmap* bitmap)
        {
            ::bitmap_layer_set_bitmap(layer_, bitmap);
        }

        void SetAlignment(Alignment2d alignment)
        {
            ::bitmap_layer_set_alignment(layer_, static_cast< ::GAlign >(alignment));
        }

        void SetBackgroundColor(Color color)
        {
            ::bitmap_layer_set_background_color(layer_, color.sdk_value());
        }

        ~BitmapLayer()
        {
            ::bitmap_layer_destroy(layer_);
        }
    private:
        ::BitmapLayer* layer_;
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
