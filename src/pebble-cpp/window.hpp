//
// Created by matt on 08/11/15.
//

#ifndef PEBBLE_CPP_WINDOW_HPP
#define PEBBLE_CPP_WINDOW_HPP

#include <functional>

#include "pebble-sdk.hpp"
#include "layer.hpp"
#include "optional.hpp"

namespace pebble
{
    class Window
    {
    public:
        /// Move constructor for a window, takes ownership of the object if the original also had ownership.
        Window(Window&& window);

        /// Destructor, will free if underlying SDK window is owned by this object
        ~Window();

        Layer& root_layer();

        sdk::Window* sdk_handle();

        template <class THandler>
        static Window CreateWithHandler();

    private:
        Window();

        Window(sdk::Window* window);

        Window(const Window&) = delete;

        sdk::Window* window_;
        bool owned_;

        util::Optional<Layer> layer_;
        void* handler_ = nullptr;

        template <class THandler>
        class Handler
        {
            friend class Window;

            static void ForWindow(Window& window);

            static void Load(sdk::Window* sdk_window);
            static void Unload(sdk::Window* sdk_window);
            static void Appear(sdk::Window* sdk_window);
            static void Disappear(sdk::Window* window);

        };
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
        :
            next_(&stack_[0])
        {
        }

        Window* Find(sdk::Window* window)
        {
            Window** current = next_;
            do
            {
                current--;
                if (current < stack_) return nullptr;
            }
            while ((*current)->sdk_handle() != window);

            return *current;
        }

        void Push(Window& window, Animate animated)
        {
            if (next_ > stack_ + sizeof(stack_))
            {
                // TODO(matt) Logging
                return;
            }

            *next_ = &window;
            next_++;

            window_stack_push(window.sdk_handle(), static_cast<bool>(animated));
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

#include "window.inline.hpp"

#endif //PEBBLE_CPP_WINDOW_HPP
