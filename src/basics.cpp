
#include "pebble-cpp/application.hpp"
#include "pebble-cpp/tick_handler.hpp"
#include "countdown_timer.hpp"
#include "pebble-cpp/vibration.hpp"
#include "pebble-cpp/text_layer.hpp"
#include "pebble-cpp/graphics.hpp"


struct Stopwatch : pebble::WindowController<Stopwatch, pebble::ClickHandling::On>
{
    enum class Period  { Jam, Break };

    struct Background : public pebble::GraphicsLayer<Background>
    {
        Background(pebble::Rect& rect) : pebble::GraphicsLayer<Background>(*this, rect) {}
        void OnGraphicsUpdate(pebble::Graphics& g);
    };

    TWindow& window_;
    Background background_;
    pebble::TextLayerWithBuffer<8> jam_clock_layer_;
    pebble::TextLayerWithBuffer<16> prev_jam_clock_layer_;
    pebble::TextLayerWithBuffer<8> break_clock_layer_;
    pebble::OnTickHandler<Stopwatch> tick_handler_;

    pebble::Vibration start_of_jam_vibration_;
    util::Optional<pebble::CustomVibration<20>> end_of_jam_vibration_;

    bool paused_ = true;
    Period period_ = Period::Break;
    CountdownTimer jam_countdown_ = { util::Minutes(2) };
    CountdownTimer prev_jam_countdown_ = { util::Minutes(2) };
    CountdownTimer break_countdown_ = { util::Seconds(30) };

    Stopwatch(TWindow& window);

    static Stopwatch& GetStatic();

    void SetupClickHandlers(TClickHandlerSetup& setup);
    void OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask);
    void OnSingleClick(pebble::ClickInfo click_info);
    void OnLongClick(pebble::ClickInfo click_info, pebble::ButtonState state);

    void SwitchPeriod(Period new_period);
    void RedrawClock();
};

struct App : public pebble::Application<App>
{
    Stopwatch::TWindow main_window_;

    App()
    {
        window_stack().Push(main_window_, pebble::Animate::SlideIn);
    }
};

extern "C" int main(void)
{
    App::Run();
}

/*--------------------------------------------------------------------------------------------------------------------*\
 * Stopwatch - Implementation
\*--------------------------------------------------------------------------------------------------------------------*/

