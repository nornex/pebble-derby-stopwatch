//
// Created by matt on 15/11/15.
//

#ifndef PEBBLE_CPP_TIME_HPP
#define PEBBLE_CPP_TIME_HPP

#include "../pebble-sdk.hpp"

#include <algorithm>
#include <memory>

namespace util
{
    enum class Day : int
    {
        Sunday      = 0,
        Monday      = 1,
        Tuesday     = 2,
        Wednesday   = 3,
        Thursday    = 4,
        Friday      = 5,
        Saturday    = 6
    };

    typedef struct ::tm RawTime;

    template<class TWrapper>
    class CalendarTimeBase
    {
    public:
        CalendarTimeBase() = default;

        template <class... TParam>
        CalendarTimeBase(const TParam&... param) : time_(param...) {}

        /// Seconds. [0-60] (1 leap second)
        int seconds() const { return get_raw().tm_sec; }

        /// Minutes. [0-59]
        int minutes() const { return get_raw().tm_min; }
        
        /// Hours.  [0-23]
        int hours() const { return get_raw().tm_hour; }

        /// Day. [1-31]
        int day_of_month() const { return get_raw().tm_mday; }

        /// Month. [0-11]
        int month() const { return get_raw().tm_mon; }

        /// Years since 1900
        int offset_year() const { return get_raw().tm_year; }

        /// Year.
        int year() const { return 1900 + get_raw().tm_year; }

        /// Day of week. [0-6] Sunday = 0
        int day_of_week() const { return get_raw().tm_wday; }

        /// Day of week as a an enum
        Day weekday() const { return static_cast<Day>(get_raw().tm_wday); }

        /// Days in year.[0-365]
        int day_of_year() const { return get_raw().tm_yday; }

        /// DST. [-1/0/1]
        int dst() const { return get_raw().tm_isdst; }

        /// Seconds east of UTC
        int utc_seconds_offset() const { return get_raw().tm_gmtoff; }

        /// Timezone abbreviation
        const char* timezone() const { return get_raw().tm_zone; }

        ::tm& get_raw() { return time_.get(); }
        const ::tm& get_raw() const { return time_.get(); }

    private:
        TWrapper time_;
    };

    enum class FromTimeSource
    {
        Localtime
    };


    class TmValue
    {
    public:
        explicit TmValue(const ::tm& value)
        {
            std::memcpy(&value_, &value, sizeof(value));
        }

        TmValue(const TmValue&) = delete;

        ::tm& get() { return value_; }
        const ::tm& get() const { return value_; }

    private:
        ::tm value_;
    };

    typedef CalendarTimeBase<TmValue> CalendarTime;


    class TmPointer
    {
    public:
        TmPointer(::tm* ptr) : ptr_(ptr) {}

        template<class TWrapper>
        TmPointer(CalendarTimeBase<TWrapper>& value)
        :
            ptr_(&(value.get_raw()))
        {
        }

        TmPointer(const FromTimeSource time_source)
        {
            switch (time_source)
            {
                case FromTimeSource::Localtime:
                {
                    time_t current_unix_time;
                    ::time(&current_unix_time);

                    ptr_ = ::localtime(&current_unix_time);
                    break;
                }
            }
        }

        ::tm& get() { return *ptr_; }
        const ::tm& get() const { return *ptr_; }

    private:
        ::tm* ptr_;
    };

    class CalendarTimeRef : public CalendarTimeBase<TmPointer>
    {
    public:
        CalendarTimeRef(::tm* tm_ptr)
        :
            CalendarTimeBase<TmPointer>(tm_ptr)
        {
        }

        template<class TWrapper>
        CalendarTimeRef(CalendarTimeBase<TWrapper>& other)
        :
            CalendarTimeBase<TmPointer>(other)
        {
        }

        static CalendarTimeRef Localtime()
        {
            return CalendarTimeRef(FromTimeSource::Localtime);
        }

    private:
        CalendarTimeRef(const FromTimeSource time_source)
            :
            CalendarTimeBase<TmPointer>(time_source)
        {
        }
    };
}

#endif //PEBBLE_CPP_TIME_HPP
