//
// Created by matt on 08/11/15.
//

#ifndef PEBBLE_CPP_WINDOW_HPP
#define PEBBLE_CPP_WINDOW_HPP

#include <functional>

#include "pebble-sdk.hpp"
#include "layer.hpp"
#include "util/optional.hpp"
#include "click_handler.hpp"

namespace pebble
{
    template<class TController>  class ClickHandlerSetup;

    /// Wrapper around the Pebble SDK's Window pointer. The TController object is created when the window's load
    /// handler is called, and destroyed when unload is called.
    template<class TController, ClickHandling TClickHandling = ClickHandling::Off>
    class Window
    {
    public:
        typedef Window<TController, TClickHandling> TWindow;

        /// Constructor
        Window();

        /// Destructor, will free if underlying SDK window is owned by this object
        ~Window();

        /// Get the base UI layer of this window.
        Layer &root_layer();

        /// Get the Pebble SDK's pointer for this object.
        ::Window *sdk_handle();

        /// Lookup a window object based on the SDK pointer.
        static TWindow &FromSdkHandle(::Window *sdk_handle);

        /// Get the event handler, will only be set if the Window is loaded.
        util::Optional<TController> &event_handler()
        {
            return handler_.lifetime_;
        }

    private:
        typedef ClickHandlerSetup<TController> TClickHandlerSetup;

        Window(Window &&window) = delete;

        Window(const Window &) = delete;

        ::Window *window_;

        util::Optional<Layer> layer_;

        class EventHandler
        {
        public:
            EventHandler(::Window *sdk_window);

            util::Optional<TController> lifetime_;

            static void Load(::Window* sdk_window);
            static void Unload(::Window* sdk_window);
            static void Appear(::Window* sdk_window);
            static void Disappear(::Window* sdk_window);

            static void SetupClickProviders(void* sdk_window);
            static void SingleClick(::ClickRecognizerRef ref, void* sdk_window);
            static void RepeatingClick(::ClickRecognizerRef ref, void* sdk_window);
        };

        EventHandler handler_;

        friend TClickHandlerSetup;
    };

    /// Contains methods for setting up click handling
    template<class TController>
    class ClickHandlerSetup
    {
    private:
        typedef Window<TController, ClickHandling::On> TWindow;
    public:
        void SetupSingleClick(Button button)
        {
            ::window_single_click_subscribe(
                static_cast< ::ButtonId >(button), TWindow::EventHandler::SingleClick
            );
        }

        void SetupRepeatingClick(Button button, uint16_t interval_ms)
        {
            ::window_single_repeating_click_subscribe(
                static_cast< ::ButtonId >(button), interval_ms, TWindow::EventHandler::RepeatingClick
            );
        }

    private:
        ClickHandlerSetup() {}
        friend class TWindow::EventHandler;
    };

    template <class TController, pebble::ClickHandling TClickHandling>
    class WindowController
    {
    public:
        typedef pebble::Window<TController, TClickHandling> TWindow;
        typedef pebble::ClickHandlerSetup<TController> TClickHandlerSetup;

        void OnAppear() {}
        void OnDisappear() {}

        //template <class T, ClickHandling C> inline
        //void SetupClickHandlers(typename Window<T, C>::ClickHandlerSetup& setup) {}
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

        template<class THandler, ClickHandling TClickHandling>
        void Push(Window<THandler, TClickHandling> &window, Animate animated)
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

        static Application &GetStaticBase()
        {
            return *singleton;
        }

        template<class TApp>
        static TApp &GetStaticCast()
        {
            return *static_cast<TApp *>(singleton);
        }

        WindowStack &window_stack()
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
        static Application *singleton;
    };

    /*----------------------------------------------------------------------------------------------------------------*\
     * Window - Implementation
    \*----------------------------------------------------------------------------------------------------------------*/

    template<class TController, ClickHandling TClickHandling> inline
    Window<TController, TClickHandling>::Window()
    :
        window_(::window_create()),
        handler_(window_)
    {
        ::window_set_user_data(window_, this);

        if (TClickHandling == ClickHandling::On)
        {
            ::window_set_click_config_provider(window_, EventHandler::SetupClickProviders);
        }
    }

    template<class TController, ClickHandling TClickHandling> inline
    Window<TController, TClickHandling>::~Window()
    {
        ::window_destroy(window_);
    }

    template<class TController, ClickHandling TClickHandling> inline
    Layer& Window<TController, TClickHandling>::root_layer()
    {
        if (layer_.is_none())
        {
            layer_.Emplace(window_get_root_layer(window_));
        }

        return layer_.unsafe_get();
    }

    template<class TController, ClickHandling TClickHandling> inline
    ::Window* Window<TController, TClickHandling>::sdk_handle()
    {
        return window_;
    }

    template<class TController, ClickHandling TClickHandling> inline
    Window<TController, TClickHandling>& Window<TController, TClickHandling>::FromSdkHandle(::Window* sdk_handle)
    {
        return *static_cast<Window *>(::window_get_user_data(sdk_handle));
    }

    /*----------------------------------------------------------------------------------------------------------------*\
     * Window::EventHandler - Implementation
    \*----------------------------------------------------------------------------------------------------------------*/

    template<class TController, ClickHandling TClickHandling> inline
    Window<TController, TClickHandling>::EventHandler::EventHandler(::Window* window)
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

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::Load(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().Emplace(window);
    }

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::Unload(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().Clear();
    }

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::Appear(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().unsafe_get().OnAppear();
    }

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::Disappear(::Window* sdk_window)
    {
        auto& window(FromSdkHandle(sdk_window));
        window.event_handler().unsafe_get().OnDisappear();
    }

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::SetupClickProviders(void* sdk_window)
    {
        auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
        TClickHandlerSetup setup;
        window.event_handler().unsafe_get().SetupClickHandlers(setup);
    }

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::SingleClick(::ClickRecognizerRef ref, void* sdk_window)
    {
        auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
        ClickInfo info(ref);
        window.event_handler().unsafe_get().OnSingleClick(info);
    }

    template<class TController, ClickHandling TClickHandling> inline
    void Window<TController, TClickHandling>::EventHandler::RepeatingClick(::ClickRecognizerRef ref, void* sdk_window)
    {
        auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
        ClickInfo info(ref);
        window.event_handler().unsafe_get().OnRepeatingClick(info);
    }

}

#endif //PEBBLE_CPP_WINDOW_HPP
