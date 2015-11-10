
#ifndef PEBBLE_API_WRAPPER_HPP_
#define PEBBLE_API_WRAPPER_HPP_

#define _TIME_H_

extern "C"
{
#include <pebble.h>
}

#define FORCE_INLINE inline

namespace pebble
{
    namespace sdk
    {
        //// Application ////
        FORCE_INLINE void app_event_loop()
        {
            ::app_event_loop();
        }

        //// Windows ////
        typedef ::Window Window;

        FORCE_INLINE Window* window_create()
        {
            return ::window_create();
        }

        FORCE_INLINE void window_destroy(Window* window)
        {
            ::window_destroy(window);
        }

        FORCE_INLINE void window_stack_push(Window* window, bool animated)
        {
            ::window_stack_push(window, animated);
        }

        //// GRect ////
        typedef ::GRect GRect;
        typedef ::GPoint GPoint;
        typedef ::GSize GSize;

        //// GFont ////
        typedef ::GFont GFont;

        FORCE_INLINE GFont fonts_get_system_font(const char *font_key)
        {
            return ::fonts_get_system_font(font_key);
        }

        //// GFont ////
        typedef ::GFont GFont;

        //// GColor ////
        typedef ::GColor GColor;

        //// Layer ////
        typedef ::Layer Layer;

        FORCE_INLINE void layer_add_child(Layer *parent, Layer *child)
        {
            return ::layer_add_child(parent, child);
        }

        FORCE_INLINE Layer * layer_create(GRect frame)
        {
            return ::layer_create(frame);
        }

        FORCE_INLINE GRect layer_get_bounds(const Layer *layer)
        {
            return ::layer_get_bounds(layer);
        }

        //// TextLayer ////
        typedef ::TextLayer TextLayer;

        FORCE_INLINE TextLayer* text_layer_create(GRect frame)
        {
            return ::text_layer_create(frame);
        }

        FORCE_INLINE void text_layer_destroy(TextLayer* text_layer)
        {
            ::text_layer_destroy(text_layer);
        }

        FORCE_INLINE void text_layer_set_background_color(TextLayer *text_layer, GColor color)
        {
            ::text_layer_set_background_color(text_layer, color);
        }

        FORCE_INLINE void text_layer_set_text_color(TextLayer *text_layer, GColor color)
        {
            ::text_layer_set_text_color(text_layer, color);
        }

        FORCE_INLINE void text_layer_set_text(TextLayer *text_layer, const char *text)
        {
            ::text_layer_set_text(text_layer, text);
        }


        FORCE_INLINE void text_layer_set_font(TextLayer *text_layer, GFont font)
        {
            ::text_layer_set_font(text_layer, font);
        }

        FORCE_INLINE void text_layer_set_overflow_mode(TextLayer *text_layer, ::GTextOverflowMode line_mode)
        {
            ::text_layer_set_overflow_mode(text_layer, line_mode);
        }

        FORCE_INLINE void text_layer_set_text_alignment(TextLayer *text_layer, ::GTextAlignment alignment)
        {
            ::text_layer_set_text_alignment(text_layer, alignment);
        }

    }
}

#undef FORCE_INLINE

#endif // PEBBLE_API_WRAPPER_HPP_