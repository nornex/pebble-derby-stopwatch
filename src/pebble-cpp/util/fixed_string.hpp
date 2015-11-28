//
// Created by matt on 15/11/15.
//

#ifndef PEBBLE_CPP_FIXED_STRING_HPP
#define PEBBLE_CPP_FIXED_STRING_HPP

#include "../pebble-sdk.hpp"

#include <stddef.h>
#include <stdint.h>
#include <cstdio>
#include <cstring>

#include "calendar_time.hpp"

namespace util
{
    template<uint16_t TSize>
    class FixedString
    {
    public:
        static_assert(TSize > 0, "Size must be larger than 0");

        FixedString()
        :
            length_(0)
        {
            str_[0] = '\0';
        }

        template <size_t TOtherSize>
        explicit FixedString(const FixedString<TOtherSize>& other)
        :
            length_(std::min(other.length(), TSize - 1))
        {
            std::memcpy(str_, other.c_str(), length_);
            str_[length_] = '\0';
        }

        explicit FixedString(const char* other)
        :
            length_(static_cast<uint16_t>(std::min(
                std::strlen(other), static_cast<size_t>(TSize - 1)
            )))
        {
            std::memcpy(str_, other, length_);
            str_[length_] = '\0';
        }

        const char* c_str() const { return str_; }

        uint16_t length() const { return length_; }

        template <class TWrapper> inline
        FixedString& SetFromTime(const CalendarTimeBase<TWrapper>& time, const char* format)
        {
            length_ = ::strftime(str_, TSize, format, &(time.get_raw()));
            return *this;
        }

        template <class... TParams> inline
        FixedString& SetFromFormat(const char* format, TParams... params)
        {
            length_ = std::sprintf(str_, format, params...);
            return *this;
        }

    private:
        uint16_t length_;
        char str_[TSize];
    };

    namespace convert
    {
        inline
        static const char* CString(const char* str) { return str; }

        template <uint16_t TSize> inline
        static const char* CString(const FixedString<TSize>& str) { return str.c_str(); }
    }
}

#endif //PEBBLE_CPP_FIXED_STRING_HPP
