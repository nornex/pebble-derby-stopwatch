//
// Created by matt on 30/11/15.
//

#ifndef PEBBLE_CPP_GRAPHICS_HPP
#define PEBBLE_CPP_GRAPHICS_HPP

#include "layer.hpp"
#include "util/fixed_string.hpp"

namespace pebble
{
    enum class Overflow : util::IntOfSameSize< ::GTextOverflowMode >::Type
    {
        Fill                = GTextOverflowModeFill,
        TrailingEllipsis    = GTextOverflowModeTrailingEllipsis,
        WordWrap            = GTextOverflowModeWordWrap
    };

    class Graphics
    {
    public:
        Graphics(::GContext* context) : context_(context) {}

        void DrawRectangleBorder(Rect rect, Color color)
        {
            ::graphics_context_set_stroke_color(context_, color.sdk_value());
            ::graphics_draw_rect(context_, rect.sdk_value());
        }

        void DrawRectangleFilled(Rect rect, Color color)
        {
            ::graphics_context_set_fill_color(context_, color.sdk_value());
            ::graphics_fill_rect(context_, rect.sdk_value(), 0, GCornerNone);
        }

        template <class TString>
        void DrawText(
            const TString& string, Rect rect, Font font, Color color,
            Alignment alignment = Alignment::Left, Overflow overflow = Overflow::Fill)
        {
            ::graphics_draw_text(
                context_,
                util::convert::CString(string),
                font.sdk_reference(),
                rect.sdk_value(),
                static_cast< ::GTextOverflowMode >(overflow),
                static_cast< ::GTextAlignment >(alignment),
                nullptr
            );
        }

    private:
        ::GContext* context_;
    };

    template<class TController>
    class GraphicsLayer : public LayerBase<GraphicsLayer<TController>>
    {
    public:
        GraphicsLayer(
            TController& controller,
            int16_t x, int16_t y, int16_t width, int16_t height)
        :
            LayerBase<GraphicsLayer<TController>>(x, y, width, height),
            layer_(::layer_create_with_data(this->bounds_, sizeof(TController*)))
        {

            auto** data = static_cast<TController**>(::layer_get_data(layer_));
            *data = &controller;

            ::layer_set_update_proc(layer_, Update);
            ::layer_mark_dirty(layer_);
        }

        GraphicsLayer(TController& controller,Rect rect)
        :
            GraphicsLayer(controller, rect.x(), rect.y(), rect.width(), rect.height())
        {
        }

        void ReDraw()
        {
            ::layer_mark_dirty(layer_);
        }

        ::Layer* sdk_layer() { return layer_; }

    private:
        static void Update(::Layer* layer, ::GContext* ctx)
        {
            auto& controller(controller_from_layer(layer));
            Graphics graphics(ctx);
            controller.OnGraphicsUpdate(graphics);
        }

        static TController& controller_from_layer(::Layer* sdk_layer)
        {
            return **static_cast<TController**>(::layer_get_data(sdk_layer));
        }

        ::Layer* layer_;
    };
}

#endif //PEBBLE_CPP_GRAPHICS_HPP
