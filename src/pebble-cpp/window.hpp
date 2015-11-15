//
// Created by matt on 08/11/15.
//

#ifndef PEBBLE_CPP_WINDOW_HPP
#define PEBBLE_CPP_WINDOW_HPP

#include <functional>

#include "pebble-sdk.hpp"
#include "layer.hpp"
#include "util/optional.hpp"

namespace pebble
{
    /// Wrapper around the Pebble SDK's Window pointer. The TEventHandler object is created when the window's load
    /// handler is called, and destroyed when unload is called.
    template <class TEventHandler>
    class Window
    {
    public:
        /// Constructor
        Window();

        /// Destructor, will free if underlying SDK window is owned by this object
        ~Window();

        /// Get the base UI layer of this window.
        Layer& root_layer();

        /// Get the Pebble SDK's pointer for this object.
        ::Window* sdk_handle();

        /// Lookup a window object based on the SDK pointer.
        static Window<TEventHandler>& FromSdkHandle(::Window* sdk_handle);

        util::Optional<TEventHandler>& event_handler()
        {
            return handler_.lifetime_;
        }

    private:
        Window(Window&& window) = delete;
        Window(const Window&) = delete;

        ::Window* window_;

        util::Optional<Layer> layer_;

        class EventHandler
        {
        public:
            EventHandler(::Window* window);

            util::Optional<TEventHandler> lifetime_;

            static void Load(::Window* sdk_window);
            static void Unload(::Window* sdk_window);
            static void Appear(::Window* sdk_window);
            static void Disappear(::Window* window);
        };

        EventHandler handler_;
    };

    enum class Animate : bool
    {
        SlideIn = true,
        None = false
    };

    class WindowStack
    {
    public:
        WindowStack()
        {
        }

        template<class THandler>
        void Push(Window<THandler>& window, Animate animated)
        {
            window_stack_push(window.sdk_handle(), static_cast<bool>(animated));
        }
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

        static Application& GetStaticBase()
        {
            return *singleton;
        }

        template <class TApp>
        static TApp& GetStaticCast()
        {
            return *static_cast<TApp*>(singleton);
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
            ::app_event_loop();
        }

        WindowStack window_stack_;
        static Application* singleton;
    };
}

#include "window.inline.hpp"

#endif //PEBBLE_CPP_WINDOW_HPP
