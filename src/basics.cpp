
#include "pebble-cpp/window.hpp"
#include "pebble-cpp/tick_handler.hpp"

class CountdownTimer
{
public:
    typedef uint8_t TSeconds;

    CountdownTimer(TSeconds max_seconds)
    :
        max_seconds_(max_seconds)
    {
    }

    CountdownTimer(const CountdownTimer&) = delete;

public:
    TSeconds total_elapsed_seconds() const {  return elapsed_seconds_; }
    TSeconds total_remaining_seconds() const {  return max_seconds_ - elapsed_seconds_;  }
    TSeconds total_max_seconds() const { return max_seconds_; }

    uint8_t minutes() const { return total_remaining_seconds() / 60; }
    uint8_t seconds() const { return total_remaining_seconds() % 60; }

    void Reset()
    {
        elapsed_seconds_ = 0;
    }

    void decrement_seconds(TSeconds seconds)
    {
        if (total_remaining_seconds() <= seconds)
        {
            elapsed_seconds_ = max_seconds_;
        }
        else
        {
            elapsed_seconds_ += seconds;
        }
    }

private:
    const TSeconds max_seconds_;
    TSeconds elapsed_seconds_ = 0;
};

struct Stopwatch : pebble::WindowController<Stopwatch, pebble::ClickHandling::On>
{
    TWindow& window_;
    pebble::TextLayer clock_text_;
    pebble::OnTickHandler<Stopwatch> tick_handler_;

    enum class Period
    {
        TimingJam,
        BetweenJams
    };

    bool paused_ = true;
    Period period_ = Period::TimingJam;
    CountdownTimer jam_countdown_ = {60 * 2 };
    CountdownTimer between_jam_countdown_ = {30 };

    util::FixedString<8> time_str_;

    Stopwatch(TWindow& window);

    static Stopwatch& GetStatic();

    void SetupClickHandlers(TClickHandlerSetup& setup);
    void OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask);
    void OnSingleClick(pebble::ClickInfo click_info);
    void OnLongClick(pebble::ClickInfo click_info, pebble::ButtonState state);

    CountdownTimer& current_countdown()
    {
        switch (period_)
        {
            case Period::TimingJam:   return jam_countdown_;
            case Period::BetweenJams: return between_jam_countdown_;
        }
        return jam_countdown_;
    }
};

struct App : public pebble::Application<App>
{
    Stopwatch::TWindow main_window_;

    App()
    {
        window_stack().Push(main_window_, pebble::Animate::SlideIn);
    }
};

/*--------------------------------------------------------------------------------------------------------------------*\
 * Stopwatch - Implementation
\*--------------------------------------------------------------------------------------------------------------------*/

Stopwatch::Stopwatch(TWindow& window)
:
    window_(window),
    clock_text_(0, PBL_IF_ROUND_ELSE(58, 52), window.root_layer().width(), 50),
    tick_handler_(pebble::TimeUnit::Minute)
{
    clock_text_.SetBackgroundColor(GColorClear);
    clock_text_.SetTextColor(pebble::Color(255, 0, 0));
    clock_text_.SetFont(pebble::Font(FONT_KEY_BITHAM_42_BOLD));
    clock_text_.SetAlignment(pebble::Alignment::Center);

    OnTick(
        util::CalendarTimeRef::Localtime(),
        pebble::TimeUnitMask(pebble::TimeUnit::Minute)
    );

    window.root_layer().AddChild(clock_text_);
}

Stopwatch& Stopwatch::GetStatic()
{
    return App::GetStatic().main_window_.controller().unsafe_get();
}

void Stopwatch::SetupClickHandlers(TClickHandlerSetup& setup)
{
    setup.SetupSingleClick(pebble::Button::Select);
    setup.SetupLongClick(pebble::Button::Select);
}

void Stopwatch::OnSingleClick(pebble::ClickInfo click_info)
{
    paused_ = !paused_;
}

void Stopwatch::OnLongClick(pebble::ClickInfo click_info, pebble::ButtonState state)
{
    if (state == pebble::ButtonState::Up)
    {
        paused_ = false;
        current_countdown().Reset();
    }
}

void Stopwatch::OnTick(const util::CalendarTimeRef& time, pebble::TimeUnitMask)
{
    if (paused_) return;

    auto& countdown(current_countdown());

    countdown.decrement_seconds(1);

    clock_text_.SetText(time_str_.SetFromFormat(
        "%uh:%uh", countdown.minutes(), countdown.seconds()
    ));
}

extern "C" int main(void)
{
    App::Run();
}
