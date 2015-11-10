//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_WINDOW_INLINE_HPP
#define PEBBLE_CPP_WINDOW_INLINE_HPP

namespace pebble
{
    inline
    Window::Window(Window &&window)
    :
        window_(window.sdk_handle()),
        owned_(window.owned_),
        layer_(std::move(window.layer_)),
        handler_(window.handler_)
    {
        window.owned_ = false;
    }

    inline
    Window::~Window()
    {
        if (owned_)
        {
            window_destroy(window_);
        }
    }

    inline
    Layer& Window::root_layer()
    {
        if (layer_.is_none())
        {
            layer_.Emplace(window_get_root_layer(window_));
        }

        return layer_.UnsafeGet();
    }

    inline
    sdk::Window* Window::sdk_handle()
    {
        return window_;
    }

    template <class THandler> inline
    Window Window::CreateWithHandler()
    {
        Window window;
        Handler<THandler>::ForWindow(window);
        return window;
    }

    inline
    Window:: Window()
    :
        window_(sdk::window_create()),
        owned_(true)
    {
    }

    inline
    Window::Window(sdk::Window* window)
    :
        window_(window),
        owned_(false)
    {
    }

    template <class THandler> inline
    void Window::Handler<THandler>::ForWindow(Window& window)
    {
        window_set_window_handlers(
            window.sdk_handle(),
            (WindowHandlers) {
                .load =         &Load,
                .appear =       &Appear,
                .disappear =    &Disappear,
                .unload =       &Unload
            }
        );
    }

    template <class THandler> inline
    void Window::Handler<THandler>::Load(sdk::Window* sdk_window)
    {
        auto* window = Application::Base().window_stack().Find(sdk_window);
        if (window != nullptr)
        {
            window->handler_ = static_cast<void *>(new THandler(*window));
        }
    }

    template <class THandler> inline
    void Window::Handler<THandler>::Unload(sdk::Window* sdk_window)
    {
        auto* window = Application::Base().window_stack().Find(sdk_window);
        if (window != nullptr && window->handler_ != nullptr)
        {
            delete static_cast<THandler*>(window->handler_);
            window->handler_ = nullptr;
        }
    }

    template <class THandler> inline
    void Window::Handler<THandler>::Appear(sdk::Window* sdk_window)
    {
        auto* window = Application::Base().window_stack().Find(sdk_window);
        if (window != nullptr && window->handler_ != nullptr)
        {
            static_cast<THandler*>(window->handler_)->WindowAppear();
        }
    }

    template <class THandler> inline
    void Window::Handler<THandler>::Disappear(sdk::Window* sdk_window)
    {
        auto* window = Application::Base().window_stack().Find(sdk_window);
        if (window != nullptr && window->handler_ != nullptr)
        {
            static_cast<THandler*>(window->handler_)->WindowDisappear();
        }
    }
}

#endif //PEBBLE_CPP_WINDOW_INLINE_HPP