Stopwatch::Stopwatch(TWindow& window)
:
    window_(window),
    background_(window.root_layer()),
    jam_clock_layer_(0, 22, window.root_layer().width(), 50),
    prev_jam_clock_layer_(0, 63, window.root_layer().width(), 30),
    break_clock_layer_(0, 116, window.root_layer().width(), 35),
    tick_handler_(pebble::TimeUnit::Second)
{
    jam_clock_layer_.SetBackgroundColor(GColorClear);
    jam_clock_layer_.SetFont(pebble::Font(FONT_KEY_BITHAM_42_BOLD));
    jam_clock_layer_.SetAlignment(pebble::Alignment::Center);

    prev_jam_clock_layer_.SetBackgroundColor(GColorClear);
    prev_jam_clock_layer_.SetFont(pebble::Font(FONT_KEY_GOTHIC_18));
    prev_jam_clock_layer_.SetAlignment(pebble::Alignment::Center);

    break_clock_layer_.SetBackgroundColor(GColorClear);
    break_clock_layer_.SetFont(pebble::Font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
    break_clock_layer_.SetAlignment(pebble::Alignment::Center);

    SwitchPeriod(Period::Break);

    paused_ = true;
    prev_jam_countdown_.finish();

    RedrawClock();

    window.root_layer().AddChild(background_);
    window.root_layer().AddChild(jam_clock_layer_);
    window.root_layer().AddChild(prev_jam_clock_layer_);
    window.root_layer().AddChild(break_clock_layer_);
}

Stopwatch& Stopwatch::GetStatic()
{
    return App::GetStatic().main_window_.controller().unsafe_get();
}

void Stopwatch::SetupClickHandlers(TClickHandlerSetup& setup)
{
    setup.SetupLongClick(pebble::Button::Up);
    setup.SetupSingleClick(pebble::Button::Select);
    setup.SetupSingleClick(pebble::Button::Down);
}

void Stopwatch::OnSingleClick(pebble::ClickInfo click_info)
{
    switch (click_info.button())
    {
        case pebble::Button::Select:
        {
            paused_ = !paused_;
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Pause changed: %d", paused_);
            break;
        }
        case pebble::Button::Down:
        {
            if (period_ == Period::Jam)
            {
                SwitchPeriod(Period::Break);
            }
            else if (period_ == Period::Break)
            {
                if (break_countdown_.remaining() > util::Seconds(5))
                {
                    break_countdown_.set_remaining(util::Seconds(5));
                    RedrawClock();
                }
                else
                {
                    SwitchPeriod(Period::Jam);
                }
            }
            break;
        }
        default: break;
    }
}

void Stopwatch::OnLongClick(pebble::ClickInfo click_info, pebble::ButtonState state)
{
    if (state != pebble::ButtonState::Down)
    {
        return;
    }

    switch (click_info.button())
    {
        case pebble::Button::Up:
        {
            if (period_ == Period::Break && !prev_jam_countdown_.is_finished())
            {
                jam_countdown_.set_remaining(prev_jam_countdown_.remaining());
                SwitchPeriod(Period::Jam);
            }
            break;
        }
        default:
        {
            OnSingleClick(click_info);
            break;
        }
    }
}

void Stopwatch::SwitchPeriod(Period new_period)
{
    switch (new_period)
    {
        case Period::Jam:
        {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Switch to JAM");
            jam_clock_layer_.SetTextColor(pebble::Color::Black());
            prev_jam_clock_layer_.SetTextColor(pebble::Color::DarkGray());
            break_clock_layer_.SetTextColor(pebble::Color::LightGray());

            prev_jam_countdown_.finish();
            break_countdown_.finish();
            break;
        }
        case Period::Break:
        {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Switch to BREAK");
            jam_clock_layer_.SetTextColor(pebble::Color::LightGray());
            prev_jam_clock_layer_.SetTextColor(pebble::Color::Black());
            break_clock_layer_.SetTextColor(pebble::Color::Black());

            prev_jam_countdown_.set_remaining(jam_countdown_.remaining());
            jam_countdown_.Reset();
            break_countdown_.Reset();
            end_of_jam_vibration_.Clear();
            break;
        }
    }

    paused_ = false;
    period_ = new_period;
    RedrawClock();
}

void Stopwatch::OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask)
{
    bool need_redraw = !prev_jam_countdown_.is_finished();
    prev_jam_countdown_.decrement(util::Seconds(1));

    if (paused_)
    {
        if (need_redraw)
        {
            RedrawClock();
        }
        return;
    }

    switch (period_)
    {
        case Period::Jam:
        {
            need_redraw = !jam_countdown_.is_finished();
            jam_countdown_.decrement(util::Seconds(1));

            if (jam_countdown_.is_finished())
            {
                using namespace pebble::vibration;

                if (end_of_jam_vibration_.is_none())
                {
                    end_of_jam_vibration_.GetOrEmplace().StartRepeatingSequence<10>(
                        Pulse(util::Seconds(1)), Wait(util::Seconds(1))
                    );
                }
            }

            break;
        }
        case Period::Break:
        {
            need_redraw = true;
            break_countdown_.decrement(util::Seconds(1));

            if (break_countdown_.remaining() == util::Seconds(5))
            {
                start_of_jam_vibration_.ShortPulse();
            }
            else if (break_countdown_.is_finished())
            {
                jam_countdown_.Reset();
                start_of_jam_vibration_.LongPulse();
                SwitchPeriod(Period::Jam);
                need_redraw = false;
            }
            break;
        }
    }

    if (need_redraw)
    {
        RedrawClock();
    }
}

void Stopwatch::RedrawClock()
{
    jam_clock_layer_.SetFromFormat(
        "%u:%02u", jam_countdown_.remaining().minutes(), jam_countdown_.remaining().seconds()
    );

    if (period_ == Period::Jam || prev_jam_countdown_.is_finished())
    {
        prev_jam_clock_layer_.SetText("----");
    }
    else
    {
        prev_jam_clock_layer_.SetFromFormat(
            "Previous: %u:%02u", prev_jam_countdown_.remaining().minutes(), prev_jam_countdown_.remaining().seconds()
        );
    }

    break_clock_layer_.SetFromFormat(
        "%u:%02u", break_countdown_.remaining().minutes(), break_countdown_.remaining().seconds()
    );
}

void Stopwatch::Background::OnGraphicsUpdate(pebble::Graphics& g)
{
    using namespace pebble;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw it");

    const Font font(FONT_KEY_GOTHIC_18_BOLD);

    const Rect top_bar_bounds(0, 0, width(), 22);
    const Rect top_bar_text_bounds(
        top_bar_bounds.x()+ int16_t(3), top_bar_bounds.y() - int16_t(2),
        top_bar_bounds.width(), top_bar_bounds.height()
    );

    const Rect middle_bar_bounds(0, height() / int16_t(2) + int16_t(5), width(), top_bar_bounds.height());
    const Rect middle_text_bounds(
        top_bar_text_bounds.x(), middle_bar_bounds.y() - int16_t(1),
        middle_bar_bounds.width(), middle_bar_bounds.height()
    );

    g.DrawRectangleFilled(top_bar_bounds, Color::Black());
    g.DrawText("Jam", top_bar_text_bounds, font, Color::White());

    g.DrawRectangleFilled(middle_bar_bounds, Color::Black());
    g.DrawText("Break", middle_text_bounds, font, Color::White());
}
