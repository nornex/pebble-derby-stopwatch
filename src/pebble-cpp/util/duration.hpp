//
// Created by matt on 28/11/15.
//

#ifndef PEBBLE_CPP_DURATION_HPP
#define PEBBLE_CPP_DURATION_HPP

namespace util
{
    template <class TValue>
    class SizedDuration
    {
    public:
        SizedDuration() : milliseconds_(static_cast<TValue>(0)) {}
        SizedDuration(const SizedDuration& other) : milliseconds_(other.milliseconds_) {}
        SizedDuration(TValue milliseconds) : milliseconds_(milliseconds) {}

        TValue total_milliseconds() const { return milliseconds_; }
        TValue total_seconds() const { return total_milliseconds() / 1000; }
        TValue total_minutes() const { return total_seconds() / 60; }
        TValue total_hours() const { return total_minutes() / 60; }

        TValue milliseconds() const { return milliseconds_ % 1000; }
        TValue seconds() const { return total_seconds() % 60; }
        TValue minutes() const { return total_minutes() % 60; }

        SizedDuration operator+(SizedDuration other) const
        {
            return SizedDuration(total_milliseconds() + other.total_milliseconds());
        }

        SizedDuration operator-(SizedDuration other) const
        {
            return SizedDuration(total_milliseconds() - other.total_milliseconds());
        }

        bool operator==(SizedDuration other) const
        {
            return milliseconds_ == other.milliseconds_;
        }

        bool operator!=(SizedDuration other) const
        {
            return milliseconds_ != other.milliseconds_;
        }

        bool operator>(SizedDuration other) const
        {
            return milliseconds_ > other.milliseconds_;
        }

        bool operator>=(SizedDuration other) const
        {
            return milliseconds_ >= other.milliseconds_;
        }

        bool operator<(SizedDuration other) const
        {
            return milliseconds_ < other.milliseconds_;
        }

        bool operator<=(SizedDuration other) const
        {
            return milliseconds_ <= other.milliseconds_;
        }

        SizedDuration& operator=(SizedDuration other)
        {
            milliseconds_ = other.milliseconds_;
            return *this;
        }

        SizedDuration& operator+=(SizedDuration other)
        {
            milliseconds_ += other.milliseconds_;
            return *this;
        }

        SizedDuration& operator-=(SizedDuration other)
        {
            milliseconds_ -= other.milliseconds_;
            return *this;
        }

        bool is_zero()
        {
            return milliseconds_ == 0;
        }

        static SizedDuration Zero()
        {
            return SizedDuration(0);
        }

    private:
        TValue milliseconds_;
    };

    typedef SizedDuration<int> Duration;

    static Duration Milliseconds(int ms)
    {
        return Duration(ms);
    }

    static Duration Seconds(int seconds)
    {
        return Milliseconds(seconds * 1000);
    }

    static Duration Minutes(int minutes)
    {
        return Seconds(minutes * 60);
    }

    static Duration Hours(int hours)
    {
        return Minutes(hours * 60);
    }
}

#endif //PEBBLE_CPP_DURATION_HPP
