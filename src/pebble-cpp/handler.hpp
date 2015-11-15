//
// Created by matt on 15/11/15.
//

#ifndef PEBBLE_CPP_HANDLER_HPP
#define PEBBLE_CPP_HANDLER_HPP

#include "pebble-sdk.hpp"

#include "util/types.hpp"
#include "util/bit_mask.hpp"
#include "util/calendar_time.hpp"

namespace pebble
{
    enum class TimeUnit : typename util::IntOfSameSize< ::TimeUnits>::Type
    {
        Second =    ::SECOND_UNIT,
        Minute =    ::MINUTE_UNIT,
        Hour =      ::HOUR_UNIT,
        Day =       ::DAY_UNIT,
        Month =     ::MONTH_UNIT,
        Year =      ::YEAR_UNIT
    };

    typedef util::EnumBitMask<TimeUnit> TimeUnitMask;

    template <class TTickHandler>
    class OnTickHandler
    {
    public:
        template <class... TUnits>
        OnTickHandler(TimeUnit unit, TUnits... units)
        {
            const TimeUnitMask unit_mask(unit, units...);
            ::tick_timer_service_subscribe(unit_mask.as_base_enum< ::TimeUnits>(), OnTick);
        }

        ~OnTickHandler()
        {
            ::tick_timer_service_unsubscribe();
        }

    private:
        static void OnTick(::tm* tick_time, ::TimeUnits units)
        {
            TTickHandler::GetStatic().OnTick(
                util::CalendarTimeRef(tick_time),
                TimeUnitMask(static_cast<TimeUnitMask::TInt>(units))
            );
        }
    };
}

#endif //PEBBLE_CPP_HANDLER_HPP
