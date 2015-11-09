
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
            ::fonts_get_system_font(font_key);
        }

        //// Layers ////
        typedef ::Layer Layer;

        FORCE_INLINE Layer * layer_create(GRect frame)
        {
            return ::layer_create(frame);
        }

        typedef ::TextLayer TextLayer;

        FORCE_INLINE TextLayer* text_layer_create(GRect frame)
        {
            return ::text_layer_create(frame);
        }

        FORCE_INLINE void text_layer_destroy(TextLayer* text_layer)
        {
            ::text_layer_destroy(text_layer);
        }

        FORCE_INLINE void layer_add_child(Layer *parent, Layer *child)
        {
            return ::layer_add_child(parent, child);
        }

    }
}

#undef FORCE_INLINE

#endif // PEBBLE_API_WRAPPER_HPP_