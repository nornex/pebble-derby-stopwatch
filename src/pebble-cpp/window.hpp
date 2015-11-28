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
    private:

        class EventHandler
        {
        public:
            EventHandler(::Window *sdk_window)
            {
                ::window_set_window_handlers(
                    sdk_window,
                    (WindowHandlers) {
                        .load =         &Load,
                        .appear =       &Appear,
                        .disappear =    &Disappear,
                        .unload =       &Unload
                    }
                );
            }

            util::Optional<TController> lifetime_;

            // Load handlers

            static void Load(::Window* sdk_window)
            {
                auto& window(FromSdkHandle(sdk_window));
                window.controller().Emplace(window);
            }

            static void Unload(::Window* sdk_window)
            {
                auto& window(FromSdkHandle(sdk_window));
                window.controller().Clear();
            }

            static void Appear(::Window* sdk_window)
            {
                auto& window(FromSdkHandle(sdk_window));
                window.controller().unsafe_get().OnAppear();
            }

            static void Disappear(::Window* sdk_window)
            {
                auto& window(FromSdkHandle(sdk_window));
                window.controller().unsafe_get().OnDisappear();
            }

            // Click handlers

            static void SetupClickProviders(void* sdk_window)
            {
                auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
                TClickHandlerSetup setup;
                window.controller().unsafe_get().SetupClickHandlers(setup);
            }

            static void SingleClick(::ClickRecognizerRef ref, void* sdk_window)
            {
                auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
                window.controller().unsafe_get().OnSingleClick(ClickInfo(ref));
            }

            static void RepeatingClick(::ClickRecognizerRef ref, void* sdk_window)
            {
                auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
                window.controller().unsafe_get().OnRepeatingClick(ClickInfo(ref));
            }

            static void LongClickDown(::ClickRecognizerRef ref, void* sdk_window)
            {
                auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
                window.controller().unsafe_get().OnLongClick(ClickInfo(ref), ButtonState::Down);
            }

            static void LongClickUp(::ClickRecognizerRef ref, void* sdk_window)
            {
                auto& window(FromSdkHandle(static_cast< ::Window* >(sdk_window)));
                window.controller().unsafe_get().OnLongClick(ClickInfo(ref), ButtonState::Up);
            }
        };

    public:
        typedef Window<TController, TClickHandling> TWindow;

        /// Creates a new Pebble window
        Window()
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

        /// Destructor, will free underlying SDK window.
        ~Window()
        {
            ::window_destroy(window_);
        }

        /// Get the base UI layer of this window.
        Layer &root_layer()
        {
            if (layer_.is_none())
            {
                layer_.Emplace(window_get_root_layer(window_));
            }

            return layer_.unsafe_get();
        }

        /// Get the Pebble SDK's pointer for this object.
        ::Window *sdk_handle() { return window_; }

        /// Lookup a window object based on the SDK pointer.
        static TWindow &FromSdkHandle(::Window *sdk_handle)
        {
            return *static_cast<Window*>(::window_get_user_data(sdk_handle));
        }

        /// Get the event handler, will only be set if the Window is loaded.
        util::Optional<TController>& controller()
        {
            return handler_.lifetime_;
        }

    private:
        typedef ClickHandlerSetup<TController> TClickHandlerSetup;

        Window(Window &&window) = delete;

        Window(const Window &) = delete;

        ::Window *window_;

        util::Optional<Layer> layer_;

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

        void SetupLongClick(Button button, uint16_t delay_ms = 0)
        {
            ::window_long_click_subscribe(
                static_cast< ::ButtonId >(button), delay_ms,
                TWindow::EventHandler::LongClickDown, TWindow::EventHandler::LongClickUp
            );
        }

    private:
        ClickHandlerSetup() = default;
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
    };
}

#endif //PEBBLE_CPP_WINDOW_HPP
