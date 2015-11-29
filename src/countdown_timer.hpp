//
// Created by matt on 28/11/15.
//

#ifndef PEBBLE_CPP_COUNTDOWN_TIMER_HPP
#define PEBBLE_CPP_COUNTDOWN_TIMER_HPP

#include "pebble-cpp/util/duration.hpp"
#include "pebble-cpp/time_handler.hpp"

class CountdownTimer
{
public:
    typedef util::Duration TDuration;

    CountdownTimer(TDuration max)
    :
        max_(max),
        remaining_(max)
    {
    }

    CountdownTimer(const CountdownTimer&) = delete;

    TDuration remaining() const { return remaining_; }
    TDuration max() const { return max_; }
    TDuration elapsed() const { return max() - remaining(); }

    bool is_finished() const { return remaining().total_milliseconds() == 0; }

    void Reset()
    {
        remaining_ = max_;
    }

    void decrement(TDuration duration)
    {
        if (remaining() <= duration)
        {
            remaining_ = TDuration::Zero();
        }
        else
        {
            remaining_ -= duration;
        }
    }

    void set_remaining(util::Duration duration)
    {
        if (duration > max())
        {
            Reset();
        }
        else
        {
            remaining_ = duration;
        }
    }

    void finish()
    {
        set_remaining(util::Duration::Zero());
    }

private:
    const TDuration max_;
    TDuration remaining_ = 0;
};



#endif //PEBBLE_CPP_COUNTDOWN_TIMER_HPP
