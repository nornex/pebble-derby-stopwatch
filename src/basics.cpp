
#include "pebble-cpp/window.hpp"

struct MainWindow
{
    pebble::Window& window_;
    pebble::TextLayer clock_text_;

    MainWindow(pebble::Window& window)
    :
        window_(window),
        clock_text_(0, PBL_IF_ROUND_ELSE(58, 52), window.root_layer().width(), 50)
    {
        clock_text_.set_background_colour(GColorClear);
        clock_text_.set_text_colour(GColorBlack);
        clock_text_.set_text("00:00");
        clock_text_.set_font(pebble::Font(FONT_KEY_BITHAM_42_BOLD));
        clock_text_.set_alignment(pebble::Alignment::Center);

        window.root_layer().AddChild(clock_text_);
    }

    ~MainWindow()
    {
    }

    void WindowAppear()
    {
    }

    void WindowDisappear()
    {

    }
};

struct App : public pebble::Application
{
    pebble::Window main_window_;

    App()
        : main_window_(pebble::Window::CreateWithHandler<MainWindow>())
    {
        window_stack().Push(main_window_, pebble::Animate::SlideIn);
    }
};

extern "C" int main(void)
{
    pebble::Application::Run<App>();
}
