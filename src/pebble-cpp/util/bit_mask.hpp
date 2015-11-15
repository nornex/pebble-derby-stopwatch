//
// Created by matt on 15/11/15.
//

#ifndef PEBBLE_CPP_ENUM_BIT_MASK_HPP_HPP
#define PEBBLE_CPP_ENUM_BIT_MASK_HPP_HPP

#include "types.hpp"

namespace util
{
    template<class T>
    class EnumBitMask
    {
    public:
        typedef typename IntOfSameSize<T>::Type TInt;

        EnumBitMask() : value_(static_cast<TInt>(0)) {}

        EnumBitMask(TInt value) : value_(value) {}

        EnumBitMask(T unit) : value_(static_cast<TInt>(unit)) {}

        template<class... TUnits>
        EnumBitMask(T unit, TUnits... units)
        :
            value_(BitwiseOr(unit, units...))
        {
        }

        bool operator[](T val) const
        {
            return (static_cast<TInt>(val) & value_) != 0;
        }

        template <class... TUnits>
        void set(T val, TUnits... extra)
        {
            value_ |= BitwiseOr(val, extra...);
        }

        void clear()
        {
            value_ = static_cast<TInt>(0);
        }

        T as_enum() const
        {
            return static_cast<T>(value_);
        }

        template <class TBase>
        TBase as_base_enum() const
        {
            return static_cast<TBase>(value_);
        }

        TInt raw_int() const
        {
            return value_;
        }

        template<class... TUnits>
        static constexpr TInt BitwiseOr(T unit, TUnits... units)
        {
            return static_cast<TInt>(unit) | BitwiseOr(units...);
        }

        static constexpr TInt BitwiseOr(T unit)
        {
            return static_cast<TInt>(unit);
        }

        static constexpr T BitwiseOr()
        {
            static_cast<T>(0);
        }

    private:
        TInt value_;
    };
}

#endif //PEBBLE_CPP_ENUM_BIT_MASK_HPP_HPP
