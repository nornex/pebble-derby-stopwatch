//
// Created by matt on 17/11/15.
//

#ifndef PEBBLE_CPP_CLICK_HANDLER_HPP
#define PEBBLE_CPP_CLICK_HANDLER_HPP

#include "pebble-sdk.hpp"
#include "util/types.hpp"
#include "util/bit_mask.hpp"

namespace pebble
{
    enum class ClickHandling { On, Off };

    enum class Button : typename util::IntOfSameSize< ::ButtonId >::Type
    {
        Back        = ::BUTTON_ID_BACK,
        Up          = ::BUTTON_ID_UP,
        Select      = ::BUTTON_ID_SELECT,
        Down        = ::BUTTON_ID_DOWN
    };

    enum class ButtonState
    {
        Down, Up
    };

    typedef util::EnumBitMask<Button> ButtonMask;

    class ClickInfo
    {
    public:
        ClickInfo(::ClickRecognizerRef sdk_reference) : sdk_reference_(sdk_reference) {}

        Button button() const
        {
            return static_cast<Button>(::click_recognizer_get_button_id(sdk_reference_));
        }

        uint8_t num_of_clicks() const
        {
            return ::click_number_of_clicks_counted(sdk_reference_);
        }

        bool is_repeating() const
        {
            return ::click_recognizer_is_repeating(sdk_reference_);
        }

    private:
        ::ClickRecognizerRef sdk_reference_;
    };
}


#endif //PEBBLE_CPP_CLICK_HANDLER_HPP
