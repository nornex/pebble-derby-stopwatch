
#include "pebble-cpp/window.hpp"

struct MainWindowHandler
{
    static void Load(pebble::Window& window)
    {
        App::Get()
    }

    static void Unload(pebble::Window& window)
    {

    }
};

class App : public pebble::Application
{
public:
    App()
    :
        main_window_(pebble::Window::CreateWithHandler<MainWindowHandler>())
    {
        singleton = this;
        window_stack().Push(main_window_, pebble::Animate::SlideIn);
    }

    static App & Get()
    {
        return *singleton;
    }

private:
    pebble::Window main_window_;

    static App * singleton = nullptr;
};

extern "C" int main(void)
{
    pebble::Application::Run<App>();
}
