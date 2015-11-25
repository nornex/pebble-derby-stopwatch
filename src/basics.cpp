
#include "pebble-cpp/window.hpp"
#include "pebble-cpp/tick_handler.hpp"

struct MainWindow : pebble::WindowController<MainWindow, pebble::ClickHandling::On>
{
    TWindow& window_;
    pebble::TextLayer clock_text_;
    pebble::OnTickHandler<MainWindow> tick_handler_;
    util::FixedString<8> clock_str_;

    MainWindow(TWindow& window);

    static MainWindow& GetStatic();

    void SetupClickHandlers(TClickHandlerSetup& setup);
    void OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask);
    void OnSingleClick(pebble::ClickInfo click_info);
};

struct App : public pebble::Application
{
    MainWindow::TWindow main_window_;

    static App& GetStatic()
    {
        return pebble::Application::GetStaticCast<App>();
    }

    App()
    {
        window_stack().Push(main_window_, pebble::Animate::SlideIn);
    }
};

/*--------------------------------------------------------------------------------------------------------------------*\
 * MainWindow - Implementation
\*--------------------------------------------------------------------------------------------------------------------*/

MainWindow::MainWindow(TWindow& window)
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

MainWindow& MainWindow::GetStatic()
{
    return App::GetStatic().main_window_.event_handler().unsafe_get();
}

void MainWindow::SetupClickHandlers(TClickHandlerSetup& setup)
{
    setup.SetupSingleClick(pebble::Button::Up);
    setup.SetupSingleClick(pebble::Button::Down);
    setup.SetupSingleClick(pebble::Button::Select);
}

void MainWindow::OnSingleClick(pebble::ClickInfo click_info)
{
    switch (click_info.button())
    {
        case pebble::Button::Up:        APP_LOG(APP_LOG_LEVEL_DEBUG, "CLICK Up"); break;
        case pebble::Button::Back:      APP_LOG(APP_LOG_LEVEL_DEBUG, "CLICK Back"); break;
        case pebble::Button::Select:    APP_LOG(APP_LOG_LEVEL_DEBUG, "CLICK Select"); break;
        case pebble::Button::Down:      APP_LOG(APP_LOG_LEVEL_DEBUG, "CLICK Down"); break;
    }
}

void MainWindow::OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask)
{
    clock_str_.SetFromTime(time, "%H:%M");
    clock_text_.set_text(clock_str_);
}

extern "C" int main(void)
{
    pebble::Application::Run<App>();
}
