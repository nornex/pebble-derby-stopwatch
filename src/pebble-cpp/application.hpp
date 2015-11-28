//
// Created by matt on 28/11/15.
//

#include "window.hpp"

#ifndef PEBBLE_CPP_APPLICATION_HPP
#define PEBBLE_CPP_APPLICATION_HPP

namespace pebble
{
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
        void Push(Window<THandler, TClickHandling>& window, Animate animated)
        {
            window_stack_push(window.sdk_handle(), static_cast<bool>(animated));
        }
    };


    template<class TController>
    class Application
    {
    public:
        static void Run()
        {
            TController app;
            app.StartEventLoop();
        }

        static TController& GetStatic()
        {
            return *static_cast<TController*>(singleton);
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

    template<class TController>
    Application<TController>* Application<TController>::singleton = nullptr;
}

#endif //PEBBLE_CPP_APPLICATION_HPP
