//
// Created by matt on 08/11/15.
//

#ifndef PEBBLE_CPP_WINDOW_HPP
#define PEBBLE_CPP_WINDOW_HPP

#include <functional>

#include "pebble-sdk.hpp"
#include "layer.hpp"

namespace pebble
{
    enum class Animate : bool
    {
        SlideIn = true,
        None = false
    };

    class Window
    {
    private:

        Window()
        :
            window_(sdk::window_create()),
            owned_(true)
        {
        }

        Window(sdk::Window* window)
        :
            window_(window),
            owned_(false)
        {
        }

        //Window(const Window&) = delete;

    public:

        Window(Window&& window)
        :
            window_(window.sdk_handle()),
            owned_(window.owned_),
            layer_(window.layer_)
        {

            window.owned_ = false;
        }

        ~Window()
        {
            if (owned_) window_destroy(window_);
        }

        Layer root_layer()
        {
            if (layer_ == nullptr)
            {
                layer_ = window_get_root_layer(window_);
            }

            return Layer(layer_);
        }

        sdk::Window* sdk_handle() { return window_; }

        template <class THandler>
        static Window CreateWithHandler()
        {
            Window window;
            Handler<THandler>::ForWindow(window);
            return window;
        }

    private:
        sdk::Window* window_;
        bool owned_;

        sdk::Layer* layer_ = nullptr;

        template <class THandler>
        class Handler
        {
            friend class Window;

            static void ForWindow(Window& window)
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

            static void Load(sdk::Window* sdk_window)
            {
                Window window(sdk_window);
                THandler::Load(window);
            }

            static void Unload(sdk::Window* sdk_window)
            {
                Window window(sdk_window);
                THandler::Unload(window);
            }

            static void Appear(sdk::Window* window)
            {
            }

            static void Disappear(sdk::Window* window)
            {
            }
        };
    };

    class WindowStack
    {
    public:
        WindowStack()
        :
            next_(&stack_[0])
        {
        }

        void Push(Window& window, Animate animated)
        {
            if (next_ > stack_ + sizeof(stack_))
            {
                // TODO(matt) Logging
                return;
            }

            window_stack_push(window.sdk_handle(), static_cast<bool>(animated));

            *next_ = &window;
            next_++;
        }

    private:
        Window* stack_[16];
        Window** next_;
    };


    class Application
    {
    public:
        template<class TApp>
        static void Run()
        {
            TApp app;
            app.StartEventLoop();
        }

        static Application& Base()
        {
            return *singleton;
        }

        WindowStack& window_stack()
        {
            return window_stack_;
        }

    protected:
        Application()
        {
            singleton = this;
        };

    private:
        void StartEventLoop()
        {
            sdk::app_event_loop();
        }

        WindowStack window_stack_;
        static Application* singleton;
    };
}

#endif //PEBBLE_CPP_WINDOW_HPP
