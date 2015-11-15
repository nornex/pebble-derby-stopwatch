
#include "pebble-cpp/window.hpp"
#include "pebble-cpp/handler.hpp"

struct MainWindow
{
    pebble::Window<MainWindow>& window_;
    pebble::TextLayer clock_text_;
    pebble::OnTickHandler<MainWindow> tick_handler_;

    util::FixedString<8> clock_str_;

    MainWindow(pebble::Window<MainWindow>& window)
    :
        window_(window),
        clock_text_(0, PBL_IF_ROUND_ELSE(58, 52), window.root_layer().width(), 50),
        tick_handler_(pebble::TimeUnit::Minute)
    {
        clock_text_.set_background_colour(GColorClear);
        clock_text_.set_text_colour(GColorBlack);
        clock_text_.set_font(pebble::Font(FONT_KEY_BITHAM_42_BOLD));
        clock_text_.set_alignment(pebble::Alignment::Center);

        OnTick(
            util::CalendarTimeRef(util::FromTimeSource::Localtime),
            pebble::TimeUnitMask(pebble::TimeUnit::Minute)
        );

        window.root_layer().AddChild(clock_text_);
    }

    void OnAppear() {}
    void OnDisappear() {}

    void OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask)
    {
        clock_str_.SetFromTime(time, "%H:%M");
        clock_text_.set_text(clock_str_);
    }

    static MainWindow& GetStatic();
};

struct App : public pebble::Application
{
    pebble::Window<MainWindow> main_window_;

    static App& GetStatic()
    {
        return pebble::Application::GetStaticCast<App>();
    }

    App()
    {
        window_stack().Push(main_window_, pebble::Animate::SlideIn);
    }
};

MainWindow& MainWindow::GetStatic()
{
    return App::GetStatic().main_window_.event_handler().unsafe_get();
}

extern "C" int main(void)
{
    pebble::Application::Run<App>();
}
