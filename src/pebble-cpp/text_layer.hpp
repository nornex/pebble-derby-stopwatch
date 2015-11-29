//
// Created by matt on 29/11/15.
//

#ifndef PEBBLE_CPP_TEXT_LAYER_HPP
#define PEBBLE_CPP_TEXT_LAYER_HPP

#include "layer.hpp"
#include "util/fixed_string.hpp"

namespace pebble
{
    class TextLayerBase : public Rect
    {
    protected:
        TextLayerBase(int16_t x, int16_t y, int16_t width, int16_t height)
            :
            Rect(x, y, width, height),
            layer_(::text_layer_create(bounds_))
        {
        }

    public:
        ~TextLayerBase()
        {
            ::text_layer_destroy(layer_);
        }

        void SetBackgroundColor(Color color)
        {
            ::text_layer_set_background_color(layer_, color.sdk_value());
        }

        void SetTextColor(Color color)
        {
            ::text_layer_set_text_color(layer_, color.sdk_value());
        }

        void SetFont(Font font)
        {
            ::text_layer_set_font(layer_, font.sdk_reference());
        }

        void SetAlignment(Alignment alignment)
        {
            ::text_layer_set_text_alignment(layer_, static_cast< ::GTextAlignment >(alignment));
        }

        ::TextLayer* sdk_handle() { return layer_; }

        ::Layer* sdk_layer() { return ::text_layer_get_layer(sdk_handle()); }

    protected:
        template <class TString>
        void SetTextRaw(const TString& text)
        {
            ::text_layer_set_text(layer_, util::convert::CString(text));
        }

    private:
        ::TextLayer* layer_;
    };

    class TextLayer : public TextLayerBase
    {
    public:
        TextLayer(int16_t x, int16_t y, int16_t width, int16_t height)
            :
            TextLayerBase(x, y, width, height)
        {
        }

        template <class TString>
        void SetText(const TString& text)
        {
            SetTextRaw(text);
        }
    };

    template<uint16_t TSize>
    class TextLayerWithBuffer : public TextLayerBase
    {
    public:
        TextLayerWithBuffer(int16_t x, int16_t y, int16_t width, int16_t height)
            :
            TextLayerBase(x, y, width, height)
        {
        }

        template<class TString>
        void SetText(const TString& text)
        {
            SetTextRaw(buffer_.Set(text));
        }

        template<class... TParams>
        inline
        void SetFromFormat(const char* format, TParams... params)
        {
            SetTextRaw(buffer_.SetFromFormat(format, params...));
        }

        template<class TWrapper>
        inline
        void SetFromTime(const char* format, const util::CalendarTimeBase<TWrapper>& time)
        {
            SetTextRaw(buffer_.SetFromFormat(format, time));
        }

        const util::FixedString<TSize>& string() const { return buffer_; }

    private:
        util::FixedString<TSize> buffer_;

    };
}

#endif //PEBBLE_CPP_TEXT_LAYER_HPP
