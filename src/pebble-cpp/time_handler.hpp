//
// Created by matt on 28/11/15.
//

#ifndef PEBBLE_CPP_TIMER_HANDLER_HPP
#define PEBBLE_CPP_TIMER_HANDLER_HPP

#include "pebble-sdk.hpp"
#include "util/duration.hpp"

namespace pebble
{
    template <class TController>
    class TimeHandler
    {
    public:
        class Timer
        {
        public:
            ~Timer()
            {
                if (sdk_reference_ != nullptr)
                {
                    ::app_timer_cancel(sdk_reference_);
                }
            }

            util::Duration duration() const { return duration_; }

            void Reshedule(util::Duration duration)
            {
                ::app_timer_reschedule(sdk_reference_, duration.total_milliseconds());
            }

        private:
            friend class TimeHandler;

            Timer(::AppTimer* sdk_reference, util::Duration duration)
                :
                duration_(duration),
                sdk_reference_(sdk_reference)
            {
            }

            Timer(Timer&& controller)
                :
                sdk_reference_(controller.sdk_reference_)
            {
                controller.sdk_reference_ = nullptr;
            }

            util::Duration duration_;
            ::AppTimer* sdk_reference_;
        };

        TimeHandler(TController& controller, util::Duration duration)
        :
            controller_(&controller),
            timer_(::app_timer_register(duration.total_milliseconds(), TimeCallback, this), duration)
        {
        }

        TimeHandler(TimeHandler&& other)
        :
            controller_(other.controller_),
            timer_(other.timer_)
        {
        }

    private:

        static void TimeCallback(void* data)
        {
            auto& handler(*static_cast< TimeHandler<TController>* >(data));
            handler.controller_->OnTimer(handler.timer_);
        }

        TController* controller_;
        Timer timer_;
    };
}

#endif //PEBBLE_CPP_TIMER_HANDLER_HPP
