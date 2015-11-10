//
// Created by matt on 09/11/15.
//

#ifndef PEBBLE_CPP_OPTIONAL_HPP
#define PEBBLE_CPP_OPTIONAL_HPP

#include <utility>

namespace util
{
    struct NoneType {};
    constexpr NoneType None = {};

    template<class TValue>
    class Optional
    {
    public:
        Optional() : initialized_(false)
        {
        }

        Optional(NoneType) : initialized_(false)
        {
        }

        Optional(TValue &&value)
        {
            UnsafeEmplace(std::move(value));
        }

        Optional(const TValue &value)
        {
            UnsafeEmplace(value);
        }

        Optional(Optional &&other)
        {
            UnsafeEmplace(std::move(other.data_.value));
            other.Clear();
        }

        Optional(const Optional &other)
        {
            UnsafeEmplace(other.data_.value);
        }

        bool is_set()
        {
            return initialized_;
        }

        bool is_none()
        {
            return !is_set();
        }

        void Set(TValue &&value)
        {
            if (this->is_set())
            {
                data_.value = std::move(value);
            }
            else
            {
                UnsafeEmplace(std::move(value));
            }
        }

        void Set(const TValue &value)
        {
            if (this->is_set())
            {
                data_.value = value;
            }
            else
            {
                new(data_) TValue(value);
                initialized_ = true;
            }
        }

        template<class... TArgs>
        void Emplace(TArgs &&... args)
        {
            this->Clear();
            UnsafeEmplace(std::forward<TArgs>(args)...);
        }

        TValue &UnsafeGet()
        {
            return data_.value;
        }

        template<class... TArgs>
        void EmplaceIfUnset(TArgs &&... args)
        {
            if (this->is_none())
            {
                UnsafeEmplace(std::forward<TArgs>(args)...);
            }
        }

        template<class... TArgs>
        TValue &GetOrEmplace(TArgs &&... args)
        {
            EmplaceIfUnset(std::forward<TArgs>(args)...);
            return UnsafeGet();
        }

        void Clear()
        {
            if (this->is_set())
            {
                (&(data_.value))->~TValue();
                initialized_ = false;
            }
        }

    private:

        template<class... TArgs>
        void UnsafeEmplace(TArgs &&... args)
        {
            new(data_.memory) TValue(std::forward<TArgs>(args)...);
            initialized_ = true;
        }

        union Union
        {
            Union() : memory() {}

            char memory[sizeof(TValue)];
            TValue value;
        };

        Union data_;
        bool initialized_;
    };

}

#endif //PEBBLE_CPP_OPTIONAL_HPP
