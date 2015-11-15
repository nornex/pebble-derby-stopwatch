//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_WINDOW_INLINE_HPP
#define PEBBLE_CPP_WINDOW_INLINE_HPP

namespace pebble
{
    template <class TEventHandler> inline
    Window<TEventHandler>::Window()
        :
        window_(::window_create()),
        handler_(window_)
    {
        ::window_set_user_data(window_, this);
    }

    template <class TEventHandler> inline
    Window<TEventHandler>::~Window()
    {
        ::window_destroy(window_);
    }

    template <class TEventHandler> inline
    Layer& Window<TEventHandler>::root_layer()
    {
        if (layer_.is_none())
        {
            layer_.Emplace(window_get_root_layer(window_));
        }

        return layer_.unsafe_get();
    }

    template <class TEventHandler> inline
    ::Window* Window<TEventHandler>::sdk_handle()
    {
        return window_;
    }

    template <class TEventHandler> inline
    Window<TEventHandler>& Window<TEventHandler>::FromSdkHandle(::Window* sdk_handle)
    {
        return *static_cast<Window*>(::window_get_user_data(sdk_handle));
    }

    template <class TEventHandler> inline
    Window<TEventHandler>::EventHandler::EventHandler(::Window* window)
    {
        ::window_set_window_handlers(
            window,
            (WindowHandlers) {
                .load =         &Load,
                .appear =       &Appear,
                .disappear =    &Disappear,
                .unload =       &Unload
            }
        );
    }

    template <class TEventHandler> inline
    void Window<TEventHandler>::EventHandler::Load(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().Emplace(window);
    }

    template <class TEventHandler> inline
    void Window<TEventHandler>::EventHandler::Unload(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().Clear();
    }

    template <class TEventHandler> inline
    void Window<TEventHandler>::EventHandler::Appear(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().unsafe_get().OnAppear();
    }

    template <class TEventHandler> inline
    void Window<TEventHandler>::EventHandler::Disappear(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().unsafe_get().OnDisappear();
    }
}

#endif //PEBBLE_CPP_WINDOW_INLINE_HPP
